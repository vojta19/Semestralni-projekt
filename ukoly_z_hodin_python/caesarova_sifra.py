from typing import List

def sifruj(text: str, posun: int) -> str:
    vysledek: List[str] = []    
    for znak in text:
        if znak.isalpha():
            # Zjistit, zda je velké nebo malé písmeno
            if znak.isupper():
                # Pro velká písmena (A-Z)
                base = ord('A')
                posunute = (ord(znak) - base + posun) % 26
                vysledek.append(chr(base + posunute))
            else:
                # Pro malá písmena (a-z)
                base = ord('a')
                posunute = (ord(znak) - base + posun) % 26
                vysledek.append(chr(base + posunute))
        else:
            # Znaky, které nejsou písmena, zůstanou beze změny
            vysledek.append(znak)
    
    return ''.join(vysledek)


if __name__ == "__main__":
    test =input("Zadejte text k zašifrování: ")
    posun = int(input("Zadejte posun (celé číslo): "))
    result = sifruj(test, posun)
    print(f"sifruj('{test}', {posun}) -> '{result}'")
