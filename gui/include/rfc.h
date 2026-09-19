/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rfc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 09:00:39 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/16 09:01:24 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RFC_H
# define RFC_H

# include "cJSON.h"
# include "layout.h"

int		rfc_route_line(char *line, t_ui_context *ctx);
void	rfc_apply_json(char *json, t_ui_context *ctx);
void	rfc_apply_status(cJSON *root, t_ui_context *ctx);
void	rfc_apply_combat(cJSON *root, t_ui_context *ctx);
void	rfc_apply_quest(cJSON *root, t_ui_context *ctx);
void	rfc_apply_list(cJSON *root, t_ui_context *ctx);
void	update_room_items(t_ui_context *ctx, const char *id, int taken);
void	remove_room_npc(t_ui_context *ctx, const char *id);
void	handle_item_event(t_ui_context *ctx, char *data);
void	handle_talk_response(char *data, t_ui_context *ctx);

#endif
