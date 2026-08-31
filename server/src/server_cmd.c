/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 20:08:00 by ayhammou          #+#    #+#             */
/*   Updated: 2026/08/31 20:31:31 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_connect(int sd, int words, char *arg)
{
	if (words == 2)
	{
		printf("Le joueur sur la socket %d s'appelle : %s\n", sd, arg);
		write(sd, "OK connected\n", 13);
	}
	else
	{
		printf("Erreur socket %d : Nom manquant pour CONNECT.\n", sd);
		write(sd, "ERR missing name\n", 17);
	}
}

void	handle_quit(int sd)
{
	printf("Le joueur sur la socket %d se déconnecte.\n", sd);
	write(sd, "OK goodbye\n", 11);
}

void	process_command(int sd, char *buffer)
{
	char	cmd[32];
	char	arg[32];
	int		words;

	memset(cmd, 0, 32);
	memset(arg, 0, 32);
	words = sscanf(buffer, "%31s %31s", cmd, arg);
	if (words >= 1 && strcmp(cmd, "CONNECT") == 0)
		handle_connect(sd, words, arg);
	else if (words >= 1 && strcmp(cmd, "QUIT") == 0)
		handle_quit(sd);
	else
	{
		printf("Erreur socket %d : Commande inconnue.\n", sd);
		write(sd, "ERR unknown command\n", 20);
	}
}
