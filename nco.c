#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nco.h"

// Initialize the NCO
void NCO_init(NUMERICALLY_CONTROLLED_OSCILLATOR* nco, int N, int f_MCLK) {
    nco->f_MCLK = f_MCLK; // clock frequency
    nco->N = N; // bit depth of phase accumulator
    
    // Allocate memory for phase_register and delta_Phase
    nco->phase_register = (char*)malloc(N + 1);
    nco->delta_Phase = (char*)malloc(N + 1);
    
    if (nco->phase_register == NULL || nco->delta_Phase == NULL) {
        // Handle memory allocation failure
        NCO_cleanup(nco);
        return;
    }
    
    memset(nco->phase_register, '0', N);
    nco->phase_register[N] = '\0';
    
    // frequency tuning word, i.e. value that phase accumulator adds in each clock cycle
    memset(nco->delta_Phase, '0', N - 1);
    nco->delta_Phase[N - 1] = '1';
    nco->delta_Phase[N] = '\0';
    
    N_BIT_ACCUMULATOR_init(&nco->n_bit_accumulator, N, 0);
}

// next frequency tuning word delta_Phase is equal to output frequency divided by system clock frequency multiplied by 2 to the Nth power,
// where N is the number of phase accumulator bits
void NCO_set_output_frequency(NUMERICALLY_CONTROLLED_OSCILLATOR* nco, double f_output) {
    if (nco->phase_register == NULL || nco->delta_Phase == NULL) {
        printf("Error: phase_register or delta_Phase is NULL.\n");
        return;
    }
    
    int new_delta_Phase = (int)((1 << nco->N) * (f_output / nco->f_MCLK));

    if (new_delta_Phase >= (1 << 28)) {
        new_delta_Phase = (1 << 28) - 1; // Limit to 28 bits
    }

    char new_delta_Phase_str[nco->N+1];
    new_delta_Phase_str[nco->N] = '\0'; // Null-terminate the string

    for (int i = 0; i < nco->N; i++) {
        new_delta_Phase_str[nco->N-1 - i] = (new_delta_Phase & (1 << i)) ? '1' : '0';
    }
    // printf("new delta Phase: %s\n", new_delta_Phase_str);

    for (int i = 0; i < nco->N; i++) {
        nco->delta_Phase[i] = new_delta_Phase_str[i];
    }
    nco->delta_Phase[nco->N] = '\0';

    NCO_set_frequency_tuning_word(nco, nco->delta_Phase);
    // printf("delta_Phase: %s\n", nco->delta_Phase);
}

// Set the frequency tuning word
// at each clock pulse input, its output is increased by one step of phase increment value
void NCO_set_frequency_tuning_word(NUMERICALLY_CONTROLLED_OSCILLATOR* nco, const char* new_delta_Phase) {
    if (nco->delta_Phase == NULL) return;
    strncpy(nco->delta_Phase, new_delta_Phase, nco->N);
    nco->delta_Phase[nco->N] = '\0';
}

// Get the frequency tuning word
const char* NCO_get_frequency_tuning_word(const NUMERICALLY_CONTROLLED_OSCILLATOR* nco) {
    return nco->delta_Phase;
}

// Set the phase register
// N bit register, which is loaded the modulus 2^N sum of its old output and the frequency tuning word
void NCO_set_phase_register(NUMERICALLY_CONTROLLED_OSCILLATOR* nco, const char* last_phase) {
    if (nco->phase_register == NULL) return;
    strncpy(nco->phase_register, last_phase, nco->N);
    nco->phase_register[nco->N] = '\0';
}

// Get the phase register
const char* NCO_get_phase_register(const NUMERICALLY_CONTROLLED_OSCILLATOR* nco) {
    return nco->phase_register;
}

// Reset the phase register
// N bit register, which is loaded the modulus 2^N sum of its old output and the frequency tuning word
void NCO_reset_phase_register(NUMERICALLY_CONTROLLED_OSCILLATOR* nco) {
    if (nco->phase_register == NULL) return;
    memset(nco->phase_register, '0', nco->N);
    nco->phase_register[nco->N] = '\0';
}

// Phase accumulator
// phase accumulator emulates the mod-2*pi nature of the sinusoidal function by accumulating frequency tuning word and
// overflowing it periodically using the mod-2^N operation of N-bit.
const char* NCO_phase_accumulator(NUMERICALLY_CONTROLLED_OSCILLATOR* nco) {
    if (nco->phase_register == NULL || nco->delta_Phase == NULL) {
        printf("Error: phase_register or delta_Phase is NULL.\n");
        return;
    }

    nco->phase_register[nco->N] = '\0';
    nco->delta_Phase[nco->N] = '\0';
    // printf("phase register: %s\n", nco->phase_register);
    // printf("delta Phase: %s\n", nco->delta_Phase);
    
    // Allocate memory for next_address
    char next_address[nco->N + 1];
    memset(next_address, '0', nco->N);
    next_address[nco->N] = '\0'; // Ensure null termination

    int carry;
    N_BIT_ACCUMULATOR_logic(&nco->n_bit_accumulator, 1, nco->delta_Phase, 0, next_address, &carry);
    // printf("next address: %s\n", next_address);
    // printf("carry: %d\n", carry);

    N_BIT_ACCUMULATOR_logic(&nco->n_bit_accumulator, 0, "0", 0, next_address, &carry);

    NCO_set_phase_register(nco, next_address);

    return nco->phase_register;
}

// Cleanup the NCO
void NCO_cleanup(NUMERICALLY_CONTROLLED_OSCILLATOR* nco) {
    if (nco->phase_register != NULL) {
        free(nco->phase_register);
        nco->phase_register = NULL;
    }
    if (nco->delta_Phase != NULL) {
        free(nco->delta_Phase);
        nco->delta_Phase = NULL;
    }
}
