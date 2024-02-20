/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:50:58 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/20 21:59:07 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cette fonction configure les descripteurs de fichiers pour l'entrée et la sortie d'un processus enfant
void configure_io(int *pipe_fd, int index_cmd, int nb_cmds)
{
    printf("index_cmd: %d\n", index_cmd);
	// Si la commande n'est pas la première, redirige l'entrée standard vers la lecture du pipe précédent
    if (index_cmd != 0)
    {
        if(dup2(pipe_fd[(index_cmd - 1) * 2], STDIN_FILENO) == -1)
            perror("dup2, STDIN_FILENO");
        close(pipe_fd[(index_cmd - 1) * 2]);
    }
	// Si la commande n'est pas la dernière, redirige la sortie standard vers l'écriture du pipe suivant
    if (index_cmd != nb_cmds - 1)
    {
        if(dup2(pipe_fd[index_cmd * 2 + 1], STDOUT_FILENO) == -1)
            perror("dup2, STDOUT_FILENO");
        close(pipe_fd[index_cmd * 2 + 1]);
    }
    // Fermez les descripteurs de fichiers inutilisés
    printf("index_cmd: %d\n", index_cmd);
}

// Crée un processus enfant pour exécuter une commande
void child(t_token **tokenlist, t_data **data, int index_cmd, int nb_cmds)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // Processus enfant
        configure_io((*data)->pipe_fd, index_cmd, nb_cmds);
        process_tokens(tokenlist, data); // Exécute la commande
        exit(EXIT_SUCCESS);
    }
}

// Gère la création de tous les processus enfants pour les commandes
void children(t_token **tokenlist, t_data **data)
{
    int nb_cmds = (*data)->nb_pipe + 1;
    t_token	*tmp = *tokenlist;
    int		i = 0;

    while (tmp && i < nb_cmds)
    {
        if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
            child(&tmp, data, i++, nb_cmds);
        tmp = tmp->next;
    }
    
    // Fermez les descripteurs de fichiers dans le processus parent ici
    i = 0;
     // Attend la fin de tous les processus enfants
    while (wait(NULL) > 0);
    while (i < 2 * (*data)->nb_pipe) {
        close((*data)->pipe_fd[i]);
        i++;    
    }
}
