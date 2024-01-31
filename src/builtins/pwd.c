#include "../../inc/minishell.h"

void ft_pwd(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    ft_putendl_fd(cwd, 1);
    free(cwd);
}