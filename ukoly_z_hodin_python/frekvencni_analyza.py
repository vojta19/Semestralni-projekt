from typing import Dict

def spocitej_znaky(text: str) -> Dict[str, int]:
    frekvence: Dict[str, int] = {}
    text_lower = text.lower()
    for znak in text_lower:
        if znak == ' ':
            continue
        frekvence[znak] = frekvence.get(znak, 0) + 1
    
    return frekvence

if __name__ == "__main__":
    text = input("Zadejte větu pro analýzu znaků: ")
    result = spocitej_znaky(text)
    print(f"\nText: '{text}'")
    print(f"Frekvence: {result}\n")


