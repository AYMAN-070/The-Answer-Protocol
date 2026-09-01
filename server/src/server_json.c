/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_json.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 11:07:12 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/01 11:09:45 by ayhammou         ###   ########.fr       */
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
