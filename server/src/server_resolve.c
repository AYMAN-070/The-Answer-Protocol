/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_resolve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 10:12:04 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/20 10:12:04 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	name_matches(char *entry, char *name)
{
	char	*p;
	char	*lim;
	char	*e;
	char	disp[64];

	p = strstr(entry, "\"name\": \"");
	lim = strstr(entry + 4, "\": {");
	if (!p || (lim && p > lim))
		return (0);
	p += 9;
	e = strchr(p, '"');
	if (!e || (e - p) > 63)
		return (0);
	strncpy(disp, p, e - p);
	disp[e - p] = '\0';
	return (strcmp(disp, name) == 0);
}

void	get_dialogue_line(char *p, int line, char *out)
{
	char	*e;

	out[0] = '\0';
	while (p)
	{
		p = strchr(p, '"');
		if (!p)
			return ;
		e = strchr(p + 1, '"');
		if (!e || (e - (p + 1)) >= 255)
			return ;
		if (line <= 0)
		{
			strncpy(out, p + 1, e - (p + 1));
			out[e - (p + 1)] = '\0';
			return ;
		}
		if (*(e + 1) != ',')
			return ;
		line--;
		p = e + 1;
	}
}

void	get_quest_reward(char *wld, char *quest, char *out)
{
	char	k[64];
	char	*p;
	char	*e;

	strcpy(out, "none");
	sprintf(k, "\"%s\": {", quest);
	p = strstr(wld, k);
	if (p)
		p = strstr(p, "\"reward\": \"");
	if (!p)
		return ;
	p += 11;
	e = strchr(p, '"');
	if (e && (e - p) < 31)
	{
		strncpy(out, p, e - p);
		out[e - p] = '\0';
	}
}

void	resolve_id(char *wld, char *sect, char *name, char *out)
{
	char	*p;
	char	*k;

	strncpy(out, name, 31);
	out[31] = '\0';
	p = strstr(wld, sect);
	if (!p)
		return ;
	p = strstr(p + 4, "\": {");
	while (p)
	{
		k = p;
		while (k > wld && *(k - 1) != '"')
			k--;
		if ((p - k) < 32 && name_matches(p, name))
		{
			strncpy(out, k, p - k);
			out[p - k] = '\0';
			return ;
		}
		p = strstr(p + 4, "\": {");
	}
}
