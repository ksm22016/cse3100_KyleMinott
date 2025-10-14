#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child;
    int exitStatus;

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
    child = fork();
    if (child < 0) {
        perror("fork()");
        return 1;
    } else if (child == 0) {
        if (execlp(argv[1], argv[1], argv[2], (char *)NULL) == -1) {
            perror("execlp()");
            exit(0);
        }
    } else {
        if (waitpid(child, &exitStatus, 0) == -1) {
            perror("waitpid()");
            return 1;
        }
        printf("exited=%d exitstatus=%d\n",
                WIFEXITED(exitStatus) ? 1 : 0,
                WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus) : -1);
    }

    char **cmd2_argv = &argv[3];

    child = fork();
    if (child < 0) {
        perror("fork()");
        return 1;
    } else if (child == 0) {
        if (execvp(cmd2_argv[0], cmd2_argv) == -1) {
            perror("execvp()");
            exit(0);
        }
    } else {
        if (waitpid(child, &exitStatus, 0) == -1) {
            perror("waitpid()");
            return 1;
        }
        printf("exited=%d exitstatus=%d\n",
                WIFEXITED(exitStatus) ? 1 : 0,
                WIFEXITED(exitStatus) ? WEXITSTATUS(exitStatus) : -1);
    }

    return 0;
}