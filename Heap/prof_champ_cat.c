#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<limits.h>
#include<fcntl.h>
#include <sys/uio.h>
#include<linux/fs.h>
#include<sys/ioctl.h>

#define BLOCK_SZ 4096

uid_t ruid, euid, suid;

struct fileptr
{
    int fd;
    char fname[PATH_MAX];
    char *summary;
};

struct fileptr filelist[10];

__attribute__((constructor)) static void null_buffer() 
{
   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0); 
   setuid(0);
   getresuid(&ruid, &euid, &suid);
   printf("\nreal-uid : %d effective-uid : %d saved-uid: %d\n", ruid, euid, suid);
   seteuid(1000);
}

void read_flag(void)
{
    setuid(0);
    perror("");
    int fd = open("/flag", 0);
    perror("open: ");
    char *buffer = (char *) malloc(256);
    memcpy(buffer, "This is the flag and here you go : ", 35);
    read(fd, buffer+35, 256-35);
    printf("%s", buffer);
    seteuid(1000);
}

int get_file_size(int fd) 
{
    struct stat st;

    if(fstat(fd, &st) < 0) 
    {
        perror("fstat");
        exit(-1);
    }
    if (S_ISBLK(st.st_mode)) 
    {
        unsigned long long bytes;
        if (ioctl(fd, BLKGETSIZE, &bytes) != 0) 
        {
            perror("ioctl");
            return -1;
        }
        return bytes;
    } 
    else if (S_ISREG(st.st_mode)) return st.st_size;
    return -1;
}

void output_to_console(char *buf, size_t len) 
{
    write(1, buf, len);
    free(buf);
}

void print_open_list()
{
    for(int idx = 0; idx < 10; idx++)
    {
        printf("fd : %d\nfilename : %s\nSummary : %s", filelist[idx].fd, filelist[idx].fname, filelist[idx].summary);
        printf("\nsummaryPtr: %p\n", filelist[idx].summary);
    }
}

int read_and_print_file(char *file_name, int file_fd) 
{
    struct iovec *iovecs;

    off_t file_sz = get_file_size(file_fd);
    off_t bytes_remaining = file_sz;
    int blocks = (int) file_sz / BLOCK_SZ;
    if (file_sz % BLOCK_SZ) blocks++;
    iovecs = malloc(sizeof(struct iovec) * blocks);

    int current_block = 0;

    while (bytes_remaining) 
    {
        off_t bytes_to_read = bytes_remaining;

        if (bytes_to_read > BLOCK_SZ)
        {
            bytes_to_read = BLOCK_SZ;
        }

        void *buf = malloc(BLOCK_SZ);
        iovecs[current_block].iov_base = buf;
        iovecs[current_block].iov_len = bytes_to_read;
        current_block++;
        bytes_remaining -= bytes_to_read;
    }

    int ret = readv(file_fd, iovecs, blocks);
    if (ret < 0) {
        perror("readv");
        return 1;
    }

    for (int i = 0; i < blocks; i++)
        output_to_console(iovecs[i].iov_base, iovecs[i].iov_len);

    free(iovecs);
    return 0;
}

void welcome()
{

    puts("\n\t=============================================================================================\n"); 
    puts("\tProf Champ is fed up with incompetent maintainers of cat utility. He has been hoping to have");
    puts("\tsomething new and fancy in cat utility but years have passed although Prof Champ is rusty when");
    puts("\tit comes to programming he decided to take matter in his own hands and opt to rewrite fancy cat");
    puts("\tYou have been graded F in OS and System Programming by Prof Champ now its your time to shine");
    puts("\tand show your worth to Prof Champ. Prove your worth or else you are destined to have F.");
    puts("\n\t=============================================================================================\n");
}

void open_new_file()
{
    int idx;
    printf("\nEnter the slot number to add entry");
    scanf("%d", &idx);
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0) 
    {
        perror("open");
        exit(-1);
    }
    filelist[idx].fd = file_fd;
    printf("Enter the file name: ");
    read(0, filelist.fname, PATH_MAX);
    
    
}

void print_menu()
{
    char command;
    printf("champ_cat > ");
    scanf("%c", &command);

    switch(command)
    {
        case 'l':
            print_open_list();
            getchar();
            break;
        case 'f':
            read_flag();
            getchar();
            break;
        default:
            break;

    }
}

int main(int argc, char **argv)
{

    welcome(argc);

    while(1)
    {
        print_menu();
    }

    return 0;
}