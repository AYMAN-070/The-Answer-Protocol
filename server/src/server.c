/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 14:27:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/08/31 20:10:03 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	init_clients(int *clients)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		clients[i] = 0;
		i++;
	}
}

int	setup_fd_set(int server_fd, int *clients, fd_set *readfds)
{
	int	i;
	int	max_sd;
	int	sd;

	FD_ZERO(readfds);
	FD_SET(server_fd, readfds);
	max_sd = server_fd;
	i = 0;
	while (i < 10)
	{
		sd = clients[i];
		if (sd > 0)
			FD_SET(sd, readfds);
		if (sd > max_sd)
			max_sd = sd;
		i++;
	}
	return (max_sd);
}

void	handle_new_connection(int server_fd, int *clients)
{
	int					new_socket;
	struct sockaddr_in	address;
	socklen_t			addrlen;
	int					i;

	addrlen = sizeof(address);
	new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (new_socket < 0)
		exit_error("Accept error");
	printf("New player connected!\n");
	i = 0;
	while (i < 10)
	{
		if (clients[i] == 0)
		{
			clients[i] = new_socket;
			break ;
		}
		i++;
	}
}

void	handle_client_messages(int *clients, fd_set *readfds)
{
	int		i;
	int		sd;
	char	buffer[1024];

	i = 0;
	while (i < 10)
	{
		sd = clients[i];
		if (sd > 0 && FD_ISSET(sd, readfds))
		{
			memset(buffer, 0, 1024);
			if (read(sd, buffer, 1023) == 0)
			{
				printf("A player disconnected.\n");
				close(sd);
				clients[i] = 0;
			}
			else
			{
				process_command(sd, buffer);
			}
		}
		i++;
	}
}

int	main(void)
{
	int		server_fd;
	int		clients[10];
	fd_set	readfds;
	int		max_sd;

	server_fd = init_server();
	init_clients(clients);
	printf("TAP Multiplayer server listening on port 4242...\n");
	while (1)
	{
		max_sd = setup_fd_set(server_fd, clients, &readfds);
		if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
			exit_error("Select error");
		if (FD_ISSET(server_fd, &readfds))
			handle_new_connection(server_fd, clients);
		handle_client_messages(clients, &readfds);
	}
	return (0);
}
