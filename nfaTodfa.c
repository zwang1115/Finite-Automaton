#include "nfaTodfa.h"
#include "LinkedList.h"

DFA NFA_to_DFA(NFA nfa) {
    LinkedList list = new_LinkedList();
    IntHashSet set = new_IntHashSet(128);
    IntHashSet_insert(set, 0);
    LinkedList_add_at_front(list, set);
    int index = 0;
    int list_size = 1;
    while (index < list_size) {
        IntHashSet cur_state = LinkedList_elementAt(list, index);
        for (int i = 0; i < 128; i++) {
            IntHashSet new_state = new_IntHashSet(128);
            IntHashSetIterator iterator = IntHashSet_iterator(cur_state);
            while (IntHashSetIterator_hasNext(iterator)) {
                int state = IntHashSetIterator_next(iterator);
                IntHashSet_union(new_state, NFA_get_transitions(nfa, state, (char)i));
            }
            bool found = false;
            for (int j = 0; j < list_size; j++) {
                IntHashSet existing_state = LinkedList_elementAt(list, j);
                if (IntHashSet_equals(new_state, existing_state)) {
                    found = true;
                    break;
                }
            }

            if (!found && !IntHashSet_isEmpty(new_state)) {
                LinkedList_add_at_end(list, new_state);
                list_size++;
            } else {
                IntHashSet_free(new_state);
            }
        }
        index++;
    }

    DFA dfa = new_DFA(list_size);
    for (int i = 0; i < list_size; i++) {
        IntHashSet set = LinkedList_elementAt(list, i);
        for (int j = 0; j < 128; j++) {
            IntHashSetIterator iterator = IntHashSet_iterator(set);
            IntHashSet set2 = new_IntHashSet(list_size);
            while (IntHashSetIterator_hasNext(iterator)) {
                IntHashSet_union(set2, NFA_get_transitions(nfa, IntHashSetIterator_next(iterator), (char)j));
            }
            int idx = 0;
            while (idx < list_size && !IntHashSet_equals(set2, LinkedList_elementAt(list, idx))) {
                idx++;
            }
            DFA_set_transition(dfa, i, (char)j, idx);
            IntHashSet_free(set2);
        }
    }
    LinkedListIterator dfaListIterator = LinkedList_iterator(list);
    int dfa_index = 0;
    while (LinkedListIterator_hasNext(dfaListIterator)) {
        IntHashSet state_set = LinkedListIterator_next(dfaListIterator);
        IntHashSetIterator accepting_iterator = IntHashSet_iterator(state_set);
        while (IntHashSetIterator_hasNext(accepting_iterator)) {
            int cur = IntHashSetIterator_next(accepting_iterator);
            if (NFA_get_accepting(nfa, cur)) {
                DFA_set_accepting(dfa, dfa_index, true);
            }
        }
        dfa_index++;
    }
    LinkedList_free(list, true);
    return dfa;
}
