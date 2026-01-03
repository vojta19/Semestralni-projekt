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

// --- 1. POMOCNÉ FUNKCE (Čištění a formátování) ---

std::string cleanHtmlEntities(std::string text)
{
    // The Trivia API posílá čistší text, ale pro jistotu
    std::string buffer = text;
    // Odstranění HTML tagů
    std::regex tagRegex("<[^>]*>");
    buffer = std::regex_replace(buffer, tagRegex, "");
    return buffer;
}

void formatQuestionText(std::wstring& text)
{
    if (text.empty()) return;
    text[0] = std::towupper(text[0]);
    while (!text.empty() && (text.back() == L'.' || text.back() == L' ')) {
        text.pop_back();
    }
    if (text.empty() || text.back() != L'?') {
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
    return sf::String::fromUtf8(str.begin(), str.end()).toWideString();
}

std::string QuestionManager::translateText(CURL* curl, std::string text)
{
    if (text.empty()) return "";
    std::string cleanText = cleanHtmlEntities(text);
    std::string readBuffer;
    
    // Zakódování textu pro URL
    char* encodedText = curl_easy_escape(curl, cleanText.c_str(), static_cast<int>(cleanText.length()));
    
    // Používáme "clients5" endpoint, který vrací jednoduché JSON pole a nevyžaduje API klíč
    std::string url = "https://clients5.google.com/translate_a/t?client=dict-chrome-ex&sl=en&tl=cs&q=" + std::string(encodedText);
    curl_free(encodedText);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // User-Agent, aby si Google myslel, že jsme prohlížeč (pro jistotu)
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) {
        try {
            // Google vrací odpověď ve formátu: ["Přeložený text"]
            auto jsonResponse = json::parse(readBuffer);
            
            // Kontrola, zda je to pole a má obsah
            if (jsonResponse.is_array() && !jsonResponse.empty()) {
                // Vezmeme první prvek pole
                std::string translated = jsonResponse[0].get<std::string>();
                return cleanHtmlEntities(translated); 
            }
        } catch (...) { 
            // Pokud parsování selže, vrátíme původní anglický text
            return cleanText; 
        }
    }
    return cleanText;
}

// --- 2. HLAVNÍ FUNKCE PRO STAŽENÍ ---

std::vector<Question> QuestionManager::fetchQuestions(std::wstring category, std::wstring difficulty)
{
    std::vector<Question> resultQuestions;
    std::set<std::string> seenQuestions; 
    CURL* curl = curl_easy_init();

    if (!curl) return resultQuestions;

    // A) Nastavení URL pro "The Trivia API"
    std::string baseUrl = "https://the-trivia-api.com/v2/questions?limit=50";
    
    // B) Mapování kategorií na TAGY (tohle je ta magie)
    std::string tags = "";
    if (category == L"Starověk") {
        // Hledáme Řím, Řecko, starověk...
        tags = "&tags=ancient_rome,ancient_greece,roman_empire,bronze_age,iron_age"; 
    } 
    else if (category == L"Středověk") {
        // Hledáme středověk, křížové výpravy...
        tags = "&tags=middle_ages,medieval,crusades,vikings,hundred_years_war";
    }
    else if (category == L"Moderní dějiny") {
        // Hledáme světové války, studenou válku, 20. století
        tags = "&tags=world_war_1,world_war_2,cold_war,soviet_union,american_civil_war,industrial_revolution";
    }
    else {
        // Fallback - prostě historie
        tags = "&categories=history";
    }

    // C) Obtížnost
    std::string diffParam = "";
    if (difficulty == L"Lehká") diffParam = "&difficulty=easy";
    else if (difficulty == L"Střední") diffParam = "&difficulty=medium";
    else diffParam = "&difficulty=hard";

    // Sestavení URL
    std::string currentUrl = baseUrl + tags + diffParam;

    // --- Smyčka pro získání dostatku otázek ---
    int retryCount = 0;
    const int MAX_RETRIES = 15; // Tady stačí méně pokusů, protože API vrací přesnější data

    while (resultQuestions.size() < 30 && retryCount < MAX_RETRIES)
    {
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, currentUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        std::cout << "Pokus " << (retryCount + 1) << ": Stahuji z The Trivia API..." << std::endl;
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) 
        {
            try 
            {
                // Pozor: The Trivia API vrací rovnou pole [], nemá to obalené v "results"
                auto jsonArray = json::parse(readBuffer);
                
                if (jsonArray.is_array()) 
                {
                    for (auto& item : jsonArray) 
                    {
                        if (resultQuestions.size() >= 30) break;

                        // 1. Získání textu otázky 
                        // (Struktura: item["question"]["text"])
                        std::string enQuestion = item["question"]["text"];

                        // 2. Duplicity
                        if (seenQuestions.count(enQuestion) > 0) continue;
                        seenQuestions.insert(enQuestion);

                        Question q;
                        std::string enCorrect = item["correctAnswer"];
                        std::vector<std::string> enIncorrect;
                        for(auto& inc : item["incorrectAnswers"]) enIncorrect.push_back(inc);

                        // 3. Překlad
                        std::string csQuestionStr = translateText(curl, enQuestion);
                        std::string csCorrectStr = translateText(curl, enCorrect);
                        
                        q.text = utf8ToWide(csQuestionStr);
                        formatQuestionText(q.text); 
                        
                        std::vector<std::wstring> answers;
                        std::wstring wCorrect = utf8ToWide(csCorrectStr);
                        formatAnswerText(wCorrect);
                        answers.push_back(wCorrect);

                        for(auto& inc : enIncorrect) {
                            std::wstring wInc = utf8ToWide(translateText(curl, inc));
                            formatAnswerText(wInc);
                            answers.push_back(wInc);
                        }

                        // Míchání
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
        // Zde by mohl být fallback na OpenTriviaDB, ale pro jednoduchost necháme prázdné
    } 

    curl_easy_cleanup(curl);
    return resultQuestions;
}