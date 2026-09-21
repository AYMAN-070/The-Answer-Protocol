/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 21:44:42 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 19:43:38 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	is_item_obtainable(char *wld, char *item)
{
	char	key[64];
	char	*p;

	sprintf(key, "\"%s\": {", item);
	p = strstr(wld, key);
	if (p)
	{
		p = strstr(p, "\"obtainable\": ");
		if (p && strncmp(p + 14, "true", 4) == 0)
			return (1);
	}
	return (0);
}

int	get_npc_hp(char *wld, char *npc)
{
	char	key[64];
	char	*p;

	sprintf(key, "\"%s\": {", npc);
	p = strstr(wld, key);
	if (p)
	{
		p = strstr(p, "\"hp\": ");
		if (p)
			return (atoi(p + 6));
	}
	return (50);
}

int	is_npc_enemy(char *wld, char *npc)
{
	char	k[64];
	char	*p;

	sprintf(k, "\"%s\": {", npc);
	p = strstr(wld, k);
	if (p)
	{
		p = strstr(p, "\"role\": \"");
		if (p && strncmp(p + 9, "enemy", 5) == 0)
			return (1);
	}
	return (0);
}

int	apply_damage_to_npc(char *room, char *npc, int dmg, char *wld)
{
	static char	n[20][32];
	static char	r[20][32];
	static int	h[20];
	int			i;

	i = -1;
	while (++i < 20)
	{
		if (n[i][0] == '\0')
		{
			strncpy(n[i], npc, 31);
			strncpy(r[i], room, 31);
			h[i] = get_npc_hp(wld, npc);
		}
		if (strcmp(n[i], npc) == 0 && strcmp(r[i], room) == 0)
		{
			h[i] -= dmg;
			return (h[i]);
		}
	}
	return (0);
}

void	get_npc_dialogue(char *wld, char *npc, char *out)
{
	static int	turn;
	char		k[64];
	char		*p;

	out[0] = '\0';
	sprintf(k, "\"%s\": {", npc);
	p = strstr(wld, k);
	if (p)
		p = strstr(p, "\"dialogue\": [");
	if (p)
		get_dialogue_line(p + 13, turn % 2, out);
	if (p && out[0] == '\0')
		get_dialogue_line(p + 13, 0, out);
	turn++;
	if (out[0] == '\0')
		strcpy(out, "*Silence*");
}
