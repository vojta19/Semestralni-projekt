#include <iostream>


long generator (long previous, int factor)
{
    return (previous*factor)%2147483647;
}

int get_last_n_bits(long value, int n)
{
    return value & ((1L << n) - 1);
}


bool compare_last_bits(long value_a, long value_b)
{
    return get_last_n_bits(value_a, 16) == get_last_n_bits(value_b, 16);
}

int generation(long a_start, long b_start, int steps)
{
    long a_result=a_start;
    long b_result=b_start;
    int pocet_shodnych=0;

    for(int i=0;i<steps;i++)
    {
        a_result=generator (a_result, 16807);
        b_result=generator(b_result,48271);
        if(compare_last_bits(a_result,b_result))
        {
            pocet_shodnych++;
           // std::cout << "Match found! Generator A: " << a_result << " Generator B: " << b_result << std::endl;
        }
    }
    std::cout << "\nCelkem nalezeno " << pocet_shodnych<<std::endl;
    return pocet_shodnych;
}



int main()
{
    long a_start = 289;
    long b_start = 629;
    int steps = 40000000;  // 40 milionů kroků pro plný test
    //std::cout << "Začínám generovat " << steps << " párů čísel...\n" << std::endl;
    int result = generation(a_start, b_start, steps);
    return 0;
}