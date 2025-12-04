#ifndef LOGGER_H
#define LOGGER_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void init_logger();
void log_command(int pid, char *cmd, int status);

#endif
