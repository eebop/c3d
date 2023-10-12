#include <stdlib.h>
#include "abstraction.h"
#include "quaternion.h"

physicsT *allocPhysics(void) {
    physicsT *p = malloc(sizeof(physicsT));
    p->maxdata = 16;
    p->usedata = 0;
    p->data = malloc(16 * sizeof(object *));
    p->last_tick = SDL_GetTicks64();
    return p;
}

entity *allocEntity() {
    entity *e = malloc(sizeof(entity));
    e->maxobjs = 1;
    e->useobjs = 0;
    e->o = malloc(sizeof(object *));
    return e;
}
void regenerateCenter(entity *e) {
    double counter = 0;
    CREATE_QUATERNION(e->centerofmass, 0, 0, 0);
    for (unsigned int i=0;i!=e->useobjs;i++) {
        if (e->o[i]->isreal) {
            for (int j=0;j!=8;j++) {
                counter++;
                e->centerofmass.i += e->o[i]->p[j].i;
                e->centerofmass.j += e->o[i]->p[j].j;
                e->centerofmass.k += e->o[i]->p[j].k;
            }
        }
    }
    if (counter) {
        e->centerofmass.i /= counter;
        e->centerofmass.j /= counter;
        e->centerofmass.k /= counter;
    }
}

void submitObjectForEntity(entity *e, object *o) {
    if (e->maxobjs == e->useobjs) {
        e->o = realloc(e->o, 2 * e->maxobjs * sizeof(object *));
        e->maxobjs *= 2;
    }
    e->o[e->useobjs++] = o;
    regenerateCenter(e);
}

void submitPhysicsEntity(physicsT *p, entity *e) {
    if (p->maxdata == p->usedata) {
        p->data = realloc(p->data, 2 * p->maxdata * sizeof(object *));
        p->maxdata *= 2;
    }
    p->data[p->usedata++] = e;
}

void step(object *o, quaternion *center, quaternion *rotation, quaternion *movement) {
    quaternion point;
    for (int i=0;i!=o->numpoints;i++) {
        point.t = 0;
        point.i = o->p[i].i - center->i;
        point.j = o->p[i].j - center->j;
        point.k = o->p[i].k - center->k;
        multiplyQuaternion(rotation, &point, &point);
        multiplyWithInverseSecondQuaternion(&point, rotation, &point);
        o->p[i].i = point.i + center->i + movement->i;
        o->p[i].j = point.j + center->j + movement->j;
        o->p[i].k = point.k + center->k + movement->k;
    }
}

void stepEntity(entity *e, double amount) {
    quaternion rotation = e->rotation;
    quaternion movement;
    for(uint64_t i=1;i!=amount;i++) {
        multiplyQuaternion(&rotation, &(e->rotation), &rotation);
    }
    movement.t = e->velocity.t * amount;
    movement.i = e->velocity.i * amount;
    movement.j = e->velocity.j * amount;
    movement.k = e->velocity.k * amount;
    for (unsigned int j=0;j!=e->useobjs;j++) {
        step(e->o[j], &(e->centerofmass), &rotation, &movement);
        regenerateCenter(e);
    }

}

void physicsStep(physicsT *p) {
    uint64_t time = (SDL_GetTicks64() - p->last_tick) / 64;
    //printf("time is %llu\n", time);
    //return;
    if (time > 0) {
        p->last_tick = SDL_GetTicks64();
    } else {
        return;
    }
    for (unsigned int i=0;i!=p->usedata;i++) {
        stepEntity(p->data[i], (double) time);
    }
}

/*
entity *loadEntity(char *string) {
    
}
*/