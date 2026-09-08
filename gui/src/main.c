/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 13:17:07 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include "network.h"
#include "state_parser.h"
#include <string.h>

static int	init_client(t_game_assets *assets, t_ui_context *ctx)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Prometheus Station - GUI Client");
	SetTargetFPS(60);
	init_graphics_assets(assets);
	memset(&ctx->state, 0, sizeof(t_room_state));
	ctx->actions.count = 0;
	return (net_connect("127.0.0.1", 4242));
}

static void	mock_data(t_ui_context *ctx)
{
	memset(&ctx->player, 0, sizeof(t_player_state));
	ctx->player.max_hp = 100;
	ctx->player.hp = 80;
	ctx->player.inv_count = 2;
	strncpy(ctx->player.inventory[0].id, "red_keycard", 31);
	strncpy(ctx->player.inventory[0].name, "Red Keycard", 31);
	strncpy(ctx->player.inventory[1].id, "energy_cell", 31);
	strncpy(ctx->player.inventory[1].name, "Energy Cell", 31);
	
	ctx->state.room_players = 2;
	ctx->state.server_players = 5;

	strncpy(ctx->state.current_npc, "Station AI", 63);
	strncpy(ctx->state.current_dialogue, "Welcome to Prometheus Station. Please proceed to the airlock.", 255);

	ctx->chat.count = 2;
	strncpy(ctx->chat.messages[0], "[Global] Alice: Server is up!", 127);
	strncpy(ctx->chat.messages[1], "[Room] Bob: I need a keycard.", 127);
	ctx->logs.count = 3;
	strncpy(ctx->logs.messages[0], "> CONNECT alice", 127);
	strncpy(ctx->logs.messages[1], "> OK connected", 127);
	strncpy(ctx->logs.messages[2], "> EVT ROOM ENTER bob", 127);
}

static void	run_client(t_game_assets *assets, t_ui_context *ctx)
{
	while (!WindowShouldClose())
	{
		if (net_receive_state(ctx->sock_fd, &ctx->state))
			build_actions_from_state(&ctx->actions, &ctx->state, &ctx->player);
		BeginDrawing();
		ClearBackground(BLACK);
		render_main_layout(assets, ctx);
		EndDrawing();
	}
}

int	main(void)
{
	t_game_assets	assets;
	t_ui_context	ctx;

	ctx.sock_fd = init_client(&assets, &ctx);
	mock_data(&ctx);
	run_client(&assets, &ctx);
	net_disconnect(&ctx.sock_fd);
	unload_graphics_assets(&assets);
	CloseWindow();
	return (0);
}
