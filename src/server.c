/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 14:27:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/08/29 10:01:11 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	init_server(struct sockaddr_in *address)
{
	int	server_fd;
	int	opt;

	opt = 1;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		exit_error("Error socket");
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(4242);
	if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0)
		exit_error("Error bind");
	if (listen(server_fd, 3) < 0)
		exit_error("Error listen");
	return (server_fd);
}

int	main(void)
{
	int					server_fd;
	int					new_socket;
	struct sockaddr_in	address;
	socklen_t			addrlen;

	addrlen = sizeof(address);
	server_fd = init_server(&address);
	printf("Server TAP listen on port 4242...\n");
	new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
	if (new_socket < 0)
		exit_error("Erreur accept");
	printf("Un joueur vient de se connecter !\n");
	close(new_socket);
	close(server_fd);
	return (0);
}