#include "utility.h"

int number_of_occurences_suffix(int *pat, int *txt, int *SA, int n, int m);
int number_of_occurences_automata(int m, int *txt, int **Transition_function, int n);
void make_transition_function(int* pat, int m, int **Transition_function, int sigma_size);
void suffix_array(int * txt, int *SA, int size, int max_key);
