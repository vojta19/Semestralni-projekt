#include "QuestionManager.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <random>

using json = nlohmann::json;

size_t QuestionManager::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::wstring QuestionManager::utf8ToWide(const std::string& str)
{
    return sf::String::fromUtf8(str.begin(), str.end()).toWideString();
}

// --- FUNKCE PRO PŘEKLAD (EN -> CS) ---
std::string QuestionManager::translateText(CURL* curl, std::string text)
{
    if (text.empty()) return "";
    std::string readBuffer;

    char* encodedText = curl_easy_escape(curl, text.c_str(),static_cast<int>(text.length()));
    // Používáme MyMemory API
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

// --- NOVÁ FUNKCE: ROZHODOVÁNÍ O KATEGORII ---
// Zjistí, zda anglický text otázky odpovídá vybranému období
bool isQuestionRelevant(const std::string& text, const std::wstring& category)
{
    std::string lowerText = text;
    // Převedeme na malá písmena pro snazší hledání
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);

    // 1. STAROVĚK (Ancient History)
    if (category == L"Starověk") 
    {
        // Hledáme klíčová slova: BC (před kristem), Ancient, Rome, Greek, Egypt...
        if (lowerText.find(" bc") != std::string::npos) return true;
        if (lowerText.find("b.c.") != std::string::npos) return true;
        if (lowerText.find("ancient") != std::string::npos) return true;
        if (lowerText.find("rome") != std::string::npos) return true;
        if (lowerText.find("roman") != std::string::npos) return true;
        if (lowerText.find("greek") != std::string::npos) return true;
        if (lowerText.find("greece") != std::string::npos) return true;
        if (lowerText.find("egypt") != std::string::npos) return true;
        if (lowerText.find("pharaoh") != std::string::npos) return true;
        if (lowerText.find("caesar") != std::string::npos) return true;
        if (lowerText.find("alexander") != std::string::npos) return true;
        return false;
    }
    // 2. MODERNÍ DĚJINY (Modern History)
    else if (category == L"Moderní dějiny") 
    {
        // Hledáme roky 19xx, 20xx, World War, Nazi, Soviet, President...
        if (lowerText.find("19") != std::string::npos) return true; // Roky 1900-1999
        if (lowerText.find("20") != std::string::npos) return true; // Roky 2000+
        if (lowerText.find("war") != std::string::npos) return true;
        if (lowerText.find("soviet") != std::string::npos) return true;
        if (lowerText.find("nazi") != std::string::npos) return true;
        if (lowerText.find("president") != std::string::npos) return true;
        if (lowerText.find("nuclear") != std::string::npos) return true;
        return false;
    }
    // 3. STŘEDOVĚK (Middle Ages)
    else if (category == L"Středověk") 
    {
        // Středověk je všechno mezi tím (zhruba). Hledáme King, Empire, Castle...
        // Tady je to těžší, takže to vezmeme vylučovací metodou:
        // Pokud to není BC (Starověk) a není to 19xx (Moderní), tak to zkusíme.
        if (lowerText.find(" bc") != std::string::npos) return false;
        if (lowerText.find("19") != std::string::npos) return false;
        if (lowerText.find("20") != std::string::npos) return false;
        
        // Pozitivní slova
        if (lowerText.find("king") != std::string::npos) return true;
        if (lowerText.find("queen") != std::string::npos) return true;
        if (lowerText.find("empire") != std::string::npos) return true;
        if (lowerText.find("century") != std::string::npos) return true;
        if (lowerText.find("middle ages") != std::string::npos) return true;
        return false; 
    }

    // Pokud kategorie není specifikovaná, bereme všechno
    return true; 
}

std::vector<Question> QuestionManager::fetchQuestions(std::wstring category, std::wstring difficulty)
{
    std::vector<Question> resultQuestions;
    CURL* curl = curl_easy_init();

    if (!curl) return resultQuestions;

    std::string readBuffer;
    
    // --- ZMĚNA: Stahujeme VŽDY Historii (ID 23) ---
    // amount=15 : Stáhneme víc otázek (15), abychom měli z čeho filtrovat
    std::string apiUrl = "https://opentdb.com/api.php?amount=200&category=23&type=multiple";

    // Obtížnost
    if (difficulty == L"Lehká") apiUrl += "&difficulty=easy";
    else if (difficulty == L"Střední") apiUrl += "&difficulty=medium";
    else apiUrl += "&difficulty=hard";

    // Stažení anglických otázek
    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) 
    {
        try 
        {
            auto jsonData = json::parse(readBuffer);
            
            if (jsonData["response_code"] == 0) 
            {
                auto results = jsonData["results"];
                int count = 0;

                for (auto& item : results) 
                {
                    // POKUD MÁME UŽ 5 OTÁZEK, KONČÍME (aby to netrvalo dlouho)
                    if (count >= 30) break;

                    std::string enQuestion = item["question"];

                    // --- FILTROVÁNÍ: Patří otázka do zvolené doby? ---
                    // Pokud ne, přeskočíme ji a jdeme na další
                    if (!isQuestionRelevant(enQuestion, category)) 
                    {
                        continue; 
                    }

                    // Pokud prošla filtrem, přeložíme ji
                    Question q;
                    std::string enCorrect = item["correct_answer"];
                    std::vector<std::string> enIncorrect;
                    for(auto& inc : item["incorrect_answers"]) enIncorrect.push_back(inc);

                    // Překlad
                    std::string csQuestionStr = translateText(curl, enQuestion);
                    std::string csCorrectStr = translateText(curl, enCorrect);
                    
                    q.text = utf8ToWide(csQuestionStr);
                    
                    std::vector<std::wstring> answers;
                    answers.push_back(utf8ToWide(csCorrectStr));

                    for(auto& inc : enIncorrect) 
                    {
                        answers.push_back(utf8ToWide(translateText(curl, inc)));
                    }

                    // Míchání
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
                    count++;
                    
                    sf::sleep(sf::milliseconds(50));
                }
            }
        } catch (...) {
            std::cerr << "Chyba pri zpracovani JSON" << std::endl;
        }
    }

    // ZÁCHRANA: Pokud jsme po filtraci nenašli ŽÁDNOU otázku (filtr byl moc přísný),
    // stáhneme pro jistotu cokoliv z historie, aby hra nespadla.
    if (resultQuestions.empty()) {
        std::cerr << "Filtr nenasel otazky pro tuto kategorii, pouzivam obecnou historii." << std::endl;
        // Tady by se rekurzivně zavolalo fetchQuestions s jinou kategorií, 
        // ale pro jednoduchost to necháme prázdné a GameplayScreen použije offline zálohu.
    }

    curl_easy_cleanup(curl);
    return resultQuestions;
}