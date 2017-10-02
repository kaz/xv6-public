#include "types.h"
#include "user.h"

int main(void) {
    char *argv[] = {
        "grep",
        "xv6",
        "/README",
        0
    };
    if (fork() == 0) {
        printf(1, "child: process (pid=%d) started\n", getpid());
        exec("/grep", argv);
        printf(2, "child: exec failed\n");
    }
    else {
        int cpid = wait();
        printf(1, "parent: child process (pid=%d) finished\n", cpid);
    }
    exit();
}

