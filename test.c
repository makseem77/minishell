# include  "inc/minishell.h"

void exec_exemple(char **argv)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        printf("I'm the little child\n");
    }
    else
    {

        execve("/bin/ls", argv, NULL);
    }
}

void exmple_loop(char **argv)
{
    int i = 0;

    while (i < 5)
    {
        exec_exemple(argv);
        i++;
    }
}

int main(int argc, char **argv)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        exmple_loop(argv);
        execve("/bin/ls", argv, NULL);
    }
    while (wait(NULL) > 0);
    printf("I WANT TO CONTINUE ONLY IF ALL THE CHILDREN ARE DEAD\n");
    return 0;
}
