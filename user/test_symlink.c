#include "../kernel/types.h"
#include "../kernel/stat.h"
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
        unlink("target.txt");
        exit(1);
    }

    // Open through the symlink and verify content
    fd = open("symlink.txt", O_RDONLY);
    if (fd < 0) {
        printf("FAIL: symlink open failed\n");
        unlink("target.txt");
        unlink("symlink.txt");
        exit(1);
    }

    char buf[10];
    int n = read(fd, buf, 5);
    close(fd);

    if (n != 5 || buf[0] != 'h') {
        printf("FAIL: wrong content read through symlink\n");
        unlink("target.txt");
        unlink("symlink.txt");
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
    if (symlink("test.txt", "a.txt") < 0) {
        printf("FAIL: chain symlink a.txt creation failed\n");
        unlink("test.txt");
        exit(1);
    }
    if (symlink("a.txt", "b.txt") < 0) {
        printf("FAIL: chain symlink b.txt creation failed\n");
        unlink("a.txt");
        unlink("test.txt");
        exit(1);
    };

    if (symlink("b.txt", "c.txt") < 0) {
        printf("FAIL: chain symlink c.txt creation failed\n");
        unlink("a.txt");
        unlink("b.txt");
        unlink("test.txt");
        exit(1);
    };

    fd = open("c.txt", O_RDONLY);
    if (fd < 0) {
        printf("FAIL: open through chain failed\n");
        unlink("c.txt");
        unlink("b.txt");
        unlink("a.txt");
        unlink("test.txt");
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
    if (symlink("nonexistent.txt", "dangling.txt") < 0) {
        printf("FAIL: dangling symlink creation failed\n");
        exit(1);
    };

    int fd = open("dangling.txt", O_RDONLY);
    if (fd >= 0) {
        printf("FAIL: dangling symlink should not be readable\n");
        close(fd);
        unlink("dangling.txt");
        exit(1);
    }

    printf("Pass: dangling symlink test successful\n");
    unlink("dangling.txt");
}

void
test_cycle(void) {
    // Create a two-node cycle: b -> a -> b
    if (symlink("b_link.txt", "a_link.txt") < 0) {
        printf("FAIL: cycle symlink a_link.txt creation failed\n");
        exit(1);
    };
    if (symlink("a_link.txt", "b_link.txt") < 0) {
        printf("FAIL: cycle symlink b_link.txt creation failed\n");
        unlink("a_link.txt");
        exit(1);
    };

    int fd = open("a_link.txt", O_RDONLY);
    if (fd >= 0) {
        printf("FAIL: cycle symlink should not be readable\n");
        close(fd);
        unlink("a_link.txt");
        unlink("b_link.txt");
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
    if (fd < 0) {
        printf("FAIL: nofollow target file creation failed\n");
        exit(1);
    }
    close(fd);

    if (symlink("nf_target.txt", "nf_link.txt") < 0) {
        printf("FAIL: nofollow symlink creation failed\n");
        unlink("nf_target.txt");
        exit(1);
    };

    // O_NOFOLLOW should return an error when opening a symlink
    fd = open("nf_link.txt", O_RDONLY | O_NOFOLLOW);
    if (fd >= 0) {
        printf("FAIL: O_NOFOLLOW should not follow symlink\n");
        close(fd);
        unlink("nf_target.txt");
        unlink("nf_link.txt");
        exit(1);
    }

    // Verify the real file is still accessible without O_NOFOLLOW
    fd = open("nf_target.txt", O_RDONLY);
    if (fd < 0) {
        printf("FAIL: real file should still be accessible after nofollow\n");
        unlink("nf_target.txt");
        unlink("nf_link.txt");
        exit(1);
    }
    close(fd);

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
    printf("ALL SYMLINK TESTS PASSED\n");
    exit(0);
}

