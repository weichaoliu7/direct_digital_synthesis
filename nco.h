#ifndef NCO_H
#define NCO_H
#include "logic_block.h"

// Structure for NCO
typedef struct{
    int f_MCLK;           // Clock frequency
    int N;                // Bit depth of phase accumulator
    char *phase_register; // Phase register
    char *delta_Phase;    // Frequency tuning word
    N_BIT_ACCUMULATOR n_bit_accumulator;
} NUMERICALLY_CONTROLLED_OSCILLATOR;

// Function prototypes
void NCO_init(NUMERICALLY_CONTROLLED_OSCILLATOR *nco, int N, int f_MCLK);
void NCO_set_output_frequency(NUMERICALLY_CONTROLLED_OSCILLATOR *nco, double f_output);
void NCO_set_frequency_tuning_word(NUMERICALLY_CONTROLLED_OSCILLATOR *nco, const char *new_delta_Phase);
const char *NCO_get_frequency_tuning_word(const NUMERICALLY_CONTROLLED_OSCILLATOR *nco);
void NCO_set_phase_register(NUMERICALLY_CONTROLLED_OSCILLATOR *nco, const char *last_phase);
const char *NCO_get_phase_register(const NUMERICALLY_CONTROLLED_OSCILLATOR *nco);
void NCO_reset_phase_register(NUMERICALLY_CONTROLLED_OSCILLATOR *nco);
const char *NCO_phase_accumulator(NUMERICALLY_CONTROLLED_OSCILLATOR *nco);
void NCO_cleanup(NUMERICALLY_CONTROLLED_OSCILLATOR *nco);

#endif // NCO_H
