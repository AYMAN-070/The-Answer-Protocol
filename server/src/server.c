/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 14:27:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/19 14:40:50 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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
	int					sd;
	struct sockaddr_in	addr;
	socklen_t			len;
	int					i;

	len = sizeof(addr);
	sd = accept(server_fd, (struct sockaddr *)&addr, &len);
	if (sd < 0)
		exit_error("Accept error");
	write(sd, "OK hello proto=1\n", 17);
	server_log("INFO", inet_ntoa(addr.sin_addr), "unknown", "New connection");
	i = -1;
	while (++i < 10)
	{
		if (clients[i].is_active == 0)
		{
			memset(&clients[i], 0, sizeof(t_player));
			clients[i].sd = sd;
			clients[i].is_active = 1;
			strncpy(clients[i].ip, inet_ntoa(addr.sin_addr), 15);
			clients[i].ip[15] = '\0';
			return ;
		}
	}
}

static void	read_client_data(t_player *p, t_player *c, char *wld)
{
	int		b;
	char	*nl;

	b = read(p->sd, p->cmd_buffer + p->cmd_len, 1023 - p->cmd_len);
	if (b <= 0)
	{
		server_log("INFO", p->ip, p->name, "Disconnected");
		close(p->sd);
		p->is_active = 0;
		p->cmd_len = 0;
		memset(p->cmd_buffer, 0, sizeof(p->cmd_buffer));
		return (broadcast_player_stats(c));
	}
	p->cmd_len += b;
	p->cmd_buffer[p->cmd_len] = '\0';
	nl = strchr(p->cmd_buffer, '\n');
	while (nl)
	{
		*nl = '\0';
		if (p->cmd_buffer[0] != '\0')
			process_command(p, c, p->cmd_buffer, wld);
		p->cmd_len -= (nl - p->cmd_buffer) + 1;
		memmove(p->cmd_buffer, nl + 1, p->cmd_len + 1);
		nl = strchr(p->cmd_buffer, '\n');
	}
}

void	handle_client_messages(t_player *clients, fd_set *readfds, char *wld)
{
	int	i;

	i = -1;
	while (++i < 10)
	{
		if (clients[i].is_active == 1 && FD_ISSET(clients[i].sd, readfds))
		{
			read_client_data(&clients[i], clients, wld);
		}
	}
}

int	main(void)
{
	int			server_fd;
	t_player	clients[10];
	fd_set		readfds;
	int			max_sd;
	char		*world_data;

	srand(time(NULL));
	setvbuf(stdout, NULL, _IOLBF, 0);
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
