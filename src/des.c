#include <stdint.h>
#include <stdio.h>

/* Bang hoan vi chuan DES (Slide 17 & FIPS 46-3) */
static const int IP_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

static const int E_TABLE[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static const int P_TABLE[32] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

/* Ham hoan vi bit tong quat */
static uint64_t permute(uint64_t input, const int *table, int in_bits, int out_bits) {
    uint64_t output = 0;
    for (int i = 0; i < out_bits; i++) {
        uint64_t bit = (input >> (in_bits - table[i])) & 1ULL;
        output = (output << 1) | bit;
    }
    return output;
}

/* In ma tran vi tri bit tong quat b0..b63 qua IP, E(R0) va b0..b31 qua P */
static void print_symbolic_des_tables(void) {
    printf("=== BIEU DIEN TONG QUAT HOAN VI IP, MO RONG EP VA HOAN VI P ===\n");
    printf("[1] Ma tran sau IP (L0 = 4 hang tren, R0 = 4 hang duoi):\n");
    for (int row = 0; row < 8; row++) {
        if (row == 0) printf("  -- L0 (32 bit dau) --\n");
        if (row == 4) printf("  -- R0 (32 bit sau) --\n");
        printf("  ");
        for (int col = 0; col < 8; col++) {
            printf("b%-3d ", IP_TABLE[row * 8 + col] - 1);
        }
        printf("\n");
    }

    printf("\n[2] Ma tran mo rong EP(R0) (48 bit) suy tu R0:\n");
    for (int row = 0; row < 8; row++) {
        printf("  ");
        for (int col = 0; col < 6; col++) {
            int pos_in_R0 = E_TABLE[row * 6 + col] - 1;
            int bit_idx = IP_TABLE[32 + pos_in_R0] - 1;
            printf("b%-3d ", bit_idx);
        }
        printf("\n");
    }

    printf("\n[3] Ma tran hoan vi P(b0..b31) (32 bit):\n");
    for (int row = 0; row < 4; row++) {
        printf("  ");
        for (int col = 0; col < 8; col++) {
            printf("b%-3d ", P_TABLE[row * 8 + col] - 1);
        }
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    print_symbolic_des_tables();

    /* Test nhanh ham permute voi bang IP */
    uint64_t pt = 0x0123456789ABCDEFULL;
    uint64_t ip = permute(pt, IP_TABLE, 64, 64);
    printf("Test IP(0x0123456789ABCDEF) = 0x%016llX (Ky vong: 0xCC00CCFFF0AAF0AA)\n",
           (unsigned long long)ip);
    return 0;
}