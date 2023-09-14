#ifndef __GFX_H
#define __GFX_H
#include "SDL2/SDL.h"
#include "quaternion.h"
/*
typedef struct {
    double x;
    double y;
    double z;
} point3;
*/
typedef struct {
    double cx;
    double cy;
    double cz;
    quaternion *q;
} camera;

typedef struct {
    int *p;
    int np;
    SDL_Color c;
} texture;

typedef struct {
    int useArctan;
    double fov;
} scene_settings;

typedef struct {
    camera *c;
    quaternion **points;
    unsigned int num_points;
    unsigned int max_points;
    texture **textures;
    unsigned int num_textures;
    unsigned int max_textures;
    scene_settings *settings;
} scene;


scene *alloc_scene(void);
void submitRotation(scene *s, int rotation, int direction);
void submit_pt(scene *s, quaternion *p);
void submit_txt(scene *s, texture *t);
void render(SDL_Renderer *r, scene *s);
void compileScene(scene *s);
#endif