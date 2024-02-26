#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> // for open flags
#include <time.h> // for time measurement
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void	recursive_execution(char **command, int command_count)
{
	char *const argv[] = {*command, NULL};
	if (command_count == 1)
	{
		printf("CMD = %s\n", *command);
		execvp(*command, argv);
	}
	else
		recursive_execution(command + 1, command_count - 1);
}

int	main(int argc, char **argv)
{
	int	fd[2];

	recursive_execution((argv + 1), argc - 1);
}
