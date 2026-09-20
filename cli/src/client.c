/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 17:11:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/20 11:09:37 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static int	handle_server_msg(int sock, char *buffer, int *len)
{
	int	b;

	b = read(sock, buffer + *len, 1023 - *len);
	if (b <= 0)
		return (0);
	*len += b;
	buffer[*len] = '\0';
	print_lines(buffer, len);
	return (1);
}

static int	handle_user_input(int sock, char *buffer)
{
	memset(buffer, 0, 1024);
	if (!fgets(buffer, 1024, stdin))
		return (0);
	write(sock, buffer, strlen(buffer));
	printf("> ");
	fflush(stdout);
	return (1);
}

static void	client_loop(int sock)
{
	fd_set	read_fds;
	char	in_buf[1024];
	char	rx_buf[1024];
	int		rx_len;

	rx_len = 0;
	rx_buf[0] = '\0';
	printf("Connected. Tape 'QUIT' to quit.\n> ");
	fflush(stdout);
	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(0, &read_fds);
		FD_SET(sock, &read_fds);
		if (select(sock + 1, &read_fds, NULL, NULL, NULL) < 0)
			break ;
		if (FD_ISSET(sock, &read_fds)
			&& !handle_server_msg(sock, rx_buf, &rx_len))
			break ;
		if (FD_ISSET(0, &read_fds) && !handle_user_input(sock, in_buf))
			break ;
	}
}

static int	connect_to_server(int argc, char **argv)
{
	struct sockaddr_in	serv_addr;
	const char			*ip;
	int					port;
	int					sock;

	ip = "127.0.0.1";
	port = 4242;
	if (argc > 1)
		ip = argv[1];
	if (argc > 2)
		port = atoi(argv[2]);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		exit_error("Erreur socket client");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
		exit_error("Adresse invalide");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		exit_error("Erreur connect to server");
	return (sock);
}

int	main(int argc, char **argv)
{
	int	sock;

	sock = connect_to_server(argc, argv);
	client_loop(sock);
	close(sock);
	return (0);
}
