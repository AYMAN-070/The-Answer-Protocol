/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:28:08 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/03 09:49:55 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	location[32];
	int		hp;
	int		max_hp;
	int		in_combat;
	char	inventory[5][32]; 
}	t_player;

void	exit_error(char *msg);
int		init_server(void);
void	process_command(t_player *player, t_player *clients,
			char *buffer, char *world_data);
char	*load_world(char *filename);
void	extract_json_value(char *json, char *key, char *result);
void	get_room_exit(char *json, char *room, char *dir, char *dest);
void	handle_look(t_player *player, char *world_data);
void	handle_move(t_player *player, int words, char *arg, char *world);
void	handle_inventory(t_player *player);
void	handle_take(t_player *player, int words, char *item, char *world);
int		check_item_in_room(char *json, char *room, char *item);
int		has_item(t_player *player, char *item);

#endif