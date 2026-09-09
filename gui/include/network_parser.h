/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:25:04 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_PARSER_H
# define NETWORK_PARSER_H

# include "cJSON.h"
# include "state_parser.h"

void	parse_server_text(char *buffer, t_ui_context *ctx);

#endif
