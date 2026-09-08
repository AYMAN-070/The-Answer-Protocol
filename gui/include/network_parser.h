/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 09:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_PARSER_H
# define NETWORK_PARSER_H

# include "cJSON.h"
# include "state_parser.h"

void	populate_state(cJSON *json, t_room_state *state);

#endif
