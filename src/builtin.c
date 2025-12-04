#include "builtin.h"

int handle_builtin(Command *cmd) {
    if (cmd->argc == 0) return 0;

    if (strcmp(cmd->argv[0], "exit") == 0 || strcmp(cmd->argv[0], "quit") == 0) {
        exit(0);
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        char *path = cmd->argv[1];
        if (path == NULL) {
            path = getenv("HOME");
        }
        
        if (chdir(path) != 0) {
            perror("cd error");
        }
        return 1; // Executed builtin
    }

    return 0; // Not a builtin
}
