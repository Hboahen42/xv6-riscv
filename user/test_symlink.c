#include "../user/user.h"
#include "../kernel/fcntl.h"

void
test_basic(void) {
    // Create a real file with content.
    int fd = open("target.txt", O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf("FAIL: target file creation failed\n");
        exit(1);
    }
    write(fd, "hello", 5);
    close(fd);

    // Create a symlink pointing to it.
    if (symlink("target.txt", "symlink.txt") < 0) {
        printf("FAIL: symlink creation failed\n");
        exit(1);
    }

    // Open through the symlink and verify content
    fd = open("symlink.txt", O_RDONLY);
    if (fd < 0) {
        printf("FAIL: symlink open failed\n");
        exit(1);
    }
    char buf[10];
    int n = read(fd, buf, 5);
    close(fd);

    if (n != 5 || buf[0] != 'h') {
        printf("FAIL: symlink read failed\n");
        exit(1);
    }

    printf("Pass: symlink test successful\n");
    unlink("symlink.txt");
    unlink("target.txt");
}

void
test_chain(void) {
    // Create a real file
    int fd = open("test.txt", O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("FAIL: test file creation failed\n");
        exit(1);
    }
    write(fd, "x", 1);
    close(fd);

    // Create a chain: c -> b -> a -> test.txt
    symlink("test.txt", "a.txt");
    symlink("a.txt", "b.txt");
    symlink("b.txt", "c.txt");

    fd = open("c.txt", O_RDONLY);
    if (fd < 0) {
        printf("FAIL: chain symlink creation failed\n");
        close(fd);
        exit(1);
    }
    close(fd);

    printf("Pass: chain symlink test successful\n");
    unlink("c.txt");
    unlink("b.txt");
    unlink("a.txt");
    unlink("test.txt");

}

void
test_dangling(void) {
    // Create a symlink pointing to a file that does not exist
    symlink("nonexistent.txt", "dangling.txt");

    int fd = open("dangling.txt", O_RDONLY);
    if (fd >= 0) {
        printf("FAIL: dangling symlink should not be readable\n");
        close(fd);
        exit(1);
    }

    printf("Pass: dangling symlink test successful\n");
    unlink("dangling.txt");
}

void
test_cycle(void) {
    // Create a two-node cycle: b -> a -> b
    symlink("b_link.txt", "a_link.txt");
    symlink("a_link.txt", "b_link.txt");

    int fd = open("a_link.txt", O_RDONLY);
    if (fd >= 0) {
        printf("FAIL: cycle symlink should not be readable\n");
        close(fd);
        exit(1);
    }

    printf("Pass: cycle symlink test successful\n");
    unlink("a_link.txt");
    unlink("b_link.txt");
}

void
test_nofollow(void) {
    // Create a real file and symlink to it
    int fd = open("nf_target.txt", O_CREATE | O_WRONLY);
    close(fd);
    symlink("nf_target.txt", "nf_link.txt");

    // O_NOFOLLOW should return an error when opening a symlink
    fd = open("nf_link.txt", O_RDONLY | O_NOFOLLOW);
    if (fd >= 0) {
        printf("FAIL: O_NOFOLLOW should not follow symlink\n");
        close(fd);
        exit(1);
    }

    printf("Pass: nofollow symlink test successful\n");
    unlink("nf_target.txt");
    unlink("nf_link.txt");
}

int
main(int argc, char *argv[]) {
    test_basic();
    test_chain();
    test_dangling();
    test_cycle();
    test_nofollow();
    return 0;
}

