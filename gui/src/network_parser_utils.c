/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 15:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 15:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>

void	route_message(char *line, t_ui_context *ctx)
{
	if (!strncmp(line, "NPC:", 4))
		add_message(&ctx->chat, line + 4);
	else if (!strncmp(line, "RCHAT:", 6))
		add_message(&ctx->chat, line + 6);
	else if (!strncmp(line, "GCHAT:", 6))
		add_message(&ctx->global_chat, line + 6);
	else if (!strncmp(line, "COMBAT:", 7))
		add_message(&ctx->logs, line + 7);
	else if (!strncmp(line, "EVENT:", 6))
		add_message(&ctx->logs, line + 6);
	else if (!strncmp(line, "DEAD:", 5))
		add_message(&ctx->logs, line + 5);
	else if (strncmp(line, "INVENTORY:", 10) && strncmp(line, "STATUS:", 7))
		add_message(&ctx->logs, line);
}
