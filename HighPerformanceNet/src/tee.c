#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    assert(fd >= 0);

    int pipe_stdout[2];
    int ret = pipe(pipe_stdout);
    assert(ret != -1);

    int pipe_f[2];
    ret = pipe(pipe_f);
    assert(ret != -1);

    ret = splice(STDIN_FILENO, NULL, pipe_stdout[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    ret = tee(pipe_stdout[0], pipe_f[1], 32768, SPLICE_F_NONBLOCK);
    assert(ret != -1);

    ret = splice(pipe_f[0], NULL, fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    ret = splice(pipe_stdout[0], STDOUT_FILENO, fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    close(fd);
    close(pipe_stdout[0]);
    close(pipe_stdout[1]);
    close(pipe_f[0]);
    close(pipe_f[1]);
    return 0;
}
