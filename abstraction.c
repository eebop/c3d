#include <stdlib.h>
#include "abstraction.h"
#include "quaternion.h"

physicsT *allocPhysics(void) {
    physicsT *p = malloc(sizeof(physicsT));
    p->maxdata = 16;
    p->usedata = 0;
    p->data = malloc(16 * sizeof(object *));
    return p;
}

void submitPhysicsObject(physicsT *p, object *o) {
    if (p->maxdata == p->usedata) {
        p->data = realloc(p->data, 2 * p->maxdata * sizeof(object *));
        p->maxdata *= 2;
    }
    p->data[p->usedata++] = o;
}

void stepOne(object *o) {
    quaternion center;
    quaternion point;
    quaternion qtemp;
    quaternion qout;
    center.t = 0;
    center.i = (o->p[0].i + o->p[1].i + o->p[2].i + o->p[3].i + o->p[4].i + o->p[5].i + o->p[6].i + o->p[7].i) / 8;
    center.j = (o->p[0].j + o->p[1].j + o->p[2].j + o->p[3].j + o->p[4].j + o->p[5].j + o->p[6].j + o->p[7].j) / 8;
    center.k = (o->p[0].k + o->p[1].k + o->p[2].k + o->p[3].k + o->p[4].k + o->p[5].k + o->p[6].k + o->p[7].k) / 8;
    for (int x=0;x!=8;x++) {
        point.t = 0;
        point.i = o->p[x].i - center.i;
        point.j = o->p[x].j - center.j;
        point.k = o->p[x].k - center.k;
        multiplyQuaternion(&(o->rotation), &point, &qtemp);
        multiplyWithInverseSecondQuaternion(&qtemp, &(o->rotation), &qout);
        o->p[x].i = qout.i + center.i + o->velocity.i;
        o->p[x].j = qout.j + center.j + o->velocity.j;
        o->p[x].k = qout.k + center.k + o->velocity.k;
    }
}

void physicsStep(physicsT *p) {
    for (int i=0;i!=p->usedata;i++) {
        stepOne(p->data[i]);
    }
}