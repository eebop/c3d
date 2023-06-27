#ifndef __GFX_H
#define __GFX_H
#include "SDL2/SDL.h"

typedef struct {
    double x;
    double y;
    double z;
} point3;

typedef struct {
    double cx;
    double cy;
    double cz;
    double a1;
    double a2;
    double fov;
} camera;

typedef struct {
    int *p;
    int np;
    SDL_Color c;
} texture;

typedef struct {
    camera *c;
    point3 **points;
    uint num_points;
    uint max_points;
    texture **textures;
    uint num_textures;
    uint max_textures;
} scene;


scene *alloc_scene(void);
//int compute_one(point3 *p, camera *c, SDL_FPoint *op); // not needed as it is private
void submit_pt(scene *s, point3 *p);
void submit_txt(scene *s, texture *t);
void render(SDL_Renderer *r, scene *s, uint type);
void scene_comp(scene *s);
#endif