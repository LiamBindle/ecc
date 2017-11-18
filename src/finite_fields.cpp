#include <finite_fields.hpp>
#include <iostream>

void ff::PrimeField::add(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_add(result, op1, op2);
    mpz_mod(result, result, p);
}

void ff::PrimeField::sub(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_sub(result, op1, op2);
    mpz_mod(result, result, p);
}

void ff::PrimeField::mul(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_mul(result, op1, op2);
    mpz_mod(result, result, p);
}

void ff::PrimeField::inv(const mpz_t op1, mpz_t result) {
    mpz_invert(result, op1, p);
}

void ff::BinaryField::add(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_xor(result, op1, op2);
}

void ff::BinaryField::sub(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_xor(result, op1, op2);
}

void ff::BinaryField::mul(const mpz_t op1, const mpz_t op2, mpz_t result) {
    mpz_t b;
    mpz_init2(b, nbits);
    mpz_set(b, op2);

    if(mpz_tstbit(op1, 0)) {
        mpz_set(result, b);
    } else {
        mpz_set_ui(result, 0);
    }

    for(size_t i = 1; i < nbits; ++i) {
        mpz_mul_2exp(b, b, 1);
        if(mpz_tstbit(b, nbits-1)) {
            mpz_xor(b, b, ip);
        }
        if(mpz_tstbit(op1, i)) {
            mpz_xor(result, result, b);
        }
    }
}

void ff::BinaryField::inv(const mpz_t op1, mpz_t result) {
    mpz_t u, v, tu, tv, g1, g2, tmp;
    int deg_u, deg_v;
    mpz_init(tmp);
    mpz_init_set(u, op1);
    mpz_init_set(v, ip);
    mpz_init2(tu, nbits);
    mpz_init2(tv, nbits);
    mpz_init_set_ui(g1, 1);
    mpz_init_set_ui(g2, 0);
    mpz_inits(tu, tv, NULL);
    do {
        mpz_set(tu, u);
        mpz_set(tv, v);
        for(deg_u = 0; mpz_cmp_ui(tu, 0) > 0; mpz_fdiv_q_2exp(tu, tu, 1), ++deg_u);
        for(deg_v = 0; mpz_cmp_ui(tv, 0) > 0; mpz_tdiv_q_2exp(tv, tv, 1), ++deg_v);
        int j = deg_u - deg_v;
        if(j < 0) {
            mpz_swap(u, v);
            mpz_swap(g1, g2);
            j = -j;
        }
        mpz_mul_2exp(tmp, v, j);
        mpz_xor(u, u, tmp);

        mpz_mul_2exp(tmp, g2, j);
        mpz_xor(g1, g1, tmp);
    } while(mpz_cmp_ui(u, 1) != 0);

    mpz_set(result, g1);
}