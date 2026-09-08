/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:36 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 09:56:42 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
#include "network.h"
#include <string.h>

static void	set_button_colors(int is_hovered, Color *bg, Color *text)
{
	if (is_hovered)
	{
		*bg = Fade(LIGHTGRAY, 0.4f);
		*text = YELLOW;
	}
	else
	{
		*bg = Fade(BLACK, 0.6f);
		*text = WHITE;
	}
}

static void	draw_button(t_action_btn *btn, int sock_fd)
{
	int		is_hovered;
	Color	bg_color;
	Color	text_color;
	int		text_width;

	is_hovered = CheckCollisionPointRec(GetMousePosition(), btn->bounds);
	if (is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		net_send_command(sock_fd, btn->command);
	set_button_colors(is_hovered, &bg_color, &text_color);
	DrawRectangleRec(btn->bounds, bg_color);
	DrawRectangleLinesEx(btn->bounds, 1.0f, LIGHTGRAY);
	text_width = MeasureText(btn->label, 16);
	DrawText(btn->label,
		(int)(btn->bounds.x + (btn->bounds.width - text_width) / 2.0f),
		(int)(btn->bounds.y + 7), 16, text_color);
}

void	render_action_panel(Rectangle inv_panel, t_action_list *list,
			int sock_fd)
{
	Rectangle	panel;
	int			i;
	float		btn_width;

	panel.x = inv_panel.x;
	panel.y = inv_panel.y + (inv_panel.height / 2);
	panel.width = inv_panel.width;
	panel.height = inv_panel.height / 2;
	DrawRectangleRec(panel, Fade(DARKGRAY, 0.8f));
	DrawRectangleLinesEx(panel, 2.0f, LIGHTGRAY);
	DrawText("Available Actions", (int)panel.x + 15,
		(int)panel.y + 15, 20, WHITE);
	btn_width = (panel.width - 60.0f) / 3.0f;
	i = 0;
	while (i < list->count)
	{
		list->buttons[i].bounds.width = btn_width;
		list->buttons[i].bounds.height = 30;
		list->buttons[i].bounds.x = panel.x + 15 + (i % 3) * (btn_width + 15);
		list->buttons[i].bounds.y = panel.y + 45 + (i / 3) * 40;
		draw_button(&list->buttons[i], sock_fd);
		i++;
	}
}
