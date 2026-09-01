/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:30:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/01 10:32:29 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*load_world(char *filename)
{
	FILE	*file;
	long	length;
	char	*buffer;

	file = fopen(filename, "rb");
	if (!file)
		exit_error("Error : can't open world.json");

	fseek(file, 0, SEEK_END);
	length = ftell(file);

	fseek(file, 0, SEEK_SET);

	buffer = malloc(length + 1);
	if (!buffer)
		exit_error("Error : malloc failed for world_data");

	fread(buffer, 1, length, file);
	buffer[length] = '\0';
	fclose(file);

	return (buffer);
}