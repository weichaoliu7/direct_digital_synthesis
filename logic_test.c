#include <stdio.h>
#include "logic_block.h"

void print_d_flip_flop_test() {
    printf("D Flip Flop test\n");
    D_FLIP_FLOP dff;
    D_FLIP_FLOP_init(&dff, 0);

    int Q, nQ;

    printf("1 ----> ");
    D_FLIP_FLOP_logic(&dff, 0, 1, &Q, &nQ);
    printf("Q = %d, no change\n", Q);

    printf("2 rising edge ----> ");
    D_FLIP_FLOP_logic(&dff, 1, 1, &Q, &nQ);
    printf("Q = %d, change to 1\n", Q);

    printf("3 ----> ");
    D_FLIP_FLOP_logic(&dff, 0, 0, &Q, &nQ);
    printf("Q = %d, no change\n", Q);

    printf("4 rising edge ----> ");
    D_FLIP_FLOP_logic(&dff, 1, 0, &Q, &nQ);
    printf("Q = %d, change to 0\n", Q);

    printf("5 ----> ");
    D_FLIP_FLOP_logic(&dff, 0, 1, &Q, &nQ);
    printf("Q = %d, no change\n", Q);

    printf("6 rising edge ----> ");
    D_FLIP_FLOP_logic(&dff, 1, 1, &Q, &nQ);
    printf("Q = %d, change to 1\n", Q);
}

void print_full_adder_test() {
    printf("\nFull adder test\n");
    int sum, Cout;

    logic_full_adder(0, 0, 0, &sum, &Cout);
    printf("(0, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(0, 0, 1, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(0, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(0, 1, 1, &sum, &Cout);
    printf("(0, 1) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(1, 0, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(1, 0, 1, &sum, &Cout);
    printf("(0, 1) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(1, 1, 0, &sum, &Cout);
    printf("(0, 1) --> sum = %d, Cout = %d\n", sum, Cout);

    logic_full_adder(1, 1, 1, &sum, &Cout);
    printf("(1, 1) --> sum = %d, Cout = %d\n", sum, Cout);
}

void print_one_bit_accumulator_test() {
    printf("\nOne bit accumulator test\n");
    ONE_BIT_ACCUMULATOR OBA;
    ONE_BIT_ACCUMULATOR_init(&OBA, 0);

    int sum, Cout;

    printf("1 ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 0, 0, 0, &sum, &Cout);
    printf("(0, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("2 ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 0, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("3 rising edge ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 1, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("4 ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 0, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("5 rising edge ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 1, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("6 ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 0, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("7 rising edge ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 1, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("8 ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 0, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

    printf("9 rising edge ----> ");
    ONE_BIT_ACCUMULATOR_logic(&OBA, 1, 1, 0, &sum, &Cout);
    printf("(1, 0) --> sum = %d, Cout = %d\n", sum, Cout);

}

void print_four_bit_accumulator_test() {
    printf("\nFour bit accumulator\n");
    FOUR_BIT_ACCUMULATOR FBA;
    FOUR_BIT_ACCUMULATOR_init(&FBA, 0);

    char sum[5];
    int Cout;

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 1, "0010", 0, sum, &Cout);
    printf("%s --> sum = 0010, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 0, "0000", 0, sum, &Cout);
    printf("\tfalling-edge %s --> same: sum = 0010, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 1, "0100", 0, sum, &Cout);
    printf("%s --> sum = 0110, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 0, "0000", 0, sum, &Cout);
    printf("\tfalling-edge %s --> same: sum = 0110, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 1, "1000", 0, sum, &Cout);
    printf("%s --> sum = 1110, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 0, "0000", 0, sum, &Cout);
    printf("\tfalling-edge %s --> same: sum = 1110, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 1, "0001", 0, sum, &Cout);
    printf("%s --> sum = 1111, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 0, "0000", 0, sum, &Cout);
    printf("\tfalling-edge %s --> same: sum = 1110, Cout = %d\n", sum, Cout);

    FOUR_BIT_ACCUMULATOR_logic(&FBA, 1, "0001", 0, sum, &Cout);
    printf("%s --> sum = 1110 with carry, Cout = %d\n", sum, Cout);
}

void print_n_bit_accumulator_test() {
    printf("\nN bit accumulator\n");
    N_BIT_ACCUMULATOR NBA;
    N_BIT_ACCUMULATOR_init(&NBA, 8, 0);

    char sum[9];
    int Cout;

    // Test cases for N BIT ACCUMULATOR
    N_BIT_ACCUMULATOR_logic(&NBA, 1, "00000010", 0, sum, &Cout);
    printf("%s --> sum = 00000010, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 0, "00000000", 0, sum, &Cout);
    printf("\tfallback-edge %s --> same: sum = 00000010, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 1, "00000100", 0, sum, &Cout);
    printf("%s --> sum = 00000110, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 0, "00000000", 0, sum, &Cout);
    printf("\tfallback-edge %s --> same: sum = 00000110, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 1, "00100000", 0, sum, &Cout);
    printf("%s --> sum = 00100110, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 0, "00000000", 0, sum, &Cout);
    printf("\tfallback-edge %s --> same: sum = 00100110, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 1, "00001000", 0, sum, &Cout);
    printf("%s --> sum = 00101110, Cout = %d\n", sum, Cout);

    N_BIT_ACCUMULATOR_logic(&NBA, 0, "00000000", 0, sum, &Cout);
    printf("\tfallback-edge %s --> same: sum = 00101110, Cout = %d\n", sum, Cout);
    
    N_BIT_ACCUMULATOR_logic(&NBA, 1, "11111111", 0, sum, &Cout);
    printf("%s --> sum = 11101101 with carry, Cout = %d\n", sum, Cout);
}

int main() {
    print_d_flip_flop_test();
    print_full_adder_test();
    print_one_bit_accumulator_test();
    print_four_bit_accumulator_test();
    print_n_bit_accumulator_test();

    getch();

    return 0;
}
