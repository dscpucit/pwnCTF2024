#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>


__attribute__((constructor)) static void null_buffer() 
{
   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0); 
}


void welcome(void)
{
    printf(
        "Prof Champ just is fed up with incompetent maintainers of cat utility. He has been hoping to have\n
        to have something new and fancy in cat utility but years have passed although Prof Champ is rusty when\n
        it comes to programming he decided to take matter in his own hands and opt to rewrite cat in his own way\n  
    ")
}

void print_menu(void)
{
    printf("")
}

int main(int argc, char **argv)
{
    return 0;
}