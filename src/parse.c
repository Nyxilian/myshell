#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

void free_pipeline(Pipeline *p) {
    if (!p) return;

    for (int i = 0; i < p->command_count; i++) {
        Command *cmd = &p->commands[i];
        
        for (int j = 0; j < cmd->argc; j++) {
            free(cmd->argv[j]);
        }
        
        if (cmd->input_file) free(cmd->input_file);
        if (cmd->output_file) free(cmd->output_file);
    }

    free(p);
}

static void parse_command_segment(char *segment, Command *cmd) {
    char *token;
    char *saveptr;
    int arg_idx = 0;

    token = strtok_r(segment, " \t\n", &saveptr);
    while (token != NULL && arg_idx < MAX_ARGS - 1) {
        cmd->argv[arg_idx++] = strdup(token);
        token = strtok_r(NULL, " \t\n", &saveptr);
    }
    cmd->argv[arg_idx] = NULL;
    cmd->argc = arg_idx;

    int new_argc = 0;
    for (int i = 0; i < cmd->argc; i++) {
        if (strcmp(cmd->argv[i], ">") == 0) {
            if (i + 1 < cmd->argc) {
                cmd->output_file = strdup(cmd->argv[i + 1]);
                cmd->append_mode = 0;
                free(cmd->argv[i]);
                free(cmd->argv[i+1]);
                i++; 
            }
        } else if (strcmp(cmd->argv[i], ">>") == 0) {
            if (i + 1 < cmd->argc) {
                cmd->output_file = strdup(cmd->argv[i + 1]);
                cmd->append_mode = 1;
                free(cmd->argv[i]);
                free(cmd->argv[i+1]);
                i++;
            }
        } else if (strcmp(cmd->argv[i], "<") == 0) {
            if (i + 1 < cmd->argc) {
                cmd->input_file = strdup(cmd->argv[i + 1]);
                free(cmd->argv[i]);
                free(cmd->argv[i+1]);
                i++;
            }
        } else {
            cmd->argv[new_argc++] = cmd->argv[i];
        }
    }
    for (int i = new_argc; i < cmd->argc; i++) {
        cmd->argv[i] = NULL;
    }
    cmd->argc = new_argc;
}

Pipeline* parse_line(char *line) {
    Pipeline *p = (Pipeline *)calloc(1, sizeof(Pipeline));
    if (!p) return NULL;
    
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[--len] = '\0';
    
    if (len > 0 && line[len - 1] == '&') {
        p->is_background = 1;
        line[len - 1] = '\0';
    }

    char *command_str;
    char *saveptr;
    
    command_str = strtok_r(line, "|", &saveptr);
    while (command_str != NULL && p->command_count < 2) {
        parse_command_segment(command_str, &p->commands[p->command_count]);
        p->command_count++;
        command_str = strtok_r(NULL, "|", &saveptr);
    }

    return p;
}
