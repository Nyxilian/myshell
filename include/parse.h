#ifndef PARSE_H
#define PARSE_H

#define MAX_ARGS 64

typedef struct {
    char *argv[MAX_ARGS];
    int argc;
    char *input_file;
    char *output_file;
    int append_mode;
} Command;

typedef struct {
    Command commands[2];
    int command_count;
    int is_background;
} Pipeline;

Pipeline* parse_line(char *line);
void free_pipeline(Pipeline *p);

#endif
