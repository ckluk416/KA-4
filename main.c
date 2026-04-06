#include "raylib.h"
#include "src/firework.h"
#include "src/algo/bresenham.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_FIREWORKS 4
#define NUM_STARS 150

typedef struct {
    Vector2 pos;
    float radius;
    float alpha;
    float blinkRate;
} Star;

float RandFloat(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

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
        spawn_delays[i] = RandFloat(0.0f, 4.0f); 
    }

    Star stars[NUM_STARS];
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].pos = (Vector2){ RandFloat(0, screenWidth), RandFloat(0, screenHeight * 0.7f) };
        stars[i].radius = RandFloat(0.5f, 1.5f);
        stars[i].alpha = RandFloat(0.1f, 1.0f);
        stars[i].blinkRate = RandFloat(1.0f, 3.0f);
    }

    float timeScale = 1.0f;
    bool wireframeMode = false;
    ExplosionType currentExplosionType = EXPLOSION_NORMAL;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) timeScale += 0.25f;
        if (IsKeyPressed(KEY_DOWN)) timeScale -= 0.25f;
        if (timeScale < 0.1f) timeScale = 0.1f;
        if (timeScale > 5.0f) timeScale = 5.0f;

        if (IsKeyPressed(KEY_W)) wireframeMode = !wireframeMode;
        if (IsKeyPressed(KEY_E)) {
            if (currentExplosionType == EXPLOSION_NORMAL) currentExplosionType = EXPLOSION_WILLOW;
            else if (currentExplosionType == EXPLOSION_WILLOW) currentExplosionType = EXPLOSION_PISTIL;
            else if (currentExplosionType == EXPLOSION_PISTIL) currentExplosionType = EXPLOSION_CHRYSANTHEMUM;
            else currentExplosionType = EXPLOSION_NORMAL;
        }

        float dt = GetFrameTime() * timeScale;

        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage == DONE) {
                if (spawn_delays[i] > 0) {
                    spawn_delays[i] -= dt;
                } else {
                    InitFirework(&fireworks[i], screenWidth, screenHeight, currentExplosionType);
                    spawn_delays[i] = RandFloat(1.0f, 4.0f); 
                }
            } else {
                UpdateFirework(&fireworks[i], dt);
            }
        }

        BeginDrawing();
        ClearBackground((Color){8, 8, 16, 255});

        for (int i = 0; i < NUM_STARS; i++) {
            float a = stars[i].alpha + sinf(GetTime() * stars[i].blinkRate) * 0.5f;
            if (a < 0.0f) a = 0.0f;
            if (a > 1.0f) a = 1.0f;
            Color starColor = ColorAlpha(WHITE, a);
            MidpointCircleFill((int)stars[i].pos.x, (int)stars[i].pos.y, (int)stars[i].radius, starColor);
        }

        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage != DONE) {
                DrawFirework(&fireworks[i], wireframeMode);
            }
        }
        
        DrawText("TUBES UTS 241524041", 20, 20, 20, RAYWHITE);
        DrawText(TextFormat("Speed: %.2fx (UP/DOWN)", timeScale), 20, 45, 12, LIGHTGRAY);
        DrawText(TextFormat("Wireframe Mode: %s (W)", wireframeMode ? "ON" : "OFF"), 20, 60, 12, LIGHTGRAY);
        const char *expTypeStr = "Normal";
        if (currentExplosionType == EXPLOSION_WILLOW) expTypeStr = "Willow";
        else if (currentExplosionType == EXPLOSION_PISTIL) expTypeStr = "Pistil";
        else if (currentExplosionType == EXPLOSION_CHRYSANTHEMUM) expTypeStr = "Chrysanthemum";
        DrawText(TextFormat("Explosion Type: %s (E)", expTypeStr), 20, 75, 12, LIGHTGRAY);
        
        int total_particles = 0;
        for (int i = 0; i < NUM_FIREWORKS; i++) {
            if (fireworks[i].stage == EXPLODE) {
                for (int j = 0; j < NUM_PARTICLES; j++) {
                    if (fireworks[i].particles[j].active) total_particles++;
                }
            }
        }
        DrawText(TextFormat("active Particles: %d", total_particles), 20, 90, 12, LIGHTGRAY);
        DrawFPS(screenWidth - 100, 20);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
