#include <stdlib.h>
#include "quaternion.h"


quaternion *newQuaternion(void) {
    quaternion *q = malloc(sizeof(quaternion));
    return q;
}

void multiplyQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    qout->t = (q1->t * q2->t) - (q1->i * q2->i) - (q1->j * q2->j) - (q1->k * q2->k);
    qout->i = (q1->t * q2->i) + (q1->i * q2->t) - (q1->j * q2->k) + (q1->k * q2->j);
    qout->j = (q1->t * q2->j) + (q1->i * q2->k) + (q1->j * q2->t) - (q1->k * q2->i);
    qout->k = (q1->t * q2->k) - (q1->i * q2->j) + (q1->j * q2->i) + (q1->k * q2->t);
}
void multiplyWithInverseFirstQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    qout->t = (q1->t * q2->t) + (q1->i * q2->i) + (q1->j * q2->j) + (q1->k * q2->k);
    qout->i = (q1->t * q2->i) - (q1->i * q2->t) + (q1->j * q2->k) - (q1->k * q2->j);
    qout->j = (q1->t * q2->j) - (q1->i * q2->k) - (q1->j * q2->t) + (q1->k * q2->i);
    qout->k = (q1->t * q2->k) + (q1->i * q2->j) - (q1->j * q2->i) - (q1->k * q2->t);
}
void multiplyWithInverseSecondQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    qout->t = (q1->t * q2->t) + (q1->i * q2->i) + (q1->j * q2->j) + (q1->k * q2->k);
    qout->i =-(q1->t * q2->i) + (q1->i * q2->t) + (q1->j * q2->k) - (q1->k * q2->j);
    qout->j =-(q1->t * q2->j) - (q1->i * q2->k) + (q1->j * q2->t) + (q1->k * q2->i);
    qout->k =-(q1->t * q2->k) + (q1->i * q2->j) - (q1->j * q2->i) + (q1->k * q2->t);

}