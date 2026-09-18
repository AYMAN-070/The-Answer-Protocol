/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:28:08 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:12:30 by ayhammou         ###   ########.fr       */
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
# include <time.h>

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
	int		group_id;
	int		quest_status;
	int		quest_override;
	char	cmd_buffer[1024];
	int		cmd_len;
}	t_player;

void	exit_error(char *msg);
int		init_server(void);
void	init_clients(t_player *clients);
void	process_command(t_player *player, t_player *clients,
			char *buffer, char *world_data);
char	*load_world(char *filename);
void	extract_json_value(char *json, char *key, char *result);
void	get_room_exit(char *json, char *room, char *dir, char *dest);
void	handle_quit(t_player *player, t_player *c);
void	handle_who(t_player *p, t_player *c);
void	handle_look(t_player *p, t_player *c, char *wld);
void	handle_move(t_player *p, t_player *c, char *buf, char *wld);
void	handle_inventory(t_player *player);
void	handle_take(t_player *player, int words, char *item, char *world);
int		check_item_in_room(char *json, char *room, char *item);
int		has_item(t_player *player, char *item);
int		check_npc_in_room(char *json, char *room, char *npc);
void	handle_attack(t_player *player, t_player *c, char *target, char *world);
void	give_item(t_player *player, char *item);
void	handle_defend(t_player *player, t_player *c, char *tgt, char *world);
void	handle_flee(t_player *player, t_player *c, char *world);
void	handle_drop(t_player *player, t_player *c,
			char *item, char *world_data);
void	handle_talk(t_player *player, int words, char *npc, char *world);
void	handle_status(t_player *player);
void	handle_quest(t_player *p, char *npc_name, char *wld);
void	handle_quests(t_player *p);
void	server_log(char *level, char *ip, char *player, char *msg);
int		check_abuse(t_player *player);
void	handle_chat(t_player *p, t_player *c, char *arg);
void	handle_group(t_player *p, t_player *c, char *arg);
void	handle_inventory(t_player *player);
void	broadcast_player_stats(t_player *c);
int		is_item_obtainable(char *wld, char *item);
int		get_npc_hp(char *wld, char *npc);
int		is_npc_enemy(char *wld, char *npc);
char	*manage_taken(int mode, char *item);
int		apply_damage_to_npc(char *room, char *npc, int dmg, char *wld);
void	get_npc_dialogue(char *wld, char *npc, char *out);
void	handle_death(t_player *p, t_player *c, char *wld);
void	get_room_desc(char *wld, char *room, char *out);

typedef struct s_drop
{
	char	room[32];
	char	item[32];
}	t_drop;

char	*manage_dropped(int mode, char *room, char *item, char *buf);

#endif