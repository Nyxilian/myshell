#include "logger.h"

static int log_fd = -1;

void init_logger() {
    log_fd = open("myshell.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Failed to open log file");
    }
}

void close_logger() {
    if (log_fd >= 0) {
        close(log_fd);
        log_fd = -1;
    }
}

void log_command(int pid, char *cmd, int status) {
    if (log_fd < 0) return;
    
    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer), "[pid=%d] cmd=\"%s\" status=%d\n", pid, cmd, status);
    
    if (len < 0) {
        perror("snprintf error");
        return;
    }
    if( len >= (int)sizeof(buffer)) {
        perror("log entry truncated");
        len = sizeof(buffer) - 1;
    }
    if (len > 0) {
        write(log_fd, buffer, len);
    }
}
