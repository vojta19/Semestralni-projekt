import random
hadane_cislo=random.randrange(1,9)
while True:
    pokus=int(input("Zadej cislo od 1 do 9: "))
    if pokus<hadane_cislo:
        print("Hledane cislo je vetsi.")
    elif pokus>hadane_cislo:
        print("Hledane cislo je mensi.")
    else:
        print("Uhadl jsi!")
        break