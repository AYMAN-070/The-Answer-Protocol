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

static int	init_client(t_game_assets *assets, t_ui_context *ctx,
				const char *name)
{
	int		sock_fd;
	char	title[64];
	char	cmd[64];

	snprintf(title, sizeof(title), "Prometheus Station - %s", name);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, title);
	SetTargetFPS(60);
	init_graphics_assets(assets);
	memset(ctx, 0, sizeof(t_ui_context));
	ctx->player.hp = 100;
	ctx->player.max_hp = 100;
	load_local_world(ctx);
	sock_fd = net_connect("127.0.0.1", 4242);
	if (sock_fd >= 0)
	{
		ctx->connected = 1;
		snprintf(cmd, sizeof(cmd), "CONNECT %s\n", name);
		net_send_command(sock_fd, cmd);
		net_send_command(sock_fd, "WHO\n");
	}
	return (sock_fd);
}

static void	run_client(t_game_assets *assets, t_ui_context *ctx)
{
	while (!WindowShouldClose())
	{
		if (net_receive_state(ctx->sock_fd, ctx) < 0)
		{
			ctx->connected = 0;
			net_disconnect(&ctx->sock_fd);
		}
		BeginDrawing();
		ClearBackground(BLACK);
		render_main_layout(assets, ctx);
		EndDrawing();
	}
}

int	main(int argc, char **argv)
{
	t_game_assets	assets;
	t_ui_context	ctx;
	const char		*name;

	name = "gui_user";
	if (argc > 1)
		name = argv[1];
	ctx.sock_fd = init_client(&assets, &ctx, name);
	run_client(&assets, &ctx);
	net_disconnect(&ctx.sock_fd);
	unload_graphics_assets(&assets);
	if (ctx.world_db)
		cJSON_Delete(ctx.world_db);
	CloseWindow();
	return (0);
}
