#include "QuestionManager.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <random>
#include <cwctype>
#include <set>
#include <regex>
#include <map>
#include <vector>
#include <codecvt>
#include <locale>

using json = nlohmann::json;

void replaceAll(std::string& str, const std::string& from, const std::string& to) 
{
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) 
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string cleanHtmlEntities(std::string text)
{
    std::string buffer = text;
    std::regex tagRegex("<[^>]*>");
    buffer = std::regex_replace(buffer, tagRegex, "");
    replaceAll(buffer, "&quot;", "\"");
    replaceAll(buffer, "&#039;", "'");
    replaceAll(buffer, "&#39;", "'");
    replaceAll(buffer, "&amp;", "&");
    replaceAll(buffer, "&lt;", "<");
    replaceAll(buffer, "&gt;", ">");
    replaceAll(buffer, "&deg;", "°");
    replaceAll(buffer, "&rsquo;", "'");
    return buffer;
}

void formatQuestionText(std::wstring& text)
{
    if (text.empty()) return;
    text[0] = std::towupper(text[0]);
    while (!text.empty() && (text.back() == L'.' || text.back() == L' ')) 
    {
        text.pop_back();
    }
    if (text.empty() || text.back() != L'?') 
    {
        text += L'?';
    }
}

void formatAnswerText(std::wstring& text)
{
    if (text.empty()) return;
    text[0] = std::towupper(text[0]);
    while (!text.empty()) {
        wchar_t last = text.back();
        if (last == L'.' || last == L',' || last == L'!' || last == L'?' || last == L';' || last == L':' || last == L' ') {
            text.pop_back();
        } else {
            break;
        }
    }
}

size_t QuestionManager::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::wstring QuestionManager::utf8ToWide(const std::string& str)
{
    if (str.empty()) return L"";
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } 
    catch (...) 
    {
        return sf::String(str).toWideString();
    }
}

std::string QuestionManager::translateText(CURL* curl, std::string text)
{
    if (text.empty()) return "";
    std::string cleanText = cleanHtmlEntities(text);
    std::string readBuffer;
    
    char* encodedText = curl_easy_escape(curl, cleanText.c_str(), static_cast<int>(cleanText.length()));
    
    std::string url = "https://translate.googleapis.com/translate_a/single?client=gtx&sl=en&tl=cs&dt=t&q=" + std::string(encodedText);
    curl_free(encodedText);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) 
    {
        try 
        {

            auto jsonResponse = json::parse(readBuffer);
            
            if (jsonResponse.is_array() && !jsonResponse.empty() && jsonResponse[0].is_array()) 
            {
                std::string fullTranslation = "";
                
                for (auto& segment : jsonResponse[0]) 
                {
                    if (segment.is_array() && !segment.empty()) 
                    {
                        fullTranslation += segment[0].get<std::string>();
                    }
                }

                if (!fullTranslation.empty()) 
                {
                    return cleanHtmlEntities(fullTranslation);
                }
            }
        } catch (...) 
        { 
            return cleanText; 
        }
    }
    return cleanText;
}


std::vector<Question> QuestionManager::fetchQuestions(std::wstring category, std::wstring difficulty,std::atomic<int>* progressCounter)
{
    std::vector<Question> resultQuestions;
    
    static std::set<std::string> globalSeenQuestions; 
    
    CURL* curl = curl_easy_init();

    if (!curl) return resultQuestions;

    std::string baseUrl = "https://the-trivia-api.com/v2/questions?limit=100";
    
    std::string tags = "";
    if (category == L"Starověk") 
    {
        tags = "&tags=ancient_rome,ancient_greece,roman_empire,ancient_egypt,mesopotamia,"
        "babylon,persian_empire,alexander_the_great,julius_caesar,cleopatra,"
        "bronze_age,iron_age,mythology,ancient_world"; 
    } 
    else if (category == L"Středověk") 
    {
        tags = "&tags=middle_ages,medieval,crusades,vikings,black_death,feudalism,"
        "byzantine_empire,ottoman_empire,mongol_empire,genghis_khan,"
        "hundred_years_war,war_of_the_roses,joan_of_arc,knights,castles,holy_roman_empire";
    }
    else if (category == L"Moderní dějiny") 
    {
        tags = "&tags=world_war_1,world_war_2,cold_war,soviet_union,american_civil_war,"
        "industrial_revolution,french_revolution,napoleon,victorian_era,"
        "space_race,moon_landing,vietnam_war,korean_war,civil_rights_movement,"
        "berlin_wall,jfk,atomic_bomb,modern_history";
    }
    else if (category == L"Chaos")
    {
        tags = "&categories=history";
    }
    else
    {
        tags = "&categories=history";
    }

    std::string diffParam = "";
    if (difficulty == L"Lehká") diffParam = "&difficulty=easy";
    else if (difficulty == L"Střední") diffParam = "&difficulty=medium";
    else diffParam = "&difficulty=hard";

    std::srand(std::time(nullptr)); 
    std::string randomParam = "&random=" + std::to_string(std::rand());
    
    std::string currentUrl = baseUrl + tags + diffParam;

    int retryCount = 0;
    const int MAX_RETRIES = 15;

    while (resultQuestions.size() < 30 && retryCount < MAX_RETRIES)
    {
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, currentUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) 
        {
            try 
            {
                auto jsonArray = json::parse(readBuffer);
                
                if (jsonArray.is_array()) 
                {
                    for (auto& item : jsonArray) 
                    {
                        if (resultQuestions.size() >= 30) break;

                        std::string enQuestion = item["question"]["text"];

                        if (globalSeenQuestions.count(enQuestion) > 0)
                        {
                            continue;
                        }
                        globalSeenQuestions.insert(enQuestion);

                        Question q;
                        std::string enCorrect = item["correctAnswer"];
                        std::vector<std::string> enIncorrect;
                        for(auto& inc : item["incorrectAnswers"]) enIncorrect.push_back(inc);

                        std::string csQuestionStr = translateText(curl, enQuestion);                        
                        q.text = utf8ToWide(csQuestionStr);
                        formatQuestionText(q.text); 
                        
                        std::vector<std::wstring> answers;

                        std::wstring wCorrect = utf8ToWide(cleanHtmlEntities(enCorrect));
                        formatAnswerText(wCorrect);
                        answers.push_back(wCorrect);

                        for(auto& inc : enIncorrect) 
                        {
                            std::wstring wInc = utf8ToWide(cleanHtmlEntities(inc));
                            formatAnswerText(wInc);
                            answers.push_back(wInc);
                        }

                        std::wstring correctText = answers[0];
                        std::random_device rd;
                        std::mt19937 g(rd());
                        std::shuffle(answers.begin(), answers.end(), g);

                        for(int i=0; i<4; i++) {
                            if(answers[i] == correctText) {
                                q.correctIndex = i;
                                break;
                            }
                        }
                        q.answers = answers;
                        resultQuestions.push_back(q);

                        if (progressCounter !=nullptr)
                        {
                            (*progressCounter)++;
                        }
                        
                        std::cout << "Nacteno: " << resultQuestions.size() << "/30" << std::endl;
                    }
                }
            } catch (...) {
                std::cerr << "Chyba JSON (mozna dosly otazky nebo spatny format)" << std::endl;
            }
        }
        retryCount++;
    }

    if (resultQuestions.empty()) {
        std::cerr << "VAROVANI: Nepodarilo se stahnout otazky. Zkousim zalozni zdroj..." << std::endl;
    } 

    curl_easy_cleanup(curl);
    return resultQuestions;
}