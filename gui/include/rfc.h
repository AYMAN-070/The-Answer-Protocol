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
void	rfc_apply_room(cJSON *root, t_ui_context *ctx);
void	rfc_apply_status(cJSON *root, t_ui_context *ctx);
void	rfc_apply_combat(cJSON *root, t_ui_context *ctx);
void	rfc_apply_quest(cJSON *root, t_ui_context *ctx);
void	rfc_apply_list(cJSON *root, t_ui_context *ctx);
void	rfc_apply_inventory(cJSON *arr, t_ui_context *ctx);

#endif
