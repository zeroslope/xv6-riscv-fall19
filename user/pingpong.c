#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int parent_fd[2], child_fd[2];
  char buf[1];

  if (pipe(parent_fd) == -1) {
    fprintf(2, "create pipe error\n");
    exit();
  }
  if (pipe(child_fd) == -1) {
    fprintf(2, "create pipe error\n");
    exit();
  }

  int pid = fork();

  if (pid < 0) {
    fprintf(2, "fork failed\n");
  } else if (pid == 0) {
    // child
    close(parent_fd[1]);
    if (read(parent_fd[0], buf, 1) == 1) {
      printf("%d: received ping\n", getpid());
    }
    close(parent_fd[0]);
    close(child_fd[0]);
    write(child_fd[1], "c", 1);
    close(child_fd[1]);
  } else {
    // parent
    close(parent_fd[0]);
    write(parent_fd[1], "p", 1);
    close(parent_fd[1]);
    wait();
    close(child_fd[1]);
    if (read(child_fd[0], buf, 1) == 1) {
      printf("%d: received pong\n", getpid());
    }
    close(child_fd[0]);
  }

  exit();
}
