/********************************************************************************************************************
AD9851 Direct Digital Synthesis (DDS) system

It simulates a Direct Digital Synthesis (DDS) system. DDS is a technique used to generate precise frequency waveforms, 
typically sine waves, by digitally synthesizing the waveform using a phase accumulator, a lookup table, and a DAC. 
The resulting output is then filtered.

Reference:
- AD9851 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/AD9851.pdf
- GitHub Repository: https://github.com/Tektronica/direct_digital_synthesis_demo

Author: liu W.C.
Date: 2024.8.13
********************************************************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "logic_block.h"
#include "nco.h"

// #define M_PI 3.14159265358979323846
#define filter_order 4 // low-pass filter order

// Structure to store the simulation data
typedef struct {
    double *time;
    double *phase;
    double *dac_output;
    double *sine_reference;
    double *square_wave;
} Data;

typedef struct {
    double last_remainder;
} ROLLOVER;

// Global constants
double f_sampling = 120e6; // sampling frequency, is twice system clock frequency
double f_output = 5e2;     // output frequency, not exceeding 0.4 * f_MCLK
double f_cutoff = 1e5;     // cutoff frequency of low-pass filter
double f_MCLK = 60e6;      // clock frequency
int N = 28;                // bit depth of phase accumulator
int dac_bit_depth = 10;    // bit depth of DAC

void init_ROLLOVER(ROLLOVER *rollover) {
    rollover->last_remainder = 0;
}

// True if rollover using modulo operation
void check_rollover(ROLLOVER *rollover, double a, double n, bool *result) {

    double this_remainder = fmod(a, n);
    *result = this_remainder <= rollover->last_remainder;
    rollover->last_remainder = this_remainder;
}

// Function to read coefficients from file
void read_file(const char *filename, double **b, double **a, int *b_size, int *a_size) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open the specified file.\n");
        return EXIT_FAILURE;
    }

    fscanf(file, "coefficients:\n");

    // numerator coefficients
    *b = (double *)malloc(*b_size * sizeof(double));
    for (int i = 0; i < *b_size; i++) {
        fscanf(file, "%lf", &(*b)[i]);
    }

    // denominator coefficients
    *a = (double *)malloc(*a_size * sizeof(double));
    for (int i = 0; i < *a_size; i++) {
        fscanf(file, "%lf", &(*a)[i]);
    }

    fclose(file);

}

// LPF design function implementation
void LPF_design(double f_sampling, double **b, double **a) {

    int b_size = filter_order + 1;
    int a_size = filter_order + 1;

    *b = (double *)malloc(b_size * sizeof(double));
    *a = (double *)malloc(a_size * sizeof(double));

    if (*b == NULL || *a == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Read the coefficients of numerator and denominator of discrete transfer function
    const char *filepath = "D:/graduate/vsCode/C/direct_digital_synthesis/table/coefficients.txt";
    read_file(filepath, b, a, &b_size, &a_size);

    printf("numerator coefficients of discrete transfer function (b):\n");
    for (int i = 0; i < b_size; i++) {
        printf("%lf ", (*b)[i]);
    }
    printf("\n");

    // Print the denominator (a) coefficients
    printf("denominator coefficients of discrete transfer function (a):\n");
    for (int i = 0; i < a_size; i++) {
        printf("%lf ", (*a)[i]);
    }
    printf("\n");

    printf("discrete transfer function:\n H(z) =\n");

    for (int j = 0; j < b_size; j++) {
        if (j > 0) {
            printf(" + ");
        }
        printf("%lf*z^{- %d}", (*b)[j], j);
    }

    printf("\n");

    for (int j = 0; j < 165; j++) {
        printf("-");
    }
    printf("\n");

    for (int j = 0; j < a_size; j++) {
        if (j > 0) {
            printf(" + ");
        }
        printf("%lf*z^{- %d}", (*a)[j], j);
    }

    printf("\n");

}

// sin_ROM function implementation
void sin_ROM(int N, char *address, int dac_bit_depth, char *dac_code, double *dac_value) {
    strncpy(dac_code, address, dac_bit_depth);
    dac_code[dac_bit_depth] = '\0';

    char dac_code_makeupzero[N + 1];
    strcpy(dac_code_makeupzero, dac_code);

    for (int i = dac_bit_depth; i < N; i++) {
        dac_code_makeupzero[i] = '0';
    }
    dac_code_makeupzero[N] = '\0';

    int address_int = (int)strtol(dac_code_makeupzero, NULL, 2);
    *dac_value = sin(2 * M_PI * address_int / (1 << N)) * (1 << dac_bit_depth);
}

// DAC function implementation
double DAC(double A, double dac_value, int dac_bit_depth) {
    return A * dac_value / (pow(2, dac_bit_depth) - 1);
}

// Main function
int main() {

    // TIME STEP AND RUN TIME -------------------------------------------------------------------------------------------
    double time = 0.0;
    double t_f = 10 * (1.0 / f_output);     // end time, s
    // double t_f = 10000 * (1.0 / f_sampling);     // end time, s
    double T_sampling = (1.0 / f_sampling); // sampling period, s

    // Initialize simulation data  --------------------------------------------------------------------------------------
    int num_sampling = (int)(t_f / T_sampling); // Calculate the number of sampling point

    Data data;
    data.time = (double *)malloc(num_sampling * sizeof(double));
    data.phase = (double *)malloc(num_sampling * sizeof(double));
    data.dac_output = (double *)malloc(num_sampling * sizeof(double));
    data.sine_reference = (double *)malloc(num_sampling * sizeof(double));
    data.square_wave = (double *)malloc(num_sampling * sizeof(double));

    // PRINT ------------------------------------------------------------------------------------------------------------
    // full scale current multiplied by shunt resistor resistance of 7-th order low-pass filter
    double V_out = 10 * 1e-3 * 100;
    double A = V_out; // amplitude of DAC output
    double phi = 0; // phase offset
    double delta_FSW = f_MCLK / (1 << N); // frequency resolution
    printf("generated sine wave parameters:\n");
    printf("amplitude (A): %.2f\n", A);
    printf("frequency (f): %.2f Hz\n", f_output);
    printf("phase offset (phi): %.2f rad\n", phi);
    printf("sine wave: y(t) = %.2f * sin(2π * %.2f * t + %.2f)\n", A, f_output, phi);
    printf("frequency resolution: %.3f Hz\n", delta_FSW);

    // RUN SIMULATION ---------------------------------------------------------------------------------------------------
    int phase_address = 0;
    double dac_output = 0.0;

    NUMERICALLY_CONTROLLED_OSCILLATOR nco;
    NCO_init(&nco, N, f_MCLK);

    if (nco.phase_register == NULL || nco.delta_Phase == NULL) {
        printf("NCO initialization failed.\n");
        return 1;
    }

    NCO_set_output_frequency(&nco, f_output);

    ROLLOVER rollover;
    init_ROLLOVER(&rollover); // synchronize sampling and clock frequencies

    // Coefficients of the numerator and denominator of discrete transfer function
    int b_size = filter_order + 1;
    int a_size = filter_order + 1;
    double *b = (double*)malloc(b_size * sizeof(double));
    double *a = (double*)malloc(a_size * sizeof(double));

    // LPF design
    LPF_design(f_sampling, &b, &a);

    int m = filter_order; // numerator order of discrete transfer function
    int n = filter_order; // denominator order of discrete transfer function

    double print_interval = 0.001;
    double last_print_time = 0.0;

    ROLLOVER RO;
    bool rollover_detected;

    int i = 0;

    while (time < t_f) {

        check_rollover(&RO, time, (1.0 / f_MCLK), &rollover_detected);
        if (rollover_detected) {  // If sampling frequency is synchronized with clock frequency

            printf("\ntime = %.6f s\n", time);

            // Use the data output from phase accumulator as phase sampling address of waveform memory (ROM)
            phase_address = NCO_phase_accumulator(&nco);

            // Get DAC code and value from sin_ROM
            char dac_code[N+1];
            double dac_value;
            sin_ROM(N, phase_address, dac_bit_depth, dac_code, &dac_value);

            // Calculate DAC output
            dac_output = DAC(A, dac_value, dac_bit_depth);
            printf("DAC output: %.4g\n", dac_output);

            // Reference sine wave
            double sine_reference = A * sin(2 * M_PI * f_output * time + phi);

            // Square wave calculation
            double average_voltage = 0.0;
            double square_wave = dac_output > average_voltage ? 1.0 : 0.0;

            // Print parameters every 0.001 seconds
            if (time - last_print_time >= print_interval) {
                printf("time: %.3f s\n", time);
                printf("phase address: %d\n", phase_address);
                printf("DAC code: %s\n", dac_code);
                printf("DAC value: %.5g\n", dac_value);
                printf("DAC output: %.3g\n", dac_output);
                printf("-----------------------------------\n");
                last_print_time = time;
            }

            // Save data
            data.time[i] = time;
            data.phase[i] = phase_address;
            data.dac_output[i] = dac_output;
            data.sine_reference[i] = sine_reference;
            data.square_wave[i] = square_wave;

            i++;
        }

        // Increment time
        time += T_sampling;
    }


    // Calculate duty cycle of square wave
    int high_count = 0;
    for (int i = 0; i < num_sampling; i++) {
        if (data.square_wave[i] > 0.0) {
            high_count++;
        }
    }
    double duty_cycle = (double)high_count / num_sampling * 100; // Percentage
    printf("Duty cycle of square wave: %.4f%%\n", duty_cycle);

    // Save data to file
    FILE *file = fopen("D:/graduate/vsCode/C/direct_digital_synthesis/table/data.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open data file for writing.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_sampling; i++) {
        fprintf(file, "%.6f\t%d\t%.6f\t%.1f\n",
                data.time[i], (int)data.phase[i], data.dac_output[i], data.square_wave[i]);
    }

    fclose(file);

    // Free allocated memory
    free(data.time);
    free(data.phase);
    free(data.dac_output);
    free(data.sine_reference);
    free(data.square_wave);

    getch();
    return 0;
}
