/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 17:11:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/08/31 17:12:26 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	main(void)
{
	int					sock;
	struct sockaddr_in	serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		exit_error("Error socket");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4242);
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
		exit_error(" invalid Adresse");
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		exit_error("Error connect");
	printf("Connected to TAP server !\n");
	close(sock);
	return (0);
}
