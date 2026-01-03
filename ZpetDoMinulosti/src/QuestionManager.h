#pragma once
#include <vector>
#include <string>
#include <SFML/System.hpp>
#include "GamePlayScreen.h" // Abychom znali strukturu Question
#include <curl/curl.h>

class QuestionManager
{
public:
    // Hlavní funkce: Stáhne a přeloží otázky podle tvé kategorie a obtížnosti
    static std::vector<Question> fetchQuestions(std::wstring category, std::wstring difficulty);

private:
    // Pomocné funkce pro vnitřní logiku
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::wstring utf8ToWide(const std::string& str);
    static std::string translateText(CURL* curl, std::string text);
};