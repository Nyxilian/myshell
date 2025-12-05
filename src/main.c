#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "execute.h"
#include "signals.h"
#include "logger.h"

#define MAX_LINE 1024

int main() {
    setup_signals();
    init_logger();

    char line[MAX_LINE];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
           if(ferror(stdin)) {
               perror("fgets error");
           }
            printf("\n");
            break;
        }

        if (strcmp(line, "\n") == 0) continue;
        
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            break;//exit check here to avoid parsing if possible
        }

        Pipeline *p = parse_line(line);
        
        if (p) {
            execute_pipeline(p);
            free_pipeline(p);
        }
    }
    
    close_logger();
    return 0;
}
