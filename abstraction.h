#ifndef __ABSTRACTION_H
#define __ABSTRACTION_H
#include "gfx.h"
#include "quaternion.h"
typedef struct {
    quaternion *p;
    texture *t;
    quaternion rotation;
    quaternion velocity;
} object;
// typedef struct {
// 
// } entity;
// Todo: use entity, add stepsize
typedef struct {
    object **data;
    unsigned int maxdata;
    unsigned int usedata;
    uint64_t last_tick;
} physicsT;

physicsT *allocPhysics(void);

void submitPhysicsObject(physicsT *p, object *o);

void physicsStep(physicsT *p);

#endif