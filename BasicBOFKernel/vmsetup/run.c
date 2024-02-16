#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

__attribute__((constructor))
void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

void downloadFile(const char* link) {
    // Construct the command to download the file
    char command[256];
    snprintf(command, sizeof(command), "wget %s -O exploit", link);

    // Execute the command
    int result = system(command);
    if (result != 0) {
        printf("Failed to download file from %s\n", link);
        exit(1);
    }
}

int main() {
    // Get the link from the user
    char link[256];
    printf("Enter the link to download the file: ");
    fgets(link, sizeof(link), stdin);
    link[strcspn(link, "\n")] = '\0'; // Remove the trailing newline character

    // Download the file
    downloadFile(link);

    // Execute the run.sh script in the current working directory
    char* script = "./run.sh";
    char* args[] = {script, NULL};
    execvp(script, args);

    // If execvp returns, it means the execution failed
    printf("Failed to execute the run.sh script\n");
    return 1;
}
