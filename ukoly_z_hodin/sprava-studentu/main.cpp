#include <iostream>
#include <vector>
#include <string>
#include <new>
#include <fstream>

struct Student
{
    int ID;
    std::string cele_jmeno;
    double prumer;
};

struct Node
{
    Student data;
    Node* next;
};

int parsujRadek(const std::string&radek, Student&student)
{
    size_t pos1 = radek.find(',');                  // najdi první čárku
    if (pos1 == std::string::npos) return -1;      // pokud není, vrať chybu
    
    size_t pos2 = radek.find(',', pos1 + 1);       // najdi druhou čárku
    if (pos2 == std::string::npos) return -1;      // pokud není, vrať chybu

    try         // pokus se převést hodnoty
    {
        student.ID = std::stoi(radek.substr(0, pos1));             // získej věk
        student.cele_jmeno = radek.substr(pos1 + 1, pos2 - pos1 - 1);       // získej celé jméno
        student.prumer = std::stod(radek.substr(pos2 + 1));             // získej průměr
    }
    catch (...)                 // zachyť všechny výjimky            
    {
        return -1; // pokud dojde k chybě při převodu, vrať chybu
    }

    return 0;   
}

int pridejStudentaNaKonec(Node** pHead, const Student& student)
{
    if (pHead == nullptr) return -1; //kontrola platnosti ukazatele

    Node* novy = new (std::nothrow) Node;  //alokuje novy uzel
    if (novy == nullptr) return -1; // alokace selhala

    novy->data = student; //kopiruje data studenta
    novy->next = nullptr;

    if (*pHead == nullptr) //pokud je seznam prazdny pridej na zacatek
    {
        *pHead = novy;
        return 0;
    }

    Node* current = *pHead;     // jinak projdi na konec a pripoj
    while (current->next != nullptr)
        current = current->next;

    current->next = novy;
    return 0;
}

int nactiStudentyZeSouboru(const std::string& nazev_souboru, Node** pHead)
{
    std::ifstream soubor(nazev_souboru);
    if (!soubor.is_open()) return -1;           //kontrola otevření souboru    
    std::string radek;
    int uspesne_nacteno = 0;    // počítadlo úspěšně načtených studentů
    
    while (std::getline(soubor, radek))
    {
        Student novy_student;                       // vytvoření objektu studenta
        if (parsujRadek(radek, novy_student) == 0)  // pokud se řádek podařilo naparsovat
        {
            if (pridejStudentaNaKonec(pHead, novy_student) == 0)  // pokud se podařilo přidat do seznamu
            {
                uspesne_nacteno++;
            }
        }
    }
    
    soubor.close();
    return uspesne_nacteno;  // vrátí počet úspěšně načtených studentů
}

int vypisSeznam(Node* head)
{
    Node* current = head;
    while(current != nullptr)
    {
        std::cout << "Jméno: " << current->data.cele_jmeno 
                  << ", ID: " << current->data.ID 
                  << ", Průměr: " << current->data.prumer << std::endl;
        current = current->next;
    }
    return 0;
}

int setridSeznamPodlePrumeru(Node* head)
{
    if (head==nullptr) return -1; //kontrola prazdneho seznamu
    for(Node* i=head; i!=nullptr; i=i->next)
    {
        for(Node* j=i->next; j!=nullptr; j=j->next)
        {
            if(i->data.prumer > j->data.prumer)
            {
                Student temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
    return 0; // úspěšné seřazení
}

int ulozStudentyDoSouboru(const std::string& studenti_serazeno, Node* head)
{
    std::ofstream file(studenti_serazeno);
    if(!file.is_open()) return -1;
    
    Node* current = head;
    while(current != nullptr)
    {
        file << current->data.ID << ","
             << current->data.cele_jmeno << ","
             << current->data.prumer << std::endl;
        current = current->next;
    }
    
    file.close();
    return 0; // úspěšné uložení
}

int zrusSeznam(Node** pHead)
{
    if (pHead == nullptr) return -1;  // kontrola platnosti ukazatele
    
    Node* current = *pHead;
    while(current != nullptr)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    
    *pHead = nullptr;  // nastav head na nullptr po uvolnění
    return 0;  // úspěšné uvolnění
}

int main()
{
    // Deklarace ukazatele na hlavu seznamu
    Node* head = nullptr;
    
    // Definice názvů souborů
    const std::string vstupniSoubor = "studenti.txt";
    const std::string vystupniSoubor = "studenti_serazeno.txt";
    
    // Načtení studentů ze souboru
    std::cout << "Načítám studenty ze souboru " << vstupniSoubor << "...\n";
    int nacteno = nactiStudentyZeSouboru(vstupniSoubor, &head);
    if (nacteno < 0) 
    {
        std::cout << "Chyba při načítání souboru!\n";
        return 1;
    }
    std::cout << "Úspěšně načteno " << nacteno << " studentů.\n\n";
    
    // Výpis původního seznamu
    std::cout << "Původní seznam studentů:\n";
    vypisSeznam(head);
    std::cout << "\n";
    
    // Přidání nového záznamu
    Student novyStudent = {111, "Jaroslav Vesely", 2.5};
    if (pridejStudentaNaKonec(&head, novyStudent) == 0) 
    {
        std::cout << "Přidán nový student.\n\n";
    }

    // Výpis seznamu po přidání nového záznamu
    std::cout << "Seznam studentů po přidání nového záznamu:\n";
    vypisSeznam(head);
    std::cout << "\n";
    
    // Seřazení seznamu
    std::cout << "Řadím seznam podle průměru...\n";
    setridSeznamPodlePrumeru(head);
    
    // Výpis seřazeného seznamu
    std::cout << "Seřazený seznam studentů:\n";
    vypisSeznam(head);
    std::cout << "\n";
    
    // Uložení do výstupního souboru
    std::cout << "Ukládám seřazený seznam do souboru " << vystupniSoubor << "...\n";
    if (ulozStudentyDoSouboru(vystupniSoubor, head) == 0) 
    {
        std::cout << "Seznam byl úspěšně uložen.\n";
    } 
    else 
    {
        std::cout << "Chyba při ukládání seznamu!\n";
    }
    
    // Uvolnění paměti
    std::cout << "\nUvolňuji paměť...\n";
    zrusSeznam(&head);
    std::cout << "Hotovo.\n";
    
    return 0;
}