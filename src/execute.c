#include "execute.h"
#include "builtin.h"
#include "logger.h"


void build_cmd_string(char *buffer, Command *cmd) {
    buffer[0] = '\0';
    for (int i = 0; i < cmd->argc; i++) {
        strcat(buffer, cmd->argv[i]);
        strcat(buffer, " ");
    }
}

void execute_pipeline(Pipeline *p) {
    if (p->command_count == 0) return;

    if (p->command_count == 1) {
        Command *cmd = &p->commands[0];

        if (handle_builtin(cmd)) return;

        pid_t pid = fork();
        if (pid == 0) {
            signal(SIGINT, SIG_DFL);
            
            if (cmd->input_file) {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd < 0) { perror("open input"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            
            if (cmd->output_file) {
                int flags = O_WRONLY | O_CREAT;
                if (cmd->append_mode) flags |= O_APPEND;
                else flags |= O_TRUNC;
                
                int fd = open(cmd->output_file, flags, 0644);
                if (fd < 0) { perror("open output"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            
            if (execvp(cmd->argv[0], cmd->argv) < 0) {
                perror("execvp");
                exit(1);
            }
        } else if (pid > 0) {
            int status = 0;
            if (!p->is_background) {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    status = WEXITSTATUS(status);
                }
            } else {
                printf("[bg] started pid %d\n", pid);
            }

            char cmd_buf[1024];
            build_cmd_string(cmd_buf, cmd);
            log_command(pid, cmd_buf, status);

        } else {
            perror("fork");
        }
    }
    else if (p->command_count == 2) {
        int pipefd[2];
        if (pipe(pipefd) < 0) {
            perror("pipe");
            return;
        }

        pid_t pid1 = fork();
        if (pid1 == 0) {
            signal(SIGINT, SIG_DFL);
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
            execvp(p->commands[0].argv[0], p->commands[0].argv);
            perror("execvp 1");
            exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            signal(SIGINT, SIG_DFL);
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            execvp(p->commands[1].argv[0], p->commands[1].argv);
            perror("execvp 2");
            exit(1);
        }

        close(pipefd[0]);
        close(pipefd[1]);
        
        int status1 = 0, status2 = 0;
        waitpid(pid1, &status1, 0);
        waitpid(pid2, &status2, 0);

        if (WIFEXITED(status1)) status1 = WEXITSTATUS(status1);
        if (WIFEXITED(status2)) status2 = WEXITSTATUS(status2);

        char cmd_buf[1024];
        
        build_cmd_string(cmd_buf, &p->commands[0]);
        log_command(pid1, cmd_buf, status1);

        build_cmd_string(cmd_buf, &p->commands[1]);
        log_command(pid2, cmd_buf, status2);
    }
}
