import os

def filtrace_logu(vstupni_soubor: str, vystupni_soubor: str, klicove_slovo: str = "ERROR") -> None:
    try:
        with open(vstupni_soubor, 'r', encoding='utf-8') as f:
            radky = f.readlines()
    except FileNotFoundError:
        print(f"Chyba: Soubor '{vstupni_soubor}' nebyl nalezen.")
        return
    except Exception as e:
        print(f"Chyba při čtení souboru: {e}")
        return
    
    filtrovane_radky = [radek for radek in radky if klicove_slovo in radek]
    
    try:
        with open(vystupni_soubor, 'w', encoding='utf-8') as f:
            f.writelines(filtrovane_radky)
        print(f"✓ Filtrace dokončena: {len(filtrovane_radky)} řádků s '{klicove_slovo}' zapsáno do '{vystupni_soubor}'")
    except Exception as e:
        print(f"Chyba při zápisu do souboru: {e}")


if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    vstupni = os.path.join(script_dir, "system.log")
    vystupni = os.path.join(script_dir, "errors_only.txt")
    
    filtrace_logu(vstupni, vystupni, "ERROR")
