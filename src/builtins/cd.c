#include "../../inc/minishell.h"

void ft_cd(char *absolute_path)
{
    chdir(absolute_path);
}