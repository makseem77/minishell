/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/07 05:00:51 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Go trough the tokenlist
//For >> and > operator, we check if it exists already, if not we create it, if yes we open it in append mode for >> and we remove its content for >
//For << and < still need to test some stuff
//We remove all the tokens that have the REDIRECTION AND REDIRECTION FILE TYPE and we keep the fd of the last file we passed on, this will be the one we write on.
//We store the fd somewhere (we could store in on the command token ?) so that in the execution, if we have a fd on the command we know we have to output to this one.
