/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 14:27:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 22:31:02 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	init_clients(t_player *clients)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		clients[i].sd = 0;
		clients[i].is_active = 0;
		memset(clients[i].name, 0, 32);
		i++;
	}
}

int	setup_fd_set(int server_fd, t_player *clients, fd_set *readfds)
{
	int	i;
	int	max_sd;

	FD_ZERO(readfds);
	FD_SET(server_fd, readfds);
	max_sd = server_fd;
	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 1)
		{
			FD_SET(clients[i].sd, readfds);
			if (clients[i].sd > max_sd)
				max_sd = clients[i].sd;
		}
		i++;
	}
	return (max_sd);
}

void	handle_new_connection(int server_fd, t_player *clients)
{
	int					new_sd;
	struct sockaddr_in	addr;
	socklen_t			len;
	int					i;
	char				*ip_str;

	len = sizeof(addr);
	new_sd = accept(server_fd, (struct sockaddr *)&addr, &len);
	if (new_sd < 0)
		exit_error("Accept error");
	ip_str = inet_ntoa(addr.sin_addr);
	server_log("INFO", ip_str, "unknown", "New connection established");
	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 0)
		{
			clients[i].sd = new_sd;
			clients[i].is_active = 1;
			strncpy(clients[i].ip, ip_str, 15);
			clients[i].ip[15] = '\0';
			break ;
		}
		i++;
	}
}

void	handle_client_messages(t_player *clients,
	fd_set *readfds, char *world_data)
{
	int		i;
	char	buffer[1024];

	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 1 && FD_ISSET(clients[i].sd, readfds))
		{
			memset(buffer, 0, 1024);
			if (read(clients[i].sd, buffer, 1023) <= 0)
			{
				server_log("INFO", clients[i].ip, clients[i].name,
					"Disconnected");
				close(clients[i].sd);
				clients[i].is_active = 0;
			}
			else
				process_command(&clients[i], clients, buffer, world_data);
		}
		i++;
	}
}

int	main(void)
{
	int			server_fd;
	t_player	clients[10];
	fd_set		readfds;
	int			max_sd;
	char		*world_data;

	server_fd = init_server();
	init_clients(clients);
	world_data = load_world("data/world.json");
	server_log("INFO", "sys", "none", "World loaded successfully");
	server_log("INFO", "sys", "none", "TAP server started on port 4242");
	while (1)
	{
		max_sd = setup_fd_set(server_fd, clients, &readfds);
		if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
			exit_error("Select error");
		if (FD_ISSET(server_fd, &readfds))
			handle_new_connection(server_fd, clients);
		handle_client_messages(clients, &readfds, world_data);
	}
	return (0);
}
