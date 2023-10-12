#include <stdio.h>
#include <immintrin.h>

#pragma pack (4)
struct q {
    float i;
    float j;
    float k;
    float t;
};
typedef struct q q;
int WinMain() {
#ifdef __AVX__
    printf("avx\n");
#endif
    printf("%d\n", __alignof(float));
    float a[8] = {0.1, 0.2, 3.1, 2.2, 0.1, 0.2, 3.1, 2.2};
    float b[8] = {0.1, 0.1, 1.3, 1.1, 0.1, 0.2, 3.1, 2.2};
    float o[8];
    __m256 a_ = _mm256_loadu_ps(a);
    __m256 b_ = _mm256_loadu_ps(b);
    __m256 ot = _mm256_add_ps(a_, b_);
    _mm256_storeu_ps(o, ot);
    printf("%f, %f, %f, %f, %f, %f, %f, %f\n", o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7]);
}






