#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"

static int log_fd = -1;

void init_logger() {
    log_fd = open("myshell.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void log_command(int pid, char *cmd, int status) {
    if (log_fd < 0) return;
    
    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer), "[pid=%d] cmd=\"%s\" status=%d\n", pid, cmd, status);
    
    if (len > 0) {
        write(log_fd, buffer, len);
    }
}
