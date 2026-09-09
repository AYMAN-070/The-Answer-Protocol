/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:28:08 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 22:22:01 by ayhammou         ###   ########.fr       */
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
# include <arpa/inet.h>

typedef struct s_player
{
	int		sd;			// Son numéro de connexion
	char	name[32];	// Son pseudo
	int		is_active;	// 1 si la place est prise, 0 si elle est libre
	char	location[32];
	char	last_location[32];
	int		hp;
	int		max_hp;
	int		in_combat;
	char	inventory[5][32];
	char	ip[16];
	long	last_cmd_time;
	int		cmd_count;
}	t_player;

void	exit_error(char *msg);
int		init_server(void);
void	process_command(t_player *player, t_player *clients,
			char *buffer, char *world_data);
char	*load_world(char *filename);
void	extract_json_value(char *json, char *key, char *result);
void	get_room_exit(char *json, char *room, char *dir, char *dest);
void	handle_quit(t_player *player);
void	handle_look(t_player *player, char *world_data);
void	handle_move(t_player *player, int words, char *arg, char *world);
void	handle_inventory(t_player *player);
void	handle_take(t_player *player, int words, char *item, char *world);
int		check_item_in_room(char *json, char *room, char *item);
int		has_item(t_player *player, char *item);
int		check_npc_in_room(char *json, char *room, char *npc);
void	handle_attack(t_player *player, int words, char *target, char *world);
void	give_item(t_player *player, char *item);
void	handle_defend(t_player *player, int words, char *tgt, char *world);
void	handle_flee(t_player *player, char *world);
void	handle_drop(t_player *player, int words, char *item);
void	handle_talk(t_player *player, int words, char *npc, char *world);
void	handle_status(t_player *player);
void	handle_quest(t_player *player);
void	server_log(char *level, char *ip, char *player, char *msg);
int		check_abuse(t_player *player);

#endif