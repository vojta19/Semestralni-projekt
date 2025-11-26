from os.path import join,realpath,dirname
vyska:int = int(input("zadej vysku stromu: "))
kmen_vyska:int = int(input("zadej vysku kmene: "))
kmen_sirka:int = int(input("zadej sirku kmene: "))

for i in range(vyska):
    print(" "*(vyska-i-1)+"*"*(2*i+1))


for i in range(kmen_vyska):
    print(" "*(vyska-((kmen_sirka)//2)-1)+"*"*(kmen_sirka))


with open("ukoly_z_hodin_c++/sprava-studentu/studenti_serazeno.txt","r",encoding="utf-8") as f:
    print(f.read())

cesta=join(dirname(realpath(__file__)),"ukoly_z_hodin_c++/sprava-studentu/studenti_serazeno.txt")
with open(cesta,"r",encoding="utf-8") as f:
    for c, line in enumerate(f):
        for znak in line:
            if znak.isdigit():print(znak,end=", ")
            print()