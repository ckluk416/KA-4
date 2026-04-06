#include "firework.h"
#include "algo/bresenham.h"
#include <stdlib.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static float RandomFloat(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

void InitFirework(Firework *f, int screenW, int screenH, ExplosionType type) {
    f->explosion_type = type;
    f->launch_pos = (Vector2){ RandomFloat(100, screenW - 100), (float)screenH };
    f->target_pos = (Vector2){ f->launch_pos.x + RandomFloat(-150, 150), RandomFloat(100, screenH / 2.0f) };
    
    float launch_time = 1.5f;
    f->launch_vel.x = (f->target_pos.x - f->launch_pos.x) / launch_time;
    float gravity = 150.0f;
    f->launch_vel.y = ((f->target_pos.y - f->launch_pos.y) - 0.5f * gravity * launch_time * launch_time) / launch_time;
    
    Color colors[] = { RED, GOLD, (Color){255,105,180,255}, LIME, VIOLET, ORANGE, SKYBLUE };
    f->core_color = colors[rand() % 7];
    
    f->stage = LAUNCH;
    f->active_particles = 0;
    
    f->launch_history_index = 0;
    f->launch_history_count = 0;
    for(int i = 0; i < TRAIL_LENGTH * 2; i++) {
        f->launch_history[i] = f->launch_pos;
    }
}

static void ExplodeFirework(Firework *f) {
    f->stage = EXPLODE;
    f->active_particles = NUM_PARTICLES;
    
    Color pistilColor = WHITE;
    if (f->explosion_type == EXPLOSION_PISTIL) {
        Color colors[] = { MAGENTA, SKYBLUE, YELLOW, GREEN, PINK, WHITE };
        pistilColor = colors[rand() % 6];
    }
    
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &f->particles[i];
        p->pos = f->launch_pos;
        
        float theta = RandomFloat(0, 2.0f * PI);
        float phi = acosf(RandomFloat(-1.0f, 1.0f));
        float speed = 0.0f;
        
        if (f->explosion_type == EXPLOSION_WILLOW) {
            f->core_color = GOLD;
            speed = RandomFloat(50.0f, 200.0f);
            if (rand() % 100 < 15) {
                speed = RandomFloat(150.0f, 200.0f); 
            }
            p->color = f->core_color;
            p->max_life = RandomFloat(3.0f, 5.0f); 
        } else if (f->explosion_type == EXPLOSION_PISTIL) {
            if (i < NUM_PARTICLES * 0.75f) { 
                speed = RandomFloat(200.0f, 350.0f);
                p->color = f->core_color;
                p->max_life = RandomFloat(1.5f, 2.5f);
            } else { 
                speed = RandomFloat(30.0f, 100.0f);
                p->color = pistilColor;
                p->max_life = RandomFloat(2.0f, 3.5f);
            }
        } else if (f->explosion_type == EXPLOSION_CHRYSANTHEMUM) {
            speed = RandomFloat(200.0f, 250.0f); 
            if (rand() % 100 < 15) {
                p->color = WHITE;
            } else {
                p->color = f->core_color;
            }
            p->max_life = RandomFloat(2.0f, 2.5f);
        } else { 
            speed = RandomFloat(50.0f, 300.0f);
            if (rand() % 100 < 15) {
                speed = RandomFloat(250.0f, 280.0f); 
                p->color = WHITE;
            } else {
                p->color = f->core_color;
            }
            p->max_life = RandomFloat(1.5f, 3.5f);
        }
        
        p->vel.x = speed * sinf(phi) * cosf(theta);
        p->vel.y = speed * sinf(phi) * sinf(theta);
        
        p->life = 0;
        p->active = true;
        
        p->history_index = 0;
        p->history_count = 0;
        for(int j=0; j<TRAIL_LENGTH; j++) p->history[j] = p->pos;
    }
}

void UpdateFirework(Firework *f, float dt) {
    if (f->stage == DONE) return;
    
    if (f->stage == LAUNCH) {
        Vector2 historyPos = {
            f->launch_pos.x + RandomFloat(-1.5f, 1.5f),
            f->launch_pos.y + RandomFloat(-1.5f, 1.5f)
        };
        f->launch_history[f->launch_history_index] = historyPos;
        f->launch_history_index = (f->launch_history_index + 1) % (TRAIL_LENGTH * 2);
        if (f->launch_history_count < TRAIL_LENGTH * 2) f->launch_history_count++;
        
        float gravity = 150.0f;
        f->launch_vel.y += gravity * dt;
        f->launch_pos.x += f->launch_vel.x * dt;
        f->launch_pos.y += f->launch_vel.y * dt;
        
        if (f->launch_vel.y >= 0 || f->launch_pos.y <= f->target_pos.y) {
            ExplodeFirework(f);
        }
    } else if (f->stage == EXPLODE) {
        bool any_active = false;
        
        for (int i = 0; i < NUM_PARTICLES; i++) {
            Particle *p = &f->particles[i];
            if (!p->active) continue;
            
            any_active = true;
            p->life += dt;
            
            if (p->life >= p->max_life) {
                p->active = false;
                continue;
            }
            
            p->history[p->history_index] = p->pos;
            p->history_index = (p->history_index + 1) % TRAIL_LENGTH;
            if (p->history_count < TRAIL_LENGTH) p->history_count++;
            
            p->vel.y += 100.0f * dt;
            if (f->explosion_type == EXPLOSION_WILLOW) {
                p->vel.y += 50.0f * dt; 
                p->vel.x *= 0.95f;      
                p->vel.y *= 0.98f;
            } else {
                p->vel.x *= 0.98f;    
                p->vel.y *= 0.98f;       
            }
            
            p->vel.x += 10.0f * dt;
            
            p->pos.x += p->vel.x * dt;
            p->pos.y += p->vel.y * dt;
        }
        
        if (!any_active) {
            f->stage = DONE;
        }
    }
}

void DrawFirework(Firework *f, bool wireframe) {
    if (f->stage == LAUNCH) {
        int count = f->launch_history_count;
        if (count > 1) {
            for (int i = 0; i < count - 1; i++) {
                int idx1 = (f->launch_history_index - 1 - i + (TRAIL_LENGTH * 2)) % (TRAIL_LENGTH * 2);
                int idx2 = (f->launch_history_index - 2 - i + (TRAIL_LENGTH * 2)) % (TRAIL_LENGTH * 2);
                
                float alpha = 1.0f - ((float)i / count);
                Color trailColor = ORANGE; 
                Color c = ColorAlpha(trailColor, alpha);
                
                int x1 = (int)f->launch_history[idx1].x;
                int y1 = (int)f->launch_history[idx1].y;
                int x2 = (int)f->launch_history[idx2].x;
                int y2 = (int)f->launch_history[idx2].y;
                
                BresenhamLine(x1, y1, x2, y2, c);
                if (!wireframe && rand() % 2 == 0) {
                    DrawPixel(x1 + (rand() % 5 - 2), y1 + (rand() % 5 - 2), c);
                }
            }
            int latest = (f->launch_history_index - 1 + (TRAIL_LENGTH * 2)) % (TRAIL_LENGTH * 2);
            BresenhamLine((int)f->launch_history[latest].x, (int)f->launch_history[latest].y,
                          (int)f->launch_pos.x, (int)f->launch_pos.y, ORANGE);
        }
        
        if (wireframe) {
            MidpointCircle((int)f->launch_pos.x, (int)f->launch_pos.y, 4, f->core_color);
        } else {
            MidpointCircleFill((int)f->launch_pos.x, (int)f->launch_pos.y, 4, f->core_color);
        }
    } 
    else if (f->stage == EXPLODE) {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            Particle *p = &f->particles[i];
            if (!p->active) continue;
            
            float life_pct = p->life / p->max_life;
            float alpha = 1.0f - (life_pct * life_pct); 
            
            Color pColor = p->color;
            if (life_pct < 0.2f) {
                pColor = WHITE;
            }
            
            int count = p->history_count;
            if (count > 0) {
                for (int j = 0; j < count; j++) {
                    int idx = (p->history_index - 1 - j + TRAIL_LENGTH) % TRAIL_LENGTH;
                    float trail_alpha = alpha * (1.0f - ((float)j / count));
                    Color tColor = ColorAlpha(pColor, trail_alpha);
                    
                    DrawPixel((int)p->history[idx].x, (int)p->history[idx].y, tColor);
                    if (!wireframe && j < 2) {
                        DrawPixel((int)p->history[idx].x + 1, (int)p->history[idx].y, tColor);
                        DrawPixel((int)p->history[idx].x, (int)p->history[idx].y + 1, tColor);
                    }
                }
            }
            
            if (wireframe) {
                DrawRectBresenham((int)p->pos.x - 1, (int)p->pos.y - 1, 3, 3, ColorAlpha(pColor, alpha));
            } else {
                DrawPixel((int)p->pos.x, (int)p->pos.y, ColorAlpha(pColor, alpha));
                DrawPixel((int)p->pos.x + 1, (int)p->pos.y, ColorAlpha(pColor, alpha));
                DrawPixel((int)p->pos.x, (int)p->pos.y + 1, ColorAlpha(pColor, alpha));
            }
        }
    }
}
