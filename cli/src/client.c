/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 17:11:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 17:03:14 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

static int	handle_server_msg(int sock, char *buffer)
{
	memset(buffer, 0, 1024);
	if (read(sock, buffer, 1023) <= 0)
		return (0);
	printf("\rServeur : %s> ", buffer);
	fflush(stdout);
	return (1);
}

static int	handle_user_input(int sock, char *buffer)
{
	memset(buffer, 0, 1024);
	if (!fgets(buffer, 1024, stdin) || !strncmp(buffer, "QUIT\n", 5))
		return (0);
	write(sock, buffer, strlen(buffer));
	printf("> ");
	fflush(stdout);
	return (1);
}

static void	client_loop(int sock)
{
	fd_set	read_fds;
	char	buffer[1024];

	printf("Connected. Tape 'QUIT' to quit.\n> ");
	fflush(stdout);
	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(0, &read_fds);
		FD_SET(sock, &read_fds);
		if (select(sock + 1, &read_fds, NULL, NULL, NULL) < 0)
			break ;
		if (FD_ISSET(sock, &read_fds) && !handle_server_msg(sock, buffer))
			break ;
		if (FD_ISSET(0, &read_fds) && !handle_user_input(sock, buffer))
			break ;
	}
}

int	main(void)
{
	int					sock;
	struct sockaddr_in	serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		exit_error("Erreur socket client");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4242);
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
		exit_error("Adresse invalide");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		exit_error("Erreur connect to server");
	client_loop(sock);
	close(sock);
	return (0);
}
