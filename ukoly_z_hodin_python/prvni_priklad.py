vyska:int = int(input("zadej vysku stromu: "))
kmen_vyska:int = int(input("zadej vysku kmene: "))
kmen_sirka:int = int(input("zadej sirku kmene: "))

for i in range(vyska):
    print(" "*(vyska-i-1)+"*"*(2*i+1))


for i in range(kmen_vyska):
    print(" "*(vyska-((kmen_sirka)//2)-1)+"*"*(kmen_sirka))
    


