#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{

    unsigned long long int P = 0xc55a1957ff2a0;
    unsigned long long int L = 0x55a1957ff4c0;
    
    printf("[+] P = %#llx\n", P);
    printf("[+] L = %#llx\n", data_2);
    

    printf("Calculation with out -0x8 : %#llx\n", ((unsigned long long int)L >> 12) ^ P);

    printf("Calculation with -0x8 : %#llx\n", ((unsigned long long int)L >> 12) ^ (P - 0x8));




    return 0;
}