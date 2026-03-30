#ifndef FIREWORK_H
#define FIREWORK_H

#include "raylib.h"
#include <stdbool.h>

#define TRAIL_LENGTH 8
#define NUM_PARTICLES 1500

typedef enum { 
    LAUNCH, 
    EXPLODE, 
    DONE
} FireworkStage;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float life;
    float max_life;
    Color color;
    Vector2 history[TRAIL_LENGTH];
    int history_index;
    int history_count;
    bool active;
} Particle;

typedef struct {
    Vector2 launch_pos;
    Vector2 target_pos;
    Vector2 launch_vel;
    Color core_color;
    FireworkStage stage;
    Particle particles[NUM_PARTICLES];
    int active_particles;
    
    Vector2 launch_history[TRAIL_LENGTH * 2];
    int launch_history_index;
    int launch_history_count;
} Firework;

void InitFirework(Firework *f, int screenW, int screenH);
void UpdateFirework(Firework *f, float dt);
void DrawFirework(Firework *f);

#endif
