#include "forkexec.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>


int forkexec(char *cmd[], bool wait_for_exit) {
  pid_t child_pid = fork();

  if (child_pid == -1) {
    // within parent: fork failed

    perror("An error occured while calling fork");
    return EXIT_FAILURE;

  } else if (child_pid == 0) {
    // within child: run command

    execvp(cmd[0], cmd);
    // if executed, an error in execve occured
    perror("An error occured while calling execve");
    return EXIT_FAILURE;

  } else {
    // within parent: wait for child exit

    if (wait_for_exit) {
      int child_status;
      waitpid(child_pid, &child_status, 0);
      if (WIFEXITED(child_status)) {
        printf("Child %d exited normally with exit code %d\n",
               child_pid,
               child_status);
      }
    }
  }

  return EXIT_SUCCESS;
}
