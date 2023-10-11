#include <stdio.h>
#include <immintrin.h>

int WinMain() {
    float a[4] = {0.1, 0.2, 3.1, 2.2};
    float b[4] = {0.1, 0.1, 1.3, 1.1};
    float o[4];
    __m128 a_ = _mm_loadu_ps(a);
    __m128 b_ = _mm_loadu_ps(b);
    __m128 ot = _mm_add_ps(a_, b_);
    _mm_storeu_ps(o, ot);
    printf("%f, %f, %f, %f\n", o[0], o[1], o[2], o[3]);
}






