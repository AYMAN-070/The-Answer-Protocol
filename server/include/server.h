#ifndef SERVER_H
# define SERVER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>

typedef struct s_player
{
	int		sd;			// Son numéro de connexion
	char	name[32];	// Son pseudo
	int		is_active;	// 1 si la place est prise, 0 si elle est libre
}   t_player;

void	exit_error(char *msg);
int		init_server(void);
void	process_command(int sd, char *buffer);

#endif