/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 09:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include <string.h>

static void	set_display_name(const char *id, char *dest)
{
	if (strcmp(id, "red_keycard") == 0)
		strncpy(dest, "Red Keycard", 31);
	else if (strcmp(id, "laser_pistol") == 0)
		strncpy(dest, "Laser Pistol", 31);
	else if (strcmp(id, "energy_cell") == 0)
		strncpy(dest, "Energy Cell", 31);
	else if (strcmp(id, "sec_drone") == 0)
		strncpy(dest, "Security Drone", 31);
	else if (strcmp(id, "injured_engineer") == 0)
		strncpy(dest, "Injured Engineer", 31);
	else if (strcmp(id, "alpha_drone") == 0)
		strncpy(dest, "Alpha Drone", 31);
	else
		strncpy(dest, id, 31);
}

static void	parse_entities(cJSON *array, t_entity *dest, int *count)
{
	cJSON	*item;

	*count = 0;
	if (!cJSON_IsArray(array))
		return ;
	item = array->child;
	while (item != NULL && *count < 5)
	{
		if (cJSON_IsString(item))
		{
			strncpy(dest[*count].id, item->valuestring, 31);
			set_display_name(item->valuestring, dest[*count].name);
			(*count)++;
		}
		item = item->next;
	}
}

static void	parse_exits(cJSON *tmp, t_room_state *state)
{
	cJSON	*ext;

	if (!cJSON_IsArray(tmp))
		return ;
	ext = tmp->child;
	while (ext != NULL && state->exit_count < 4)
	{
		if (cJSON_IsString(ext))
			strncpy(state->exits[state->exit_count++],
				ext->valuestring, 15);
		ext = ext->next;
	}
}

void	populate_state(cJSON *json, t_room_state *state)
{
	cJSON	*tmp;

	memset(state, 0, sizeof(t_room_state));
	tmp = cJSON_GetObjectItem(json, "room_name");
	if (cJSON_IsString(tmp))
		strncpy(state->name, tmp->valuestring, 63);
	tmp = cJSON_GetObjectItem(json, "description");
	if (cJSON_IsString(tmp))
		strncpy(state->description, tmp->valuestring, 255);
	parse_exits(cJSON_GetObjectItem(json, "exits"), state);
	parse_entities(cJSON_GetObjectItem(json, "items"), state->items,
		&state->item_count);
	parse_entities(cJSON_GetObjectItem(json, "npcs"), state->npcs,
		&state->npc_count);
}
