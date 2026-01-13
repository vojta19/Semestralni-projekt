# Zpět do minulosti (Kvízová hra)

Obdobná implementace oblíbené televizní soutěže Chcete být milionářem s otázkami z historie.

## 📋 Přehled

Toto je kvízová hra v C++. Hra běží s grafickým rozhraním postaveným na knihovně SFML, která umožňuje tvorbu pokročilých textových uživatelských rozhraní.

## ✨ Popis

- Uživatelské rozhraní - vše přehledně na úvodním okně hry s možností výběru akce pomocí tlačítek
- Grafické zpracování - hra bude postupně doplněna grafickými prvky a podtextovou hudbou
- Okruhy otázek - otázky budou až ze 3 kategorií: Středověk, Novověk a Moderní doba
- Herní módy - hráč si podle svého rozvážení vybere z několika herních módů
- Časový limit - na základě zvolené obtížnosti a herního módu

## Obsažené kategorie

- Podmínky, cykly, vstup a výstup
- Načítání ze internetu, vektory, náhodnost, měření času
- Uživatelské rozhraní, grafické rozhraní
- Filtrování dat, zpracování výsledků
- Detekce chyb a kontrola vstupu

## 🧠 Herní mechaniky

-Kvízový systém: Hráč vybírá jednu správnou odpověď ze čtyř možností
-Obtížnost: Tři úrovně obtížnosti (Lehká, Střední, Těžká), které ovlivňují časový limit na odpověď (30s, 15s, 6s)
-Systém skóre a hodnocení: Počítání správných odpovědí, výpočet úspěšnosti v procentech a udělování hodností na konci hry (např. Pán času, Časový turista)
-Dynamický časovač: Vizuální odpočet času s efekty při vypršení (blikání, pulzování)

## 📜 Herní režimy a kategorie

-Tematické okruhy: Otázky rozdělené do historických ér: Starověk, Středověk, Moderní dějiny
-Unikátní režim CHAOS: Speciální herní mód, který náhodně aktivuje stížené podmínky
-Zpřeházené odpovědi: Tlačítka s odpověďmi se v reálném čase hýbou a mění pozice
-Cenzura (Mlha): Odpovědi jsou skryté a odhalí se až po najetí myší
-Zkrácený časový limit: Rychlejší hra pro větší napětí

## 🎨 Uživatelské rozhraní (UI) a grafika

-Adaptivní pozadí: Pozadí hry se mění podle zvolené historické kategorie
-Interaktivní menu: Tlačítka s hover efekty (změna barvy po najetí myší) a vizuální zpětná vazba při kliknutí (zelená pro správnou, červená pro špatnou odpověď)
-Responzivní text: Automatické zalamování dlouhých otázek, aby se vešly na obrazovku
-Podpora češtiny: Plná podpora diakritiky (UTF-8) díky vlastní implementaci načítání textů

## ⚙️ Technické zpracování

-C++ a SFML: Hra je postavena na objektově orientovaném návrhu v jazyce C++ s využitím knihovny SFML pro grafiku, audio a práci s okny
-Asynchronní načítání: Využití multithreadingu (std::async, std::future) pro načítání otázek na pozadí, aby se hra nezasekávala
-JSON Parsing: Otázky jsou načítány a parsovány z externího formátu JSON (využití knihovny nlohmann/json)
-Robustní správa souborů: Hra obsahuje mechanismy pro bezpečné načítání assetů (obrázky, fonty, zvuky) s ošetřením chybějících souborů

## 🕹️ Ovládání

-Myš: Ovládání celého menu a výběr odpovědí
-ESC: Návrat do menu / Ukončení hry

## 📥 Instalace a zkompilování zdrojového kódu

-Pro sestavení projektu ze zdrojových kódů budete potřebovat:
  -IDE: Visual Studio 2022 (s nainstalovanou sadou pro vývoj v C++).
  -Knihovna: SFML 2.6.1 (Visual C++ 17 (2022) - 64-bit).
  -JSON: Knihovna nlohmann/json (je již součástí projektu jako json.hpp).

-Postup sestavení:
  -Otevřete soubor řešení ZpetDoMinulosti.sln ve Visual Studiu.

-Nastavení cest k SFML:
  -V nastavení projektu (Vlastnosti -> C/C++ -> Obecné) zkontrolujte cestu k hlavičkovým souborům (include).
  -V nastavení linkeru (Vlastnosti -> Linker -> Obecné) zkontrolujte cestu k knihovnám (lib).
  -Projekt je ve výchozím stavu nastaven na cestu C:\SFML-2.6.1. Pokud máte SFML jinde, cesty upravte.

-Nastavení kódování:
  -Ujistěte se, že v Příkazovém řádku kompilátoru je přidán přepínač /utf-8 pro správné zobrazování češtiny.

-Kopírování Assets:
  -Pro spouštění přímo z Visual Studia (klávesa F5) se ujistěte, že složka assets je umístěna v kořenovém adresáři projektu (tam, kde jsou soubory .cpp).

Zvolte konfiguraci Release nebo Debug (x64) a dejte Sestavit řešení (Build).
