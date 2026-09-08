/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 18:40:09 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int	main(void)
{
	t_room_assets	assets;
	int				sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd != -1)
	{
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(4242);

		if (connect(sockfd, (struct	sockaddr*)&servaddr, sizeof(servaddr)) == 0)
		{
			printf("INFO RÉSEAU : GUI connecté au serveur !\n");
			write(sockfd, "CONNECT raylib_player\n", 22);
		}
		else
			printf("ERREUR RÉSEAU : Serveur introuvable.\n");
	}

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "Prometheus Station - GUI Client");
	SetTargetFPS(60);
	init_graphics_assets(&assets);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		render_main_layout(&assets);
		EndDrawing();
	}
	unload_graphics_assets(&assets);
	CloseWindow();
	return (0);
}
