#include "QuestionManager.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <random>
#include <cwctype>
#include <set>
#include <regex>

using json = nlohmann::json;

std::string cleanHtmlEntities(std::string text)
{
    std::string buffer = text;
    
    size_t pos;
    while ((pos = buffer.find("&quot;")) != std::string::npos) buffer.replace(pos, 6, "\"");
    while ((pos = buffer.find("&apos;")) != std::string::npos) buffer.replace(pos, 6, "'");
    while ((pos = buffer.find("&#039;")) != std::string::npos) buffer.replace(pos, 6, "'");
    while ((pos = buffer.find("&amp;")) != std::string::npos) buffer.replace(pos, 5, "&");
    while ((pos = buffer.find("&lt;")) != std::string::npos) buffer.replace(pos, 4, "<");
    while ((pos = buffer.find("&gt;")) != std::string::npos) buffer.replace(pos, 4, ">");
    while ((pos = buffer.find("&deg;")) != std::string::npos) buffer.replace(pos, 5, "°");

    std::regex tagRegex("<[^>]*>");
    buffer = std::regex_replace(buffer, tagRegex, "");

    return buffer;
}

void formatQuestionText(std::wstring& text)
{
    if (text.empty()) return;

    text[0] = std::towupper(text[0]);

    while (!text.empty() && text.back() == L'.') 
    {
        text.pop_back();
    }
    
    while (!text.empty() && text.back() == L' ') 
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

    while (!text.empty()) 
    {
        wchar_t last = text.back();
        
        if (last == L'.' || last == L',' || last == L'!' || last == L'?' || last == L';' || last == L':' || last == L' ') 
        {
            text.pop_back(); 
        } 
        else 
        {
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
    return sf::String::fromUtf8(str.begin(), str.end()).toWideString();
}

std::string QuestionManager::translateText(CURL* curl, std::string text)
{
    if (text.empty()) return "";
    std::string cleanText = cleanHtmlEntities(text);
    std::string readBuffer;

    char* encodedText = curl_easy_escape(curl, text.c_str(),static_cast<int>(text.length()));
    std::string url = "https://api.mymemory.translated.net/get?q=" + std::string(encodedText) + "&langpair=en|cs";
    curl_free(encodedText);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) 
    {
        try 
        {
            auto jsonResponse = json::parse(readBuffer);
            if (jsonResponse.contains("responseData") && !jsonResponse["responseData"].is_null()) 
            {
                return jsonResponse["responseData"]["translatedText"].get<std::string>();
            }
        } catch (...) {
            return text;
        }
    }
    return text;
}

bool containsAny(const std::string& text, const std::vector<std::string>& keywords)
{
    for (const auto& word : keywords) 
    {
        if (text.find(word) != std::string::npos) 
        {
            return true;
        }
    }
    return false;
}

bool isQuestionRelevant(const std::string& text, const std::wstring& category)
{
    std::string lowerText = text;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);

    std::vector<std::string> ancientKeywords = 
    {
        " bc", "b.c.", "ancient", "antiquity", // Časové určení
        "rome", "roman", "caesar", "augustus", "nero", "gladiator", "colosseum", // Řím
        "greece", "greek", "athens", "sparta", "alexander", "socrates", "plato", "olympus", // Řecko
        "egypt", "pharaoh", "pyramid", "nile", "cleopatra", "mummy", // Egypt
        "mesopotamia", "babylon", "sumer", "persia", "trojan", "hannibal", "carthage", // Ostatní
        "stone age", "bronze age", "iron age" // Pravěk/Starověk
    };

    std::vector<std::string> modernKeywords = 
    {
        "18", "19", "20", "21", // Století (1600s - 2000s) a roky začínající 19.. 20..
        "world war", "wwi", "wwii", "war i", "war ii", "cold war", "vietnam", "korea", // Války
        "nazi", "hitler", "soviet", "ussr", "stalin", "lenin", "communism", "berlin wall", // Režimy
        "america", "usa", "president", "kennedy", "lincoln", "washington", "independence", // USA
        "revolution", "french revolution", "industrial", "napoleon", // Revoluce
        "nuclear", "atomic", "space", "moon", "apollo", "internet", "computer", // Technologie
        "union", "republic", "prime minister", "thatcher", "churchill", "queen elizabeth ii"
    };

    std::vector<std::string> medievalKeywords = 
    {
        "middle ages", "medieval", "feudal", "dark ages", 
        "knight", "castle", "sword", "crusade", "templar", // Vojenství
        "king", "queen", "monarch", "crown", "throne", "dynasty", // Panovníci (obecné, ale časté ve středověku)
        "empire", "holy roman", "byzantine", "ottoman", "mongol", "genghis", "viking", // Říše
        "plague", "black death", "flea", // Nemoci
        "magna carta", "joan of arc", "charlemagne", "william the conqueror", // Osobnosti
        "hundred years", "rosese", "tudor", "henry viii" // Pozdní středověk
    };

    if (category == L"Starověk") 
    {
        if (containsAny(lowerText, ancientKeywords)) return true;
        return false;
    }
    else if (category == L"Moderní dějiny") 
    {
        if (containsAny(lowerText, modernKeywords)) return true;
        return false;
    }
    else if (category == L"Středověk") 
    {
        if (lowerText.find(" bc") != std::string::npos || lowerText.find("b.c.") != std::string::npos) return false;
        if (lowerText.find("19") != std::string::npos && lowerText.find("19th") == std::string::npos) return false; 
        if (lowerText.find("20") != std::string::npos) return false;
        if (lowerText.find("nuclear") != std::string::npos) return false;

        if (containsAny(lowerText, medievalKeywords)) return true;

        return false;
    }

    return true;
}

std::vector<Question> QuestionManager::fetchQuestions(std::wstring category, std::wstring difficulty)
{
    std::vector<Question> resultQuestions;
    std::set<std::string> seenQuestions;
    CURL* curl = curl_easy_init();

    if (!curl) return resultQuestions;

    std::string baseUrl = "https://opentdb.com/api.php?category=23&type=multiple&amount=50";
    
    std::string difficultyParam = "";
    if (difficulty == L"Lehká") difficultyParam = "&difficulty=easy";
    else if (difficulty == L"Střední") difficultyParam = "&difficulty=medium";
    else difficultyParam = "&difficulty=hard";

    int retryCount = 0;
    const int MAX_RETRIES = 100; 

    while (resultQuestions.size() < 30 && retryCount < MAX_RETRIES)
    {
        std::string readBuffer;
        std::string currentUrl = baseUrl + difficultyParam; 

        // Nastavení CURL
        curl_easy_setopt(curl, CURLOPT_URL, currentUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        std::cout << "Pokus " << (retryCount + 1) << ": Stahuji balik otazek..." << std::endl;
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) 
        {
            try 
            {
                auto jsonData = json::parse(readBuffer);
                if (jsonData["response_code"] == 0) 
                {
                    auto results = jsonData["results"];
                    
                    for (auto& item : results) 
                    {
                        if (resultQuestions.size() >= 30) break;

                        std::string enQuestion = item["question"];

                        if (!isQuestionRelevant(enQuestion, category)) continue;

                        if (seenQuestions.count(enQuestion)>0)
                        {
                            continue;
                        }
                        seenQuestions.insert(enQuestion);

                        Question q;
                        std::string enCorrect = item["correct_answer"];
                        std::vector<std::string> enIncorrect;
                        for(auto& inc : item["incorrect_answers"]) enIncorrect.push_back(inc);

                        std::string csQuestionStr = translateText(curl, enQuestion);
                        std::string csCorrectStr = translateText(curl, enCorrect);
                        
                        q.text = utf8ToWide(csQuestionStr);
                        formatQuestionText(q.text); 
                        
                        std::vector<std::wstring> answers;

                        std::wstring wCorrect = utf8ToWide(csCorrectStr);
                        formatAnswerText(wCorrect); 
                        answers.push_back(wCorrect);

                        for(auto& inc : enIncorrect) 
                        {
                            std::wstring wInc = utf8ToWide(translateText(curl, inc));
                            formatAnswerText(wInc); 
                            answers.push_back(wInc);
                        }

                        std::wstring correctText = answers[0];
                        std::random_device rd;
                        std::mt19937 g(rd());
                        std::shuffle(answers.begin(), answers.end(), g);

                        for(int i=0; i<4; i++) 
                        {
                            if(answers[i] == correctText) 
                            {
                                q.correctIndex = i;
                                break;
                            }
                        }
                        q.answers = answers;
                        resultQuestions.push_back(q);
                        
                        std::cout << "Nacteno otazek: " << resultQuestions.size() << "/30" << std::endl;
                    }
                }
                else 
                {

                }
            } catch (...) 
            {
                std::cerr << "Chyba JSON" << std::endl;
            }
        }
        
        retryCount++;
    }

    if (resultQuestions.empty()) 
    {
        std::cerr << "VAROVANI: Nepodarilo se stahnout zadne otazky!" << std::endl;
    } else 
    {
        std::cout << "Hotovo. Celkem stazeno: " << resultQuestions.size() << " otazek." << std::endl;
    }

    curl_easy_cleanup(curl);
    return resultQuestions;
}