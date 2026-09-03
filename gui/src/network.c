/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 08:09:19 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 08:49:19 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "network_parser.h"
#include "cJSON.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int	net_connect(const char *ip, int port)
{
	struct sockaddr_in	serv_addr;
	int					flags;
	int					sock_fd;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
		return (-1);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
	{
		close(sock_fd);
		return (-1);
	}
	if (connect(sock_fd, (struct sockaddr *)&serv_addr,
			sizeof(serv_addr)) < 0)
	{
		close(sock_fd);
		return (-1);
	}
	flags = fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
	return (sock_fd);
}

void	net_send_command(int sock_fd, const char *cmd)
{
	if (sock_fd >= 0)
		send(sock_fd, cmd, strlen(cmd), 0);
}

void	net_disconnect(int *sock_fd)
{
	if (*sock_fd >= 0)
	{
		close(*sock_fd);
		*sock_fd = -1;
	}
}

int	net_receive_state(int sock_fd, t_room_state *state)
{
	char	buffer[2048];
	int		bytes;
	cJSON	*json;

	if (sock_fd < 0)
		return (0);
	bytes = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes > 0)
	{
		buffer[bytes] = '\0';
		json = cJSON_Parse(buffer);
		if (json)
		{
			populate_state(json, state);
			cJSON_Delete(json);
			return (1);
		}
	}
	return (0);
}
