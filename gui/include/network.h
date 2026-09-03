/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 08:07:44 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 08:21:46 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

# include "state_parser.h"

int		net_connect(const char *ip, int port);
void	net_send_command(int sock_fd, const char *cmd);
int		net_receive_state(int sock_fd, t_room_state *state);
void	net_disconnect(int *sock_fd);

#endif