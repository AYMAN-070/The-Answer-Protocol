/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 10:10:37 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Prometheus Station - GUI Client");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(BLACK);
        render_main_layout();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}