/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 11:06:52 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/20 11:06:52 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	print_lines(char *buf, int *len)
{
	char	*nl;

	nl = strchr(buf, '\n');
	while (nl)
	{
		*nl = '\0';
		if (buf[0] != '\0')
			printf("\rServeur : %s\n", buf);
		*len -= (nl - buf) + 1;
		memmove(buf, nl + 1, *len + 1);
		nl = strchr(buf, '\n');
	}
	printf("> ");
	fflush(stdout);
}
