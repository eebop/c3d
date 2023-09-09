#ifndef __QUATERNION_H
#define __QUATERNION_H

#define CREATE_QUATERNION(p, _i, _j, _k) p .i = _i ; p .j = _j ; p .k = _k ; p .t = 0; 

typedef struct {
    double i;
    double j;
    double k;
    double t;
} quaternion;
quaternion *newQuaternion(void);
void multiplyQuaternion(quaternion *q1, quaternion *q2, quaternion *qout);
#endif