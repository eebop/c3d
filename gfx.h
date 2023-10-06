#ifndef __GFX_H
#define __GFX_H
#include "SDL2/SDL.h"
#include "quaternion.h"
#include <stdbool.h>

#define FOUR_POINTS 4
#define ONE_POINT 1

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
    uint16_t type;
    SDL_Color c;
    bool front;
} texture;

typedef struct {
    int useArctan;
    double fov;
    int grabMouse;
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
void submitQuaternionRotation(scene *s, quaternion *rotation);
void submit_pt(scene *s, quaternion *p);
void submit_txt(scene *s, texture *t);
void render(SDL_Renderer *r, scene *s);
void compileScene(scene *s);
#endif