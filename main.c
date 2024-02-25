#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dfa.h"
#include "nfa.h"
#include <string.h>
#include "nfaTodfa.h"

int main(int argc, char const *argv[]) {
    //DFAS
    printf("Testing DFA that recognizes the exactly the string dfa. \n");
    DFA_repl(contains_string_dfa());
    printf("Testing DFA that recognizes strings starting with cat. \n");
    DFA_repl(starts_with_cat());
    printf("Testing DFA that recognizes strings contain exactly two 2s. \n");
    DFA_repl(contains_exactly_two_2s());
    printf("Testing DFA that reconginizes strings contain an even number of 0’s and an odd number of 1’s. \n");
    DFA_repl(binary_inputs());

    // //NFAS
    printf("Testing NFA that recognizes strings ending with ked. \n");
    NFA_repl(end_with_ked());
    printf("Testing NFA that recognizes strings contain ath. \n");
    NFA_repl(contains_ath());
    printf("Testing NFA that recognizes strings that have more than one o, f, or r, or more than two c’s or n’s, or more than three e’s. \n");
    NFA_repl(NFA_3());

    //Subset_constructions
    printf("Tesing NFA to DFA conversions.\n");
    printf("Testing NFA that recognizes strings ending with ked. \n");
    printf("Number of states: %d\n", DFA_get_size(NFA_to_DFA(end_with_ked())));
    DFA_repl(NFA_to_DFA(end_with_ked()));
    printf("Testing NFA that recognizes strings contain ath. \n");
    printf("Number of states: %d\n", DFA_get_size(NFA_to_DFA(contains_ath())));
    DFA_repl(NFA_to_DFA(contains_ath()));
    printf("Testing NFA that recognizes strings that have more than one o, f, or r, or more than two c’s or n’s, or more than three e’s. \n");
    printf("Number of states: %d\n", DFA_get_size(NFA_to_DFA(NFA_3())));
    DFA_repl(NFA_to_DFA(NFA_3()));

}
