/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 17:11:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/08/31 18:22:46 by ayhammou         ###   ########.fr       */
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

int	main(void)
{
	int					sock;
	struct sockaddr_in	serv_addr;
	char				buffer[1024];

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
	printf("Connected. Tape 'QUIT' to quit.\n");
	while (1)
	{
		printf("> ");
		memset(buffer, 0, 1024);
		if (fgets(buffer, 1024, stdin) == NULL)
			break ;
		if (strncmp(buffer, "QUIT\n", 5) == 0)
			break ;
		write(sock, buffer, strlen(buffer));
		memset(buffer, 0, 1024);
		if (read(sock, buffer, 1023) > 0)
			printf("Serveur : %s", buffer);
	}
	close(sock);
	return (0);
}
