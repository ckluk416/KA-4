#include "raylib.h"
#include "src/firework.h"
#include <stdlib.h>
#include <time.h>

#define NUM_FIREWORKS 4

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Tubes ETS 241524041");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    Firework fireworks[NUM_FIREWORKS];
    
    float spawn_delays[NUM_FIREWORKS];
    
    for (int i = 0; i < NUM_FIREWORKS; i++) {
        fireworks[i].stage = DONE; 
        spawn_delays[i] = (float)(rand() % 400) / 100.0f; 
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage == DONE) {
                if (spawn_delays[i] > 0) {
                    spawn_delays[i] -= dt;
                } else {
                    InitFirework(&fireworks[i], screenWidth, screenHeight);
                    spawn_delays[i] = (float)(rand() % 300 + 100) / 100.0f; 
                }
            } else {
                UpdateFirework(&fireworks[i], dt);
            }
        }

        BeginDrawing();
        ClearBackground((Color){8, 8, 16, 255});

        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage != DONE) {
                DrawFirework(&fireworks[i]);
            }
        }
        
        DrawText("TUBES UTS 241524041", 20, 20, 20, RAYWHITE);
        
        int total_particles = 0;
        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage == EXPLODE) {
                for (int j = 0; j < NUM_PARTICLES; j++) {
                    if (fireworks[i].particles[j].active) total_particles++;
                }
            }
        }
        // DrawText(TextFormat("active Particles: %d", total_particles), 20, 65, 12, LIGHTGRAY);
        DrawFPS(screenWidth - 100, 20);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
