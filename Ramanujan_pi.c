// gcc Ramanujan_pi.c -o Ramanujan_pi -lgmp -lm -fopenmp && time ./Ramanujan_pi > calc_pi.txt
// gcc Ramanujan_pi.c -o Ramanujan_pi -lgmp -lm -fopenmp && time ./Ramanujan_pi > calc_pi.txt && gcc acuracy_num.c -o acuracy_num -lgmp -fopenmp && ./acuracy_num

#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <omp.h>

#define EXPECT_DIGIT 500000 //期待桁数

int REPEAT_NUM = EXPECT_DIGIT/7.9; // 繰り返し回数
int INIT_BIT = EXPECT_DIGIT*3.4;   // 変数が確保する領域

void factorial(mpz_t result, unsigned long int n) {
    mpz_set_ui(result, 1);
    for (unsigned long int i = 1; i <= n; i++) {
        mpz_mul_ui(result, result, i);
    }
}

int main() {
    mpz_t _4, _99;
    mpf_t pi_inverse;
    mpf_init2(pi_inverse, INIT_BIT);
    mpf_set_ui(pi_inverse, 0);
    mpz_init_set_ui(_4, 4);
    mpz_init_set_ui(_99, 99);

    #pragma omp parallel
    {
        mpf_t local_result;
        mpf_init2(local_result, INIT_BIT);

        #pragma omp for
        for (unsigned long int n = 0; n < REPEAT_NUM; n++) {
            mpz_t _4n_fact, _4pown, _99pown, n_fact, block3, tmp;
            mpf_t tmp_f, block3_f;
            mpz_init2(_4n_fact, INIT_BIT);
            mpz_init2(_4pown, INIT_BIT);
            mpz_init2(_99pown, INIT_BIT);
            mpz_init2(n_fact, INIT_BIT);
            mpz_init2(block3, INIT_BIT);
            mpz_init2(tmp, INIT_BIT);
            mpf_init2(tmp_f, INIT_BIT);
            mpf_init2(block3_f, INIT_BIT);

            unsigned long int block2 = 1103 + (26390 * n);
            unsigned long int _4n = 4 * n;
            factorial(_4n_fact, _4n);
            
            mpz_pow_ui(_4pown, _4, n);
            mpz_pow_ui(_99pown, _99, n);
            factorial(n_fact, n);
            mpz_mul(block3, _4pown, _99pown);
            mpz_mul(block3, block3, n_fact);
            mpz_pow_ui(block3, block3, 4);

            mpz_mul_ui(tmp, _4n_fact, block2);
            mpf_set_z(tmp_f, tmp);
            mpf_set_z(block3_f, block3);
            mpf_div(tmp_f, tmp_f, block3_f);
            mpf_add(local_result, local_result, tmp_f);

            mpz_clear(_4n_fact);
            mpz_clear(_4pown);
            mpz_clear(_99pown);
            mpz_clear(n_fact);
            mpz_clear(block3);
            mpz_clear(tmp);
            mpf_clear(tmp_f);
            mpf_clear(block3_f);
        }

        #pragma omp critical
        {
            mpf_add(pi_inverse, pi_inverse, local_result);
        }

        mpf_clear(local_result);
    }
    mpz_clear(_4);

    mpf_t sqrt2, _2sqrt2;
    mpf_init2(sqrt2, INIT_BIT);
    mpf_init2(_2sqrt2, INIT_BIT);
    mpf_sqrt_ui(sqrt2, 2);
    mpf_mul_ui(_2sqrt2, sqrt2, 2);
    mpf_clear(sqrt2);

    mpz_t _99squared;
    mpf_t _99squared_f;
    mpz_init(_99squared);
    mpf_init(_99squared_f);
    mpz_pow_ui(_99squared, _99, 2);
    mpf_set_z(_99squared_f, _99squared);
    mpz_clear(_99);
    mpz_clear(_99squared);

    mpf_t block1;
    mpf_init2(block1, INIT_BIT);
    mpf_div(block1, _2sqrt2, _99squared_f);
    mpf_clear(_2sqrt2);
    mpf_clear(_99squared_f);

    mpf_mul(pi_inverse, block1, pi_inverse);
    mpf_clear(block1);

    mpf_t pi, _1;
    mpf_init2(pi, INIT_BIT);
    mpf_init_set_ui(_1, 1);
    mpf_div(pi, _1, pi_inverse);
    mpf_clear(pi_inverse);
    mpf_clear(_1);

    gmp_printf("%.1000000Ff\n", pi);
    mpf_clear(pi);

    return 0;
}
