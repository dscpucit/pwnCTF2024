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

    char *msg = "Hello from userspace!";
    int ret = write(fd, msg, strlen(msg));
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