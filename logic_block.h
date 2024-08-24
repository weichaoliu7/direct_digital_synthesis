#ifndef LOGIC_BLOCK_H
#define LOGIC_BLOCK_H

// LOGIC GATES ----------------------------------------------------------------------------------------------------------
int logic_not(int A);
int logic_and(int A, int B);
int logic_or(int A, int B);
int logic_nor(int A, int B);
int logic_nand(int A, int B);
int xor(int A, int B);

// LOGIC BLOCKS ---------------------------------------------------------------------------------------------------------

// SR LATCH NAND LOGIC --------------------------------------------------------------------------------------------------
typedef struct {
    int logic_id;
    int state;
} SR_LATCH;

void SR_LATCH_init(SR_LATCH* latch, int logic_id);
void SR_LATCH_logic(SR_LATCH* latch, int S, int R, int* Q, int* nQ);

// FLIP-FLOPS -----------------------------------------------------------------------------------------------------------
typedef struct {
    int logic_id;
    int clock_last_state;
    int rising_edge;
    int Q;
} D_FLIP_FLOP;

void D_FLIP_FLOP_init(D_FLIP_FLOP* flip_flop, int logic_id);
void D_FLIP_FLOP_logic(D_FLIP_FLOP* flip_flop, int clk, int d, int* Q, int* nQ);

// ADDER ----------------------------------------------------------------------------------------------------------------
void logic_full_adder(int A, int B, int Cin, int* sum, int* Cout);

// ACCUMULATOR BLOCK ----------------------------------------------------------------------------------------------------
typedef struct {
    int logic_id;
    D_FLIP_FLOP dflipflop;
    int Q;
    int nQ;
} ONE_BIT_ACCUMULATOR;

void ONE_BIT_ACCUMULATOR_init(ONE_BIT_ACCUMULATOR* accumulator, int logic_id);
void ONE_BIT_ACCUMULATOR_logic(ONE_BIT_ACCUMULATOR* accumulator, int clk, int y, int Cin, int* sum, int* Cout);

typedef struct {
    int logic_id;
    ONE_BIT_ACCUMULATOR one_bit_accumulators[4];
} FOUR_BIT_ACCUMULATOR;

void FOUR_BIT_ACCUMULATOR_init(FOUR_BIT_ACCUMULATOR* accumulator, int logic_id);
void FOUR_BIT_ACCUMULATOR_logic(FOUR_BIT_ACCUMULATOR* accumulator, int clk, const char* y, int Cin, char* result, int* Cout);

typedef struct {
    int logic_id;
    int n_bits;
    ONE_BIT_ACCUMULATOR* one_bit_accumulators;
} N_BIT_ACCUMULATOR;

void N_BIT_ACCUMULATOR_init(N_BIT_ACCUMULATOR* accumulator, int n_bits, int logic_id);
void N_BIT_ACCUMULATOR_logic(N_BIT_ACCUMULATOR* accumulator, int clk, const char* y, int Cin, char* result, int* Cout);

#endif // LOGIC_BLOCK_H
