#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"

void inspect_file(const char* filepath) {
    struct stat file_stat;

    // 1. Call stat(filepath, &file_stat).
    // 2. Check if stat() returns -1 (meaning the file doesn't exist or an error occurred).
    // 3. If successful, print the file size using file_stat.st_size.
    // 4. Use the S_ISDIR(file_stat.st_mode) and S_ISREG(file_stat.st_mode) macros
    //    to print whether it's a directory or a regular file.

    printf("Inspecting: %s\n", filepath);
    // Add your logic here

    if (stat(filepath, &file_stat) < 0) {
        printf(" Error: File '%s' does not exist or cannot be accessed.\n", filepath);
        return;
    }

    printf(" Size: %ld bytes\n", file_stat.size);

    if (file_stat.type == T_DIR) {
        printf(" Type: Directory\n");
    } else if (file_stat.type == T_FILE) {
        printf(" Type: Regular File\n");
    } else {
        printf(" Type: Unknown\n");
    }
}

int main() {
    // Ensure you have a file named "test.txt" in your working directory to test this properly,
    // or change the path to a known file/directory on your system.

    // Create a dummy file for testing purposes
    int fd = open("test.txt", O_CREATE | O_WRONLY);
    if(fd >= 0) {
        write(fd, "Hello File System!", 18);
        close(fd);
    }

    inspect_file("test.txt");
    inspect_file("."); // Inspect the current directory
    inspect_file("does_not_exist.bin");

    exit(0);
}