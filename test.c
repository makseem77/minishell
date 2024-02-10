#include <unistd.h>

int main(int argc, char const *argv[], char **envp)
{
    char *argvperso[] = {"ls", "-l", NULL};
    execve("/bin/ls", argvperso, envp);
    // if(access("/bin/ls",  X_OK) == 0)
    //     write(1, "yes\n", 4);
    // else
    //     write(1, "no\n", 3);
    return 0;
}
