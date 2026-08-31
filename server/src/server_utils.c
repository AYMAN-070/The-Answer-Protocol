#include "server.h"

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	init_server(void)
{
	int					server_fd;
	int					opt;
	struct sockaddr_in	address;

	opt = 1;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
		exit_error("Socket error");
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(4242);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		exit_error("Bind error");
	if (listen(server_fd, 3) < 0)
		exit_error("Listen error");
	return (server_fd);
}