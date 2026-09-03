/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:36 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 08:02:57 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
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

static void	draw_button(t_action_btn *btn)
{
	int		is_hovered;
	Color	bg_color;
	Color	text_color;
	int		text_width;

	is_hovered = CheckCollisionPointRec(GetMousePosition(), btn->bounds);
	if (is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		TraceLog(LOG_INFO, "NETWORK COMMAND READY: %s", btn->command);
	set_button_colors(is_hovered, &bg_color, &text_color);
	DrawRectangleRec(btn->bounds, bg_color);
	DrawRectangleLinesEx(btn->bounds, 1.0f, LIGHTGRAY);
	text_width = MeasureText(btn->label, 20);
	DrawText(btn->label,
		(int)(btn->bounds.x + (btn->bounds.width - text_width) / 2.0f),
		(int)(btn->bounds.y + 8), 20, text_color);
}

void	render_action_panel(Rectangle inv_panel, t_action_list *list)
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
	btn_width = (panel.width - 45.0f) / 2.0f;
	i = 0;
	while (i < list->count)
	{
		list->buttons[i].bounds.width = btn_width;
		list->buttons[i].bounds.height = 35;
		list->buttons[i].bounds.x = panel.x + 15 + (i % 2) * (btn_width + 15);
		list->buttons[i].bounds.y = panel.y + 50 + (i / 2) * 45;
		draw_button(&list->buttons[i]);
		i++;
	}
}
