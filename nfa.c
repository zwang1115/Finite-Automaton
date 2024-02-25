#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nfa.h"
#include "IntHashSet.h"
#include <string.h>
struct NFA {
    int nstates;
    IntHashSet **transitionTables;
    bool * acceptingStatesTable;
};

NFA new_NFA(int nstates) {
    NFA nfa = (NFA)malloc(sizeof(struct NFA));
    nfa->nstates = nstates;
    nfa->transitionTables = (IntHashSet **)malloc(nstates * sizeof(IntHashSet *));
    for (int i = 0; i < nstates; i++) {
        nfa->transitionTables[i] = (IntHashSet *)malloc(128 * sizeof(IntHashSet));
        for (int j = 0; j < 128; j++) {
            nfa->transitionTables[i][j] = new_IntHashSet(nstates);
        }
    }
    nfa->acceptingStatesTable = (bool *)malloc(nstates * sizeof(bool));
    for (int i = 0; i < nstates; i++) {
        nfa->acceptingStatesTable[i] = false;
    }
    return nfa;
}


void NFA_free(NFA nfa) {
    for (int i = 0; i < nfa->nstates; i++) {
        for (int j = 0; j < 128; j++) {
            free(nfa->transitionTables[i][j]);
        }
        free(nfa->transitionTables[i]);
    }
    free(nfa->transitionTables);
    free(nfa->acceptingStatesTable);
    free(nfa);
}


int NFA_get_size(NFA nfa) {
    return nfa->nstates;
}

Set NFA_get_transitions(NFA nfa, int state, char sym) {
    return nfa->transitionTables[state][(int)sym];
}

void NFA_add_transition(NFA nfa, int src, char sym, int dst) {
    IntHashSet_insert(nfa->transitionTables[src][(int)sym], dst);
}

void NFA_add_transition_str(NFA nfa, int src, char *str, int dst) {
    for (int i = 0; str[i] != '\0'; i++) {
        NFA_add_transition(nfa, src, str[i], dst);
    }
}

void NFA_add_transition_all(NFA nfa, int src, int dst) {
    for (int i = 0; i < 128; i++) {
        NFA_add_transition(nfa, src, (char)i, dst);
    }
}

void NFA_set_accepting(NFA nfa, int state, bool value) {
    nfa->acceptingStatesTable[state] = value;
}

bool NFA_get_accepting(NFA nfa, int state) {
    return nfa->acceptingStatesTable[state];
}


bool NFA_execute(NFA nfa, char *input) {
    IntHashSet cur_state = new_IntHashSet(nfa->nstates);
    IntHashSet_insert(cur_state, 0);
    for (int i = 0; input[i] != '\0'; i++) {
        IntHashSet next_state = new_IntHashSet(nfa->nstates);
        IntHashSetIterator iterator = IntHashSet_iterator(cur_state);
        while (IntHashSetIterator_hasNext(iterator)) {
            int cur = IntHashSetIterator_next(iterator);
            IntHashSet transitions = NFA_get_transitions(nfa, cur, input[i]);
            if (transitions != NULL) {
                IntHashSet_union(next_state, transitions);
            }
        }
        cur_state = next_state;
    }
    
    bool accepts = false;
    IntHashSetIterator acceptIterator = IntHashSet_iterator(cur_state);
    while (IntHashSetIterator_hasNext(acceptIterator)) {
        int cur = IntHashSetIterator_next(acceptIterator);
        if (nfa->acceptingStatesTable[cur] == true) {
            accepts = true;
            break;
        }
    }
    return accepts;
} 


void NFA_repl(NFA nfa) {
    char input[1024];
    printf("Enter an input (\"quit\" to quit): ");
    while(fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if(strcmp(input, "quit") == 0) {
            break;
        }
        printf("Result for input \"%s\": %s\n", input, NFA_execute(nfa, input) ? "true" : "false");
        printf("Enter an input (or 'quit' to quit): ");
    }
    NFA_free(nfa);
}



NFA end_with_ked() {
    NFA nfa = new_NFA(4);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition(nfa, 0, 'k', 1);
    NFA_add_transition(nfa, 1, 'e', 2);
    NFA_add_transition(nfa, 2, 'd', 3);
    NFA_set_accepting(nfa, 3, true);
    return nfa;
}

NFA contains_ath() {
    NFA nfa = new_NFA(4);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition_all(nfa, 3, 3);
    NFA_add_transition(nfa, 0, 'a', 1);
    NFA_add_transition(nfa, 1, 't', 2);
    NFA_add_transition(nfa, 2, 'h', 3);
    NFA_set_accepting(nfa, 3, true);
    return nfa;
}

NFA NFA_3() {
    NFA nfa = new_NFA(13);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition_all(nfa, 1, 1);
    NFA_add_transition_all(nfa, 2, 2);
    NFA_add_transition_all(nfa, 3, 3);
    NFA_add_transition_all(nfa, 4, 4);
    NFA_add_transition_all(nfa, 5, 5);
    NFA_add_transition_all(nfa, 6, 6);
    NFA_add_transition_all(nfa, 7, 7);
    NFA_add_transition_all(nfa, 8, 8);
    NFA_add_transition_all(nfa, 9, 9);
    NFA_add_transition_all(nfa, 10, 10);
    NFA_add_transition_all(nfa, 11, 11);
    NFA_add_transition(nfa, 0, 'o', 1);
    NFA_add_transition(nfa, 1, 'o', 11);
    NFA_add_transition(nfa, 0, 'f', 2);
    NFA_add_transition(nfa, 2, 'f', 11);
    NFA_add_transition(nfa, 0, 'r', 3);
    NFA_add_transition(nfa, 3, 'r', 11);
    NFA_add_transition(nfa, 0, 'c', 4);
    NFA_add_transition(nfa, 4, 'c', 5);
    NFA_add_transition(nfa, 5, 'c', 11);
    NFA_add_transition(nfa, 0, 'n', 6);
    NFA_add_transition(nfa, 6, 'n', 7);
    NFA_add_transition(nfa, 7, 'n', 11);
    
    NFA_add_transition(nfa, 0, 'e', 8);
    NFA_add_transition(nfa, 8, 'e', 9);
    NFA_add_transition(nfa, 9, 'e', 10);
    NFA_add_transition(nfa, 10, 'e', 11);

    NFA_set_accepting(nfa, 11, true);
    return nfa;
}