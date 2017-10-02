#include "types.h"
#include "user.h"

int main(void) {
    char *argv[] = {
        "grep",
        "xv6",
        "/README",
        0
    };
    exec("/grep", argv);
    printf(2, "exec failed\n");
    exit();
}

