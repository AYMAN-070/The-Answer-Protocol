/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:29:01 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_PARSER_H
# define NETWORK_PARSER_H

# include "cJSON.h"
# include "layout.h"

void	parse_server_text(char *buffer, t_ui_context *ctx);
void	resolve_display_name(cJSON *defs, const char *id, char *out);
void	parse_exits(cJSON *exits_obj, t_room_state *state);
void	confirm_who(t_ui_context *ctx, int count, const char *raw);
void	confirm_inventory(t_ui_context *ctx);
int		quests_changed(t_ui_context *ctx, cJSON *root);

#endif
