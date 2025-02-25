#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>

void get_password(char *password, size_t max_len) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fgets(password, max_len, stdin);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    password[strcspn(password, "\n")] = 0;
}

int copy_file(const char *src, const char *dest) {
    FILE *source = fopen(src, "rb");
    if (!source) {
        perror("Source file error");
        return 0;
    }
    FILE *destination = fopen(dest, "wb");
    if (!destination) {
        perror("Destination file error");
        fclose(source);
        return 0;
    }
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }
    fclose(source);
    fclose(destination);
    return 1;
}

int execute_steghide_embed(const char *output, const char *secret, const char *password) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"steghide", "embed", "-cf", output, "-ef", secret, "-p", password, "-f", NULL};
        execvp("steghide", args);
        perror("execvp failed");
        exit(1);
    } else if (pid < 0) {
        perror("fork failed");
        return -1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void embed_file() {
    char cover[256], secret[256], output[256], password[256];
    
    printf("Enter cover file: ");
    fgets(cover, sizeof(cover), stdin);
    cover[strcspn(cover, "\n")] = 0;
    
    printf("Enter secret file: ");
    fgets(secret, sizeof(secret), stdin);
    secret[strcspn(secret, "\n")] = 0;
    
    printf("Enter output file: ");
    fgets(output, sizeof(output), stdin);
    output[strcspn(output, "\n")] = 0;
    
    printf("Enter password: ");
    get_password(password, sizeof(password));
    
    if (!copy_file(cover, output)) {
        printf("File copy failed.\n");
        return;
    }
    
    int result = execute_steghide_embed(output, secret, password);
    printf(result == 0 ? "Embedded successfully into %s\n" : "Embedding failed.\n", output);
}

int execute_steghide_extract(const char *embedded, const char *password, const char *output) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"steghide", "extract", "-sf", embedded, "-xf", output, "-p", password, "-f", NULL};
        execvp("steghide", args);
        perror("execvp failed");
        exit(1);
    } else if (pid < 0) {
        perror("fork failed");
        return -1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

void extract_file() {
    char embedded[256], output[256], password[256];
    
    printf("Enter embedded file: ");
    fgets(embedded, sizeof(embedded), stdin);
    embedded[strcspn(embedded, "\n")] = 0;
    
    printf("Enter output file: ");
    fgets(output, sizeof(output), stdin);
    output[strcspn(output, "\n")] = 0;
    
    printf("Enter password: ");
    get_password(password, sizeof(password));
    
    int result = execute_steghide_extract(embedded, password, output);
    printf(result == 0 ? "Extracted to %s\n" : "Extraction failed.\n", output);
}

int main() {
    int choice;
    do {
        printf("\n1. Embed\n2. Extract\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: embed_file(); break;
            case 2: extract_file(); break;
            case 3: puts("Exiting."); break;
            default: puts("Invalid choice.");
        }
    } while (choice != 3);
    
    return 0;
}
