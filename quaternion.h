#ifndef __QUATERNION_H
#define __QUATERNION_H
typedef struct {
    double i;
    double j;
    double k;
    double t;
} quaternion;
quaternion *newQuaternion(void);
void multiplyQuaternion(quaternion *q1, quaternion *q2, quaternion *qout);
#endif