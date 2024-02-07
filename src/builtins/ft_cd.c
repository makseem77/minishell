/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:22:49 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/07 11:29:01 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(char **args)
{
	//cd without args will send you to the ~ directory (cd "" too)
	//cd on a file will output: Not a directory
	//cd on something that doesnt exist: Not a file or a directory
	//cd with more than 1 arguments will output: too many arguments
	//cd should work with absolute path and relative path
}
