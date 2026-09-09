/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:25:59 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include "network.h"
#include "state_parser.h"
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
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Prometheus Station - GUI Client");
	SetTargetFPS(60);
	init_graphics_assets(assets);
	memset(&ctx->state, 0, sizeof(t_room_state));
	ctx->actions.count = 0;
	
	load_local_world(ctx);

	fd = net_connect("127.0.0.1", 4242);
	if (fd >= 0)
		net_send_command(fd, "CONNECT gui_user\n");
	add_action(&ctx->actions, "LOOK", "LOOK\n");
	return (fd);
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
	
	if (ctx.world_db)
		cJSON_Delete(ctx.world_db);
		
	CloseWindow();
	return (0);
}
