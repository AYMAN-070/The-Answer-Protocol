/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_json.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 11:07:12 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 20:36:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	extract_json_value(char *json, char *key, char *result)
{
	char	*pos;
	char	*start;
	char	*end;
	int		len;

	pos = strstr(json, key);
	if (!pos)
	{
		result[0] = '\0';
		return ;
	}
	start = strchr(pos + strlen(key), '"');
	if (!start)
		return ;
	start++;
	end = strchr(start, '"');
	if (!end)
		return ;
	len = end - start;
	strncpy(result, start, len);
	result[len] = '\0';
}

void	get_room_exit(char *json, char *room, char *dir, char *dest)
{
	char	*pos;
	char	*limit;
	char	key[64];

	dest[0] = '\0';
	sprintf(key, "\"%s\": {", room);
	pos = strstr(json, key);
	if (!pos)
		return ;
	limit = strstr(pos, "\"items\":");
	sprintf(key, "\"%s\": \"", dir);
	pos = strstr(pos, key);
	if (!pos || pos > limit)
		return ;
	pos += strlen(key);
	limit = strchr(pos, '"');
	if (limit)
	{
		strncpy(dest, pos, limit - pos);
		dest[limit - pos] = '\0';
	}
}

int	check_item_in_room(char *json, char *room, char *item)
{
	char	*pos;
	char	*limit;
	char	key[64];

	sprintf(key, "\"%s\": {", room);
	pos = strstr(json, key);
	if (!pos)
		return (0);
	limit = strstr(pos, "\"spawns\":");
	sprintf(key, "\"%s\"", item);
	pos = strstr(pos, key);
	if (pos && (!limit || pos < limit))
		return (1);
	return (0);
}

int	check_npc_in_room(char *json, char *room, char *npc)
{
	char	*pos;
	char	*limit;
	char	key[64];

	sprintf(key, "\"%s\": {", room);
	pos = strstr(json, key);
	if (!pos)
		return (0);
	pos = strstr(pos, "\"spawns\":");
	if (!pos)
		return (0);
	limit = strchr(pos, ']');
	sprintf(key, "\"%s\"", npc);
	pos = strstr(pos, key);
	if (pos && limit && pos < limit)
		return (1);
	return (0);
}

void	get_room_desc(char *wld, char *room, char *out)
{
	char	k[64];
	char	*p;
	char	*e;

	sprintf(k, "\"%s\": {", room);
	p = strstr(wld, k);
	if (p)
		p = strstr(p, "\"description\": \"");
	if (p)
	{
		e = strchr(p + 16, '"');
		if (e && (e - (p + 16)) < 255)
		{
			strncpy(out, p + 16, e - (p + 16));
			out[e - (p + 16)] = '\0';
			return ;
		}
	}
	strcpy(out, "Description indisponible.");
}
