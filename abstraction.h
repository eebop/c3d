#ifndef __ABSTRACTION_H
#define __ABSTRACTION_H
#include "gfx.h"
#include "quaternion.h"
#include <stdbool.h>
typedef struct {
    quaternion *p;
    texture *t;
    bool isreal;
    int numpoints;
} object;
typedef struct {
    object **o;
    quaternion centerofmass;
    quaternion rotation;
    quaternion velocity;
    unsigned int maxobjs;
    unsigned int useobjs;
} entity;

typedef struct {
    texture **face;
    int numfaces;
    int maxfaces;
    quaternion **edge1;
    quaternion **edge2;
    int numedges;
    int maxedges;
} multibucket;

typedef struct {
    entity **data;
    unsigned int maxdata;
    unsigned int usedata;
    uint64_t last_tick;
    multibucket collisionTable[4096];
} physicsT;

physicsT *allocPhysics(void);

void submitPhysicsEntity(physicsT *p, entity *e);

void physicsStep(physicsT *p);

void submitObjectForEntity(entity *e, object *o);

void regenerateCenter(entity *e);

entity *loadEntity(char *data);

entity *allocEntity();

#endif