# include  "inc/minishell.h"

int main(int argc, char const *argv[])
{
    int fd = open("file1.txt", O_RDWR | O_CREAT, 0644);
    dup2(fd, STDIN_FILENO);
    execve("/bin/wc", (char *[]){"wc", NULL}, NULL);
    return 0;
}
