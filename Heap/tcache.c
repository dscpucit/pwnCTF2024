#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void print_chunk(char *p)
{
    int loc = 0;
    for(int idx = 0; idx < 3; idx++)
    {
        printf("")
    }
}
int main(void)
{
    char *data = malloc(32);
    char *data_1 = malloc(32);
    memset(data, 'A', 32);
    memset(data_1, 'B', 32);
    printf("[+] data = %p\n", data);
    printf("[+] data_1 = %p\n", data_1);

    free(data);
    free(data_1);

    malloc(32);
    malloc(32);
    return 0;
}