#include <stdlib.h>
#include "quaternion.h"


quaternion *newQuaternion(void) {
    quaternion *q = malloc(sizeof(quaternion));
    return q;
}

// Temp variable is so that operations like multiply(a, b, a) are ok
void multiplyQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    quaternion qtemp;
    qtemp.t = (q1->t * q2->t) - (q1->i * q2->i) - (q1->j * q2->j) - (q1->k * q2->k);
    qtemp.i = (q1->t * q2->i) + (q1->i * q2->t) - (q1->j * q2->k) + (q1->k * q2->j);
    qtemp.j = (q1->t * q2->j) + (q1->i * q2->k) + (q1->j * q2->t) - (q1->k * q2->i);
    qtemp.k = (q1->t * q2->k) - (q1->i * q2->j) + (q1->j * q2->i) + (q1->k * q2->t);
    qout->t = qtemp.t;
    qout->i = qtemp.i;
    qout->j = qtemp.j;
    qout->k = qtemp.k;
}
void multiplyWithInverseFirstQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    quaternion qtemp;
    qtemp.t = (q1->t * q2->t) + (q1->i * q2->i) + (q1->j * q2->j) + (q1->k * q2->k);
    qtemp.i = (q1->t * q2->i) - (q1->i * q2->t) + (q1->j * q2->k) - (q1->k * q2->j);
    qtemp.j = (q1->t * q2->j) - (q1->i * q2->k) - (q1->j * q2->t) + (q1->k * q2->i);
    qtemp.k = (q1->t * q2->k) + (q1->i * q2->j) - (q1->j * q2->i) - (q1->k * q2->t);
    qout->t = qtemp.t;
    qout->i = qtemp.i;
    qout->j = qtemp.j;
    qout->k = qtemp.k;
}
void multiplyWithInverseSecondQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    quaternion qtemp;
    qtemp.t = (q1->t * q2->t) + (q1->i * q2->i) + (q1->j * q2->j) + (q1->k * q2->k);
    qtemp.i =-(q1->t * q2->i) + (q1->i * q2->t) + (q1->j * q2->k) - (q1->k * q2->j);
    qtemp.j =-(q1->t * q2->j) - (q1->i * q2->k) + (q1->j * q2->t) + (q1->k * q2->i);
    qtemp.k =-(q1->t * q2->k) + (q1->i * q2->j) - (q1->j * q2->i) + (q1->k * q2->t);
    qout->t = qtemp.t;
    qout->i = qtemp.i;
    qout->j = qtemp.j;
    qout->k = qtemp.k;

}