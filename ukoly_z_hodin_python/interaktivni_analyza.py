from typing import List
seznam: List[int] = []
while True:
    try:
        vstup=input("Zadejte číslo nebo 'stop' pro ukončení: ")
        if vstup.lower() == 'stop':
            break
        cislo=int(vstup)
        seznam.append(cislo)
    except ValueError:
        print("Neplatný vstup, zkuste to znovu.")

def vypis_statistiku(seznam: List[int]) -> None:
    if not seznam:
        print("Žádná čísla nebyla zadána.")
        return
    pocet: int = len(seznam)
    soucet: int = sum(seznam)
    prumer: float = soucet / pocet
    minimum: int = min(seznam)
    maximum: int = max(seznam)
    print(f"Počet: {pocet}")
    print(f"Součet: {soucet}")
    print(f"Průměr: {prumer}")
    print(f"Nejmenší: {minimum}")
    print(f"Největší: {maximum}")

vypis_statistiku(seznam)