#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

void execute_pipeline(Pipeline *p);

#endif
