# include  "inc/minishell.h"

int main(int argc, char const *argv[])
{
    int fd = open("file1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    // printf("Hello, World!\n");
    write(1, "Hello, World!\n", 14);
    return 0;
}
