/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 13:26:49 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include "network.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void	load_local_world(t_ui_context *ctx)
{
	FILE	*file;
	long	length;
	char	*buffer;

	file = fopen("data/world.json", "rb");
	if (!file)
	{
		perror("Client error: Cannot open world.json");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = calloc(length + 1, sizeof(char));
	if (!buffer)
		exit(EXIT_FAILURE);
	fread(buffer, 1, length, file);
	fclose(file);
	ctx->world_db = cJSON_Parse(buffer);
	free(buffer);
	if (!ctx->world_db)
		exit(EXIT_FAILURE);
}

static int	init_client(t_game_assets *assets, t_ui_context *ctx)
{
	int	sock_fd;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Prometheus Station - GUI Client");
	SetTargetFPS(60);
	init_graphics_assets(assets);
	memset(&ctx->state, 0, sizeof(t_room_state));
	memset(&ctx->player, 0, sizeof(t_player_state));
	memset(&ctx->chat, 0, sizeof(t_message_buffer));
	memset(&ctx->global_chat, 0, sizeof(t_message_buffer));
	memset(&ctx->logs, 0, sizeof(t_message_buffer));
	ctx->chat_input[0] = '\0';
	ctx->chat_channel = 0;
	ctx->player.hp = 100;
	ctx->player.max_hp = 100;
	load_local_world(ctx);
	sock_fd = net_connect("127.0.0.1", 4242);
	if (sock_fd >= 0)
		net_send_command(sock_fd, "CONNECT gui_user\n");
	return (sock_fd);
}

static void	run_client(t_game_assets *assets, t_ui_context *ctx)
{
	while (!WindowShouldClose())
	{
		net_receive_state(ctx->sock_fd, ctx);
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
	run_client(&assets, &ctx);
	net_disconnect(&ctx.sock_fd);
	unload_graphics_assets(&assets);
	if (ctx.world_db)
		cJSON_Delete(ctx.world_db);
	CloseWindow();
	return (0);
}
