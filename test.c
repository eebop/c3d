#include <stdio.h>
#include <immintrin.h>

typedef struct {
    double t;
    double i;
    double j;
    double k;
} quaternion;

void multiplyQuaternion(quaternion *q1, quaternion *q2, quaternion *qout) {
    quaternion qtemp;
    qtemp.t = (q1->t * q2->t) - (q1->i * q2->i) - (q1->j * q2->j) - (q1->k * q2->k);
    qtemp.i = (q1->t * q2->i) + (q1->i * q2->t) - (q1->j * q2->k) + (q1->k * q2->j);
    qtemp.j = (q1->t * q2->j) + (q1->i * q2->k) + (q1->j * q2->t) - (q1->k * q2->i);
    qtemp.k = (q1->t * q2->k) - (q1->i * q2->j) + (q1->j * q2->i) + (q1->k * q2->t);
    /*
    qtemp.t = (q1->t * q2->t) + (q1->i * q2->i) + (q1->j * q2->j) + (q1->k * q2->k);
    qtemp.i = (q1->t * q2->i) + (q1->i * q2->t) + (q1->j * q2->k) + (q1->k * q2->j);
    qtemp.j = (q1->t * q2->j) + (q1->i * q2->k) + (q1->j * q2->t) + (q1->k * q2->i);
    qtemp.k = (q1->t * q2->k) + (q1->i * q2->j) + (q1->j * q2->i) + (q1->k * q2->t);
    */
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

void accMQ(quaternion *q1, quaternion *q2, quaternion *qout) {
    __m256d q1_ = _mm256_loadu_pd((double *) q1);
    __m256d t = _mm256_loadu_pd((double *) q2);
    __m256d i = _mm256_permute_pd(t, 5);
    __m256d j = _mm256_castsi256_pd(_mm256_permute2x128_si256(_mm256_castpd_si256(t), _mm256_castpd_si256(t), 1));
    __m256d k = _mm256_permute_pd(j, 5);
    t = _mm256_mul_pd(t, _mm256_set_pd( q1->t,  q1->t,  q1->t,  q1->t));
    i = _mm256_mul_pd(i, _mm256_set_pd(-q1->i,  q1->i,  q1->i, -q1->i));
    j = _mm256_mul_pd(j, _mm256_set_pd( q1->j,  q1->j, -q1->j, -q1->j));
    k = _mm256_mul_pd(k, _mm256_set_pd( q1->k, -q1->k,  q1->k, -q1->k));
    __m256d qout_ = _mm256_add_pd(_mm256_add_pd(t, i), _mm256_add_pd(j, k));
    _mm256_storeu_pd((double *) qout, qout_);
}

void accFQ(quaternion *q1, quaternion *q2, quaternion *qout) {
    __m256d t = _mm256_loadu_pd((double *) q2);
    __m256d i = _mm256_permute_pd(t, 5);
    __m256d j = _mm256_castsi256_pd(_mm256_permute2x128_si256(_mm256_castpd_si256(t), _mm256_castpd_si256(t), 1));
    __m256d k = _mm256_permute_pd(j, 5);
    t = _mm256_mul_pd(t, _mm256_set_pd( q1->t,  q1->t,  q1->t,  q1->t));
    i = _mm256_mul_pd(i, _mm256_set_pd( q1->i, -q1->i, -q1->i,  q1->i));
    j = _mm256_mul_pd(j, _mm256_set_pd(-q1->j, -q1->j,  q1->j,  q1->j));
    k = _mm256_mul_pd(k, _mm256_set_pd(-q1->k,  q1->k, -q1->k,  q1->k));
    __m256d qout_ = _mm256_add_pd(_mm256_add_pd(t, i), _mm256_add_pd(j, k));
    _mm256_storeu_pd((double *) qout, qout_);
}
void accSQ(quaternion *q1, quaternion *q2, quaternion *qout) {
    __m256d t = _mm256_loadu_pd((double *) q2);
    __m256d i = _mm256_permute_pd(t, 5);
    __m256d j = _mm256_castsi256_pd(_mm256_permute2x128_si256(_mm256_castpd_si256(t), _mm256_castpd_si256(t), 1));
    __m256d k = _mm256_permute_pd(j, 5);
    t = _mm256_mul_pd(t, _mm256_set_pd(-q1->t, -q1->t, -q1->t,  q1->t));
    i = _mm256_mul_pd(i, _mm256_set_pd( q1->i, -q1->i,  q1->i,  q1->i));
    j = _mm256_mul_pd(j, _mm256_set_pd(-q1->j,  q1->j,  q1->j,  q1->j));
    k = _mm256_mul_pd(k, _mm256_set_pd( q1->k,  q1->k, -q1->k,  q1->k));
    __m256d qout_ = _mm256_add_pd(_mm256_add_pd(t, i), _mm256_add_pd(j, k));
    _mm256_storeu_pd((double *) qout, qout_);
}


int WinMain() {
    quaternion q1 = {0.1, 2.0, 1.0, 3.0};
    quaternion q2 = {0.2, 1.0, 2.0, 1.1};
    //quaternion q1 = {0, 0, 1, 0};
    //quaternion q2 = {0, 0, 2, 0};
    quaternion qout;
    multiplyQuaternion(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    accMQ(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    multiplyWithInverseFirstQuaternion(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    accFQ(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    multiplyWithInverseSecondQuaternion(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    accSQ(&q1, &q2, &qout);
    printf("%f, %f, %f, %f\n", qout.t, qout.i, qout.j, qout.k);
    return 0;
}
int main() {
    return WinMain();
}