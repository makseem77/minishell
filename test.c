#include <unistd.h>

int main(int argc, char const *argv[], char **envp)
{
    char *argvperso[] = {"ls", "-l", NULL};
    execve("/bin/ls", argvperso, envp);
    return 0;
}
