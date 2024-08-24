#include <stdio.h>
#include <stdlib.h>
#include "logic_block.h"

// LOGIC GATES ----------------------------------------------------------------------------------------------------------
int logic_not(int A) {
    return !A;
}

int logic_and(int A, int B) {
    return A && B;
}

int logic_or(int A, int B) {
    return A || B;
}

int logic_nor(int A, int B) {
    int A_or_B = logic_or(A, B);
    return logic_not(A_or_B);
}

int logic_nand(int A, int B) {
    int A_and_B = logic_and(A, B);
    return logic_not(A_and_B);
}

int xor(int A, int B) {
    int A_nand_B = logic_nand(A, B);
    int A_nand_AnandB = logic_nand(A, A_nand_B);
    int B_nand_AnandB = logic_nand(B, A_nand_B);
    return logic_nand(A_nand_AnandB, B_nand_AnandB);
}

// LOGIC BLOCKS ---------------------------------------------------------------------------------------------------------

// SR LATCH NAND LOGIC --------------------------------------------------------------------------------------------------
void SR_LATCH_init(SR_LATCH* latch, int logic_id) {
    latch->logic_id = logic_id;
    latch->state = 0;  // Q initially set to low level
}

void SR_LATCH_logic(SR_LATCH* latch, int S, int R, int* Q, int* nQ) {
    if (!(R || S)) {
        // if both S and R are low level, SR latch does nothing
    } else if (R) {
        latch->state = 0;  // if R is high level and S is low level, it resets SR (Q=0, Q_=1)
    } else if (S) {
        latch->state = 1;  // if R is low level and S is high level, it sets SR (Q=1, Q_=0)
    }

    *Q = latch->state;
    *nQ = !(*Q);
}

// FLIP-FLOPS -----------------------------------------------------------------------------------------------------------
// rising edge D flip-flop using NAND gate takes two rising edges to reach steady-state
void D_FLIP_FLOP_init(D_FLIP_FLOP* flip_flop, int logic_id) {
    flip_flop->logic_id = logic_id;
    flip_flop->clock_last_state = 0;
    flip_flop->rising_edge = 0;
    flip_flop->Q = 0;
}

void D_FLIP_FLOP_logic(D_FLIP_FLOP* flip_flop, int clk, int d, int* Q, int* nQ) {
    if (!flip_flop->clock_last_state && clk) {
        flip_flop->rising_edge = 1;  // if the rising edge is detected
    } else {
        flip_flop->rising_edge = 0;
    }

    flip_flop->clock_last_state = clk;

    if (flip_flop->rising_edge) {  // when the clock pulse comes
        flip_flop->Q = d;  // set Q based on D
    }

    *Q = flip_flop->Q;  // for readability only
    *nQ = !(*Q);
}

// ADDER ----------------------------------------------------------------------------------------------------------------
void logic_full_adder(int A, int B, int Cin, int* sum, int* Cout) {
    *sum = xor(Cin, xor(A, B));  // the sum of this bit
    *Cout = (A && B) || (B && Cin) || (A && Cin);  // carry out to the higher bit
}

// ACCUMULATOR BLOCK ----------------------------------------------------------------------------------------------------
void ONE_BIT_ACCUMULATOR_init(ONE_BIT_ACCUMULATOR* accumulator, int logic_id) {
    accumulator->logic_id = logic_id;
    D_FLIP_FLOP_init(&accumulator->dflipflop, 0);  // D flip-flop
    accumulator->Q = 0;  // initialize accumulator Q
}

void ONE_BIT_ACCUMULATOR_logic(ONE_BIT_ACCUMULATOR* accumulator, int clk, int y, int Cin, int* sum, int* Cout) {

    // printf("\nInput -> Q = %d, y = %d, Cin = %d\n", accumulator->Q, y, Cin);

    // calculate the sum of input y and accumulator state Q, and the carry out
    logic_full_adder(y, accumulator->Q, Cin, sum, Cout);
    // printf("After Full Adder -> sum = %d, Cout = %d\n", *sum, *Cout);

    // on the rising edge of the clock signal clk, the sum value is stored into the D flip-flop
    D_FLIP_FLOP_logic(&accumulator->dflipflop, clk, *sum, &accumulator->Q, &accumulator->nQ); // Q is used for nQ here
    // printf("After D-Flip-Flop -> Q = %d\n", accumulator->Q);

}

void FOUR_BIT_ACCUMULATOR_init(FOUR_BIT_ACCUMULATOR* accumulator, int logic_id) {
    accumulator->logic_id = logic_id;
    for (int i = 0; i < 4; i++) {
        ONE_BIT_ACCUMULATOR_init(&accumulator->one_bit_accumulators[i], i);
    }
}

void FOUR_BIT_ACCUMULATOR_logic(FOUR_BIT_ACCUMULATOR* accumulator, int clk, const char* y, int Cin, char* sum, int* Cout) {
    int y3 = y[0] - '0';  // LSB
    int y2 = y[1] - '0';
    int y1 = y[2] - '0';
    int y0 = y[3] - '0';  // MSB

    int s0, Cout0;
    ONE_BIT_ACCUMULATOR_logic(&accumulator->one_bit_accumulators[0], clk, y0, Cin, &s0, &Cout0);

    int s1, Cout1;
    ONE_BIT_ACCUMULATOR_logic(&accumulator->one_bit_accumulators[1], clk, y1, Cout0, &s1, &Cout1);

    int s2, Cout2;
    ONE_BIT_ACCUMULATOR_logic(&accumulator->one_bit_accumulators[2], clk, y2, Cout1, &s2, &Cout2);

    int s3, Cout3;
    ONE_BIT_ACCUMULATOR_logic(&accumulator->one_bit_accumulators[3], clk, y3, Cout2, &s3, &Cout3);

    sum[0] = s3 + '0';
    sum[1] = s2 + '0';
    sum[2] = s1 + '0';
    sum[3] = s0 + '0';
    sum[4] = '\0';
    *Cout = Cout3;
}

void N_BIT_ACCUMULATOR_init(N_BIT_ACCUMULATOR* accumulator, int n_bits, int logic_id) {
    accumulator->logic_id = logic_id;
    accumulator->n_bits = n_bits;
    accumulator->one_bit_accumulators = (ONE_BIT_ACCUMULATOR*)malloc(n_bits * sizeof(ONE_BIT_ACCUMULATOR));
    for (int i = 0; i < n_bits; i++) {
        ONE_BIT_ACCUMULATOR_init(&accumulator->one_bit_accumulators[i], i);
    }
}

void N_BIT_ACCUMULATOR_logic(N_BIT_ACCUMULATOR* accumulator, int clk, const char* y, int Cin, char* sum, int* Cout) {
    int* bit_values = (int*)malloc(accumulator->n_bits * sizeof(int));

    // Get length of the input string y
    int y_len = strlen(y);
    
    if (y_len < accumulator->n_bits) {
        char* padded_y = (char*)malloc((accumulator->n_bits + 1) * sizeof(char));
        int padding_len = accumulator->n_bits - y_len;
        for (int i = 0; i < padding_len; i++) {
            padded_y[i] = '0';
        }
        
        strcpy(padded_y + padding_len, y);
        y = padded_y;
    }

    // printf("initial y: %s, initial Cin: %d\n", y, Cin);

    for (int i = 0; i < accumulator->n_bits; i++) {
        bit_values[accumulator->n_bits - 1 - i] = y[i] - '0';  // LSB to MSB
    }

    char* sum_list = (char*)malloc(accumulator->n_bits + 1);
    int current_Cin = Cin;
    for (int i = 0; i < accumulator->n_bits; i++) {
        int sum1;
        int Cout1;
        ONE_BIT_ACCUMULATOR_logic(&accumulator->one_bit_accumulators[i], clk, bit_values[i], current_Cin, &sum1, &Cout1);
        sum_list[i] = sum1 + '0';
        current_Cin = Cout1;
    }
    sum_list[accumulator->n_bits] = '\0';

    for (int i = 0; i < accumulator->n_bits; i++) {
        sum[i] = sum_list[accumulator->n_bits - 1 - i];  // Reverse the result
    }
    sum[accumulator->n_bits] = '\0';
    *Cout = current_Cin;

    free(bit_values);
    free(sum_list);
}
