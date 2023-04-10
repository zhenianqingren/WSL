#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PAGER "/bin/more"
#define MAXLINE 256

int main(int argc, char *argv[]) {
  int n;
  int fd[2];
  pid_t pid;
  char *pager, *argv0;
  char line[MAXLINE];
  FILE *fp;
  printf("%s\n", argv[0]);

  if (argc != 2)
    return -1;

  if ((fp = fopen(argv[1], "r")) == NULL || pipe(fd) < 0)
    return -1;

  if ((pid = fork()) < 0)
    return -1;
  else if (pid > 0) {
    close(fd[0]);
    while (fgets(line, n, fp) != NULL) {
      n = strlen(line);
      if (write(fd[1], line, n) != n)
        return -1;
    }
    close(fd[1]);
    if (waitpid(pid, NULL, 0) < 0)
      return -1;
    exit(0);
  } else {
    close(fd[1]);
    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
        return -1;
      close(fd[0]);
    }

    if ((argv0 = strrchr(PAGER, '/')) != NULL)
      ++argv0;
    else
      argv0 = PAGER;

    if (execl(PAGER, argv0, (char *)0) < 0)
      return -1;
  }

  return 0;
}