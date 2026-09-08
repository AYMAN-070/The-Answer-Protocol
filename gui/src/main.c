/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 10:01:27 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include "network.h"
#include "state_parser.h"
#include <string.h>

static int	init_client(t_game_assets *assets, t_room_state *state,
				t_action_list *actions)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Prometheus Station - GUI Client");
	SetTargetFPS(60);
	init_graphics_assets(assets);
	memset(state, 0, sizeof(t_room_state));
	actions->count = 0;
	return (net_connect("127.0.0.1", 4242));
}

static void	mock_player(t_player_state *player)
{
	memset(player, 0, sizeof(t_player_state));
	player->max_hp = 100;
	player->hp = 80;
	player->inv_count = 2;
	strncpy(player->inventory[0].id, "red_keycard", 31);
	strncpy(player->inventory[0].name, "Red Keycard", 31);
	strncpy(player->inventory[1].id, "energy_cell", 31);
	strncpy(player->inventory[1].name, "Energy Cell", 31);
}

static void	run_client(t_game_assets *assets, t_ui_context *ctx)
{
	while (!WindowShouldClose())
	{
		if (net_receive_state(ctx->sock_fd, ctx->state))
			build_actions_from_state(ctx->actions, ctx->state, ctx->player);
		BeginDrawing();
		ClearBackground(BLACK);
		render_main_layout(assets, ctx);
		EndDrawing();
	}
}

int	main(void)
{
	t_game_assets	assets;
	t_room_state	current_state;
	t_action_list	actions;
	t_player_state	player;
	t_ui_context	ctx;

	ctx.sock_fd = init_client(&assets, &current_state, &actions);
	mock_player(&player);
	ctx.state = &current_state;
	ctx.player = &player;
	ctx.actions = &actions;
	run_client(&assets, &ctx);
	net_disconnect(&ctx.sock_fd);
	unload_graphics_assets(&assets);
	CloseWindow();
	return (0);
}
