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

char *curse_buffer_data = NULL;
unsigned int curse_bytes = 0;

int flag = 0;

uid_t ruid, euid, suid;

struct fileptr
{
    int fd;
    char fname[PATH_MAX + 1];
    char *summary;
};

struct fileptr filelist[10];

__attribute__((constructor)) static void null_buffer() 
{
   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0); 
   setuid(0);
   getresuid(&ruid, &euid, &suid);//just to play with it
   seteuid(1000);
}

void read_flag(void)
{
    
    if(flag > 0)
    {
        printf("\n\tNah Nah Prof champ knows what you are doing...\n\n");
        return;
    } 
    setuid(0);
    flag++;
    int fd = open("/flag", O_RDONLY);
    perror("opening the flag: ");    
    char *buffer = (char *) malloc(256);
    printf("\n[+] Address of the flag buffer is %p\n", buffer);
    memcpy(buffer, "This is the flag and here you go : ", 35);
    read(fd, buffer+35, 256-35);
    // printf("%s", buffer);
    seteuid(1000);
    close(fd);
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
    printf("\n[+] List of the open files\n");
    for(int idx = 0; idx < 10; idx++)
    {
        printf("\n[+] fileptr # %u\n", idx);
        printf("\tfd : %u\n\tfilename : %s\n\tSummary : %s", filelist[idx].fd, filelist[idx].fname, filelist[idx].summary);
        printf("\n\tsummaryPtr: %p\n", filelist[idx].summary);
    }
}

int read_and_print_file(int file_fd) 
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
    unsigned int idx, bytes;;
    printf("\nEnter the slot number to add entry : ");
    scanf("%u", &idx);
    printf("Enter the file name: ");
    int rv = read(0, filelist[idx % 10].fname, PATH_MAX);
    filelist[idx % 10].fname[rv - 1] = '\0';
    printf("\n\t[+] Opening the file : %s\n", filelist[idx % 10].fname);
    int file_fd = open(filelist[idx % 10].fname, O_RDONLY);
    if (file_fd < 0) 
    {
        perror("open");
        exit(-1);
    }
    filelist[idx % 10].fd = file_fd;
    printf("Enter Number of bytes for the summary : ");
    scanf("%u", &bytes);
    filelist[idx % 10].summary = (char *)malloc(bytes);
    read(file_fd, filelist[idx].summary, bytes);   
    lseek(file_fd, 0, SEEK_SET);
}

void close_file(void)
{
    unsigned int idx;
    printf("\nEnter the file slot number : ");
    scanf("%u", &idx);
    idx = idx % 10;
    close(filelist[idx].fd);
    memset(filelist[idx].fname, '\0', PATH_MAX);
    free(filelist[idx].summary);
    filelist[idx].summary = NULL;

}

void file_printer(void)
{
    unsigned int idx;
    printf("\nEnter the file slot number : ");
    scanf("%u", &idx);
    idx = idx % 10;
    read_and_print_file(filelist[idx].fd);

}

void update_curse_buffer(void)
{
    printf("\nEnter the Number of bytes : ");
    scanf("%u", &curse_bytes);
    printf("Enter new curse :");
    read(0, curse_buffer_data, curse_bytes);
}

void curse_buffer(void)
{
    if(curse_buffer_data != NULL)
    {
        printf("\n\tNah Nah Prof champ knows what you are doing...\n\n");
        printf("[+] Don't allocate a lot of memory it's useless you really deserver F\n");
        return;
    }
    printf("\n[+] This is the curse buffer you can add your curses for the prof champ\n");
    printf("\nEnter the curse buffer size : ");
    scanf("%u", &curse_bytes);
    curse_buffer_data = (char *)malloc(curse_bytes);
    printf("\n[+] Address of the curse buffer : %p\n", curse_buffer_data);
    printf("Write Down your curses for prof champ : ");
    read(0, curse_buffer_data, curse_bytes);
}

void lift_curse()
{
    printf("\n\tHAHAHAHA Surrendering to the Prof Champ ? I am Champ accept the defeat.\n");
    free(curse_buffer_data);
}

void print_menu()
{
    char command;
    printf("\nchamp_cat > ");
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
        case 'n':
            open_new_file();
            getchar();
            break;
        case 'p':
            file_printer();
            getchar();
            break;
        case 'c':
            close_file();
            getchar();
            break;
        case 'm':
            curse_buffer();
            getchar();
            break;
        case 'u':
            update_curse_buffer();
            getchar();
            break;
        case 'd':
            lift_curse();
            getchar();
            break;
        case 'q':
            exit(0);
            break;
        default:
            printf("\n[+] Prof Champ hates it when people make mistakes.\n");
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