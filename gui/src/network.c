/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 08:09:19 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:32:09 by qdecross         ###   ########.fr       */
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
#include <errno.h>

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

static void	push_rx_data(t_ui_context *ctx, const char *chunk, int len)
{
	char	*last;
	size_t	used;
	int		idx;

	used = strlen(ctx->rx_buf);
	idx = -1;
	while (++idx < len && used < sizeof(ctx->rx_buf) - 1)
		if (chunk[idx] == '\n' || (unsigned char)chunk[idx] >= 32)
			ctx->rx_buf[used++] = chunk[idx];
	ctx->rx_buf[used] = '\0';
	last = strrchr(ctx->rx_buf, '\n');
	if (!last && used >= sizeof(ctx->rx_buf) - 1)
		ctx->rx_buf[0] = '\0';
	if (!last)
		return ;
	*last = '\0';
	parse_server_text(ctx->rx_buf, ctx);
	memmove(ctx->rx_buf, last + 1, strlen(last + 1) + 1);
}

int	net_receive_state(int sock_fd, t_ui_context *ctx)
{
	char	buffer[2048];
	int		bytes_read;

	if (sock_fd < 0)
		return (-1);
	bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
	if (bytes_read > 0)
	{
		push_rx_data(ctx, buffer, bytes_read);
		return (1);
	}
	if (bytes_read == 0)
		return (-1);
	if (errno != EAGAIN && errno != EWOULDBLOCK)
		return (-1);
	return (0);
}
