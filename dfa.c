#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dfa.h"
#include "IntHashSet.h"
#include <string.h>
struct DFA {
    int nstates;
    int **transitionTables;
    bool * acceptingStatesTable;
};

DFA new_DFA(int nstates) {
    DFA dfa = (DFA)malloc(sizeof(struct DFA));
    dfa->nstates = nstates;
    dfa->transitionTables = (int **)malloc(nstates * sizeof(int *));
    for (int i = 0; i < nstates; i++) {
        dfa->transitionTables[i] = (int *)malloc(128 * sizeof(int));
        for (int j = 0; j < 128; j++) {
            dfa->transitionTables[i][j] = -1;
        }
    }
    dfa->acceptingStatesTable = (bool *)malloc(nstates * sizeof(bool));
    for (int i = 0; i < nstates; i++) {
        dfa->acceptingStatesTable[i] = false;
    }
    return dfa;
}

void DFA_free(DFA dfa) {
    for (int i = 0; i < dfa->nstates; i++) {
        free(dfa->transitionTables[i]);
    }
    free(dfa->transitionTables);
    free(dfa->acceptingStatesTable);
    free(dfa);
}

int DFA_get_size(DFA dfa) {
    return dfa->nstates;
}

int DFA_get_transition(DFA dfa, int src, char sym) {
    return dfa->transitionTables[src][(int)sym];
}

void DFA_set_transition(DFA dfa, int src, char sym, int dst) {
    dfa->transitionTables[src][(int)sym] = dst;
}

void DFA_set_transition_str(DFA dfa, int src, char *str, int dst) {
    for (int i = 0; str[i] != '\0'; i++) {
        DFA_set_transition(dfa, src, str[i], dst);
    }
}

void DFA_set_transition_all(DFA dfa, int src, int dst) {
    for (int i = 0; i < 128; i++) {
        DFA_set_transition(dfa, src, (char)i, dst);
    }
}

void DFA_set_accepting(DFA dfa, int state, bool value) {
    dfa->acceptingStatesTable[state] = value;
}

bool DFA_get_accepting(DFA dfa, int state) {
    return dfa->acceptingStatesTable[state];
}

bool DFA_execute(DFA dfa, char *input) {
    int cur_state = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        int next_state = DFA_get_transition(dfa, cur_state, input[i]);
        if (next_state == -1) {
            return false;
        }
        cur_state = next_state;
    }
    return DFA_get_accepting(dfa, cur_state);
}

void DFA_repl(DFA dfa) {
    char input[1024];
    printf("Enter an input (\"quit\" to quit): ");
    while(fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if(strcmp(input, "quit") == 0) {
            break;
        }
        printf("Result for input \"%s\": %s\n", input, DFA_execute(dfa, input) ? "true" : "false");
        printf("Enter an input (or 'quit' to quit): ");
    }
    DFA_free(dfa);
}


DFA contains_string_dfa() {
    DFA dfa = new_DFA(4);
    DFA_set_transition(dfa, 0, 'd', 1);
    DFA_set_transition(dfa, 1, 'f', 2);
    DFA_set_transition(dfa, 2, 'a', 3);
    DFA_set_accepting(dfa, 3, true);
    return dfa;
}

DFA starts_with_cat() {
    DFA dfa = new_DFA(4);
    DFA_set_transition(dfa, 0, 'c', 1);
    DFA_set_transition(dfa, 1, 'a', 2);
    DFA_set_transition(dfa, 2, 't', 3);
    DFA_set_transition_all(dfa, 3, 3);
    DFA_set_accepting(dfa, 3, true);
    return dfa;
}

DFA contains_exactly_two_2s() {
    DFA dfa = new_DFA(3);
    DFA_set_transition(dfa, 0, '2', 1);
    DFA_set_transition(dfa, 1, '2', 2);
    for (int i = 0; i < 128; i++) {
        if (i != '2') {
            DFA_set_transition(dfa, 0, i, 0);
            DFA_set_transition(dfa, 1, i, 1);
            DFA_set_transition(dfa, 2, i, 2);
        }
    }
    DFA_set_accepting(dfa, 2, true);
    return dfa;
}

DFA binary_inputs() {
    DFA dfa = new_DFA(4);
    DFA_set_transition(dfa, 0, '1', 1);
    DFA_set_transition(dfa, 1, '1', 0);
    DFA_set_transition(dfa, 1, '0', 3);
    DFA_set_transition(dfa, 3, '0', 1);
    DFA_set_transition(dfa, 3, '1', 2);
    DFA_set_transition(dfa, 2, '1', 3);
    DFA_set_transition(dfa, 2, '0', 0);
    DFA_set_transition(dfa, 0, '0', 2);
    DFA_set_accepting(dfa, 1, true);
    return dfa;
}



