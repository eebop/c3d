#include <SDL2/SDL.h>
#include <stdlib.h>
#include "gfx.h"
#include "main.h"
#include "math.h"
#include "quaternion.h"
#include "events.h"
#include "abstraction.h"

#define ANGLE_MOVEMENT (M_PI / 64)

const int POINTS[][4] = {
    {0, 1, 3, 2},
    {0, 4, 5, 1},
    {0, 2, 6, 4},
    {7, 6, 4, 5},
    {7, 3, 2, 6},
    {7, 5, 1, 3}
};

void submit(scene *s, double i, double j, double k, physicsT *physics) {
    object *o = malloc(sizeof(object));
    quaternion *p = malloc(8 * sizeof(quaternion));
    texture *t = malloc(6 * sizeof(texture));
    double speed;
    for (int n=0;n!=6;n++) {
        t[n].p = malloc(4 * sizeof(int));
        t[n].np = 4;
        t[n].c.r = 0xFF;
        t[n].c.g = 0x00;
        t[n].c.b = 0x00;
        t[n].c.a = 0xFF;
    }
    CREATE_QUATERNION(p[0], i  , j  , k  );
    CREATE_QUATERNION(p[1], i  , j  , k+1);
    CREATE_QUATERNION(p[2], i  , j+1, k  );
    CREATE_QUATERNION(p[3], i  , j+1, k+1);
    CREATE_QUATERNION(p[4], i+1, j  , k  );
    CREATE_QUATERNION(p[5], i+1, j  , k+1);
    CREATE_QUATERNION(p[6], i+1, j+1, k  );
    CREATE_QUATERNION(p[7], i+1, j+1, k+1);
    
    //t[0].p = POINT1;

    for (int i=0;i!=6;i++) {
        for (int j=0;j!=4;j++) {
            t[i].p[j] = POINTS[i][j];
        }
    }

    submit_txt(s, t+0);
    submit_txt(s, t+1);
    submit_txt(s, t+2);
    submit_txt(s, t+3);
    submit_txt(s, t+4);
    submit_txt(s, t+5);
    submit_pt(s, p+0);
    submit_pt(s, p+1);
    submit_pt(s, p+2);
    submit_pt(s, p+3);
    submit_pt(s, p+4);
    submit_pt(s, p+5);
    submit_pt(s, p+6);
    submit_pt(s, p+7);
    o->t = t;
    o->p = p;
    CREATE_QUATERNION(o->velocity, ((double)rand()/(double)(RAND_MAX) - 0.5) / 100, ((double)rand()/(double)(RAND_MAX) - 0.5) / 100, ((double)rand()/(double)(RAND_MAX) - 0.5)/ 100);
    speed = ((double)rand()/(double)(RAND_MAX)) / 100;
    CREATE_QUATERNION(o->rotation, ((double)rand()/(double)(RAND_MAX)) * SDL_sin(speed), ((double)rand()/(double)(RAND_MAX)) * SDL_sin(speed), ((double)rand()/(double)(RAND_MAX)) * SDL_sin(speed));

    o->rotation.t = SDL_cos(speed);
    submitPhysicsObject(physics, o);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Test", 100, 100, 800, 800, SDL_TEXTUREACCESS_TARGET);
    SDL_Renderer *r = SDL_CreateRenderer(w, 0, SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    physicsT *physics = allocPhysics();
    SDL_SetRenderTarget(r, NULL);


    SDL_Event event;
    scene *s = alloc_scene();
    SDL_RenderClear(r);
    SDL_RenderPresent(r);



    for (int i=0;i!=1000;i++) {
        submit(s, ((double)rand()/(double)(RAND_MAX)) * 100 - 50, ((double)rand()/(double)(RAND_MAX)) * 100 - 50, ((double)rand()/(double)(RAND_MAX)) * 100 - 50, physics);
    }

    compileScene(s);

    while (1) {
        SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(r);
        SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
        render(r, s);
        SDL_RenderPresent(r);
        //SDL_Delay(1);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
            update_debug(event, s);
            if (s->settings->grabMouse && event.type == SDL_MOUSEMOTION) {
                mouseUpdate(event, s);
            }
        }
        physicsStep(physics);
        compileScene(s);
    }
}