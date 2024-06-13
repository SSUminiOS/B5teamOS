#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void file_system();

int main() {
    printf("\n[MiniOS SSU] Hello, World!\n");
    file_system();
    printf("[MiniOS SSU] MiniOS Shutdown........");

    return 0;
}
