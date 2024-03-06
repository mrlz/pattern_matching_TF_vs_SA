#include "pattern_matching.h"

int compare_arrays(int *pattern, int *txt, int m, int start){
  int j = 0;
  while(j < m){
    if (pattern[j] < txt[start+j]){
      return -1;
    }else if (pattern[j] > txt[start+j]){
      return 1;
    }
    j++;
  }
  return 0;
}

int search_first(int *pat, int *txt, int *suffArr, int n, int m){
    int l = 0, r = n-1;
    while (l <= r){
        int mid = l + (r - l)/2;
        int res = compare_arrays(pat, txt, m, suffArr[mid]);
        if (res > 0){
          l = mid + 1;
        }else if (res < 0){
          r = mid - 1;
        }else if (l != mid){
          r = mid;
        }else{
          return mid;
        }
      }
    return -1;
}

int search_last(int *pat, int* txt, int *suffArr, int n, int m){
    int l = 0, r = n-1;
    while (l <= r){
        int mid = ceil(l + (r - l)/2.0);
        int res = compare_arrays(pat, txt, m, suffArr[mid]);
        if (res > 0){
          l = mid + 1;
        }else if (res < 0){
          r = mid - 1;
        }else if (r != mid){
          l = mid;
        }else{
          return mid;
        }
      }
    return -1;
}

void radix_sort(int *input_array, int *output_array, int* keys, int size, int max_key){
  int *counts = new int[max_key+1]();
  int i = 0;
  while(i < size){
    counts[keys[input_array[i]]]++; //we add 1 occurence to the key(input_array[i]) slot
    i++;
  }
  i = 0;
  int sum = 0;
  while(i <= max_key){
    int previous_count = counts[i];
    counts[i] = sum;
    sum = sum + previous_count;
    i++;
  }
  i = 0;
  while(i < size){
    output_array[counts[keys[input_array[i]]]] = input_array[i];
    counts[keys[input_array[i]]]++;
    i++;
  }
}

bool is_unique(int *txt, int *SA_alfabeta, int* current_key, int i){
  if (txt[SA_alfabeta[i]] != current_key[0] || txt[SA_alfabeta[i]+1] != current_key[1] || txt[SA_alfabeta[i]+2] != current_key[2]){
    return true;
  }else{
    return false;
  }
}

void make_transition_function(int *pat, int m, int **Transition_function, int sigma_size){
    int i = 1;
    int j = 0;
    int X = 0;
    while(j <= sigma_size){
      Transition_function[0][j] = 0;
      j++;
    }

    Transition_function[0][pat[0]] = 1;

    while(i<m){
      j = 0;
      while(j <= sigma_size){
        Transition_function[i][j] = Transition_function[X][j];
        j++;
      }
      Transition_function[i][pat[i]] = i + 1;
        X = Transition_function[X][pat[i]];
      i++;
    }
}

int number_of_occurences_automata(int m, int *txt, int **Transition_function, int n){
    int occurences = 0;

    int i = 0;
    int j = 0;
    while (i < n){
       j = Transition_function[j][txt[i]];
       if (j == m){
         occurences++;
         j = 0;
       }
       i++;
    }
    return occurences;
}

bool leq(int a1, int a2,   int b1, int b2) {
  return (a1 < b1 || a1 == b1 && a2 < b2);
}
bool leq(int a1, int a2, int a3,   int b1, int b2, int b3) {
  return (a1 < b1 || a1 == b1 && leq(a2,a3, b2,b3));
}

int number_of_occurences_suffix(int *pat, int* txt, int *SA, int n, int m){
  int first = search_first(pat,txt,SA,n,m);
  int last = search_last(pat,txt,SA,n,m);
  if (first+last != -2){
  return 1 + (last-first);
  }
  return 0;
}

int get_index(int slot, int alfa_size){
  if (slot < alfa_size){
    return slot*3;
  }else{
    return (slot-alfa_size)*3 + 1;
  }
}

void merge(int *SA, int *R_prime, int *sorted_gammas, int *txt, int *SA_R_prime, int size, int alfa_size, int gamma_size, int alfabeta_size){
  int SA_slot = 0;
  int gamma_slot = 0;
  int k = 0;
  while(k < size){
    int j = sorted_gammas[gamma_slot];
    int i = get_index(SA_R_prime[SA_slot],alfa_size);
    int alfabeta_smaller = 0;
    if (i % 3 == 0){
      alfabeta_smaller = leq(txt[i], R_prime[SA_R_prime[SA_slot]+alfa_size], txt[j], R_prime[(j+1)/3]);
    }else{
      alfabeta_smaller = leq(txt[i], txt[i+1], R_prime[SA_R_prime[SA_slot]+1-alfa_size], txt[j], txt[j+1], R_prime[alfa_size + (j+1)/3]);
    }

    if (alfabeta_smaller){
      SA[k] = i;
      SA_slot++;
      if (SA_slot == alfabeta_size){
        k++;
        while(gamma_slot < gamma_size){
          SA[k] = sorted_gammas[gamma_slot];
          gamma_slot++;
          k++;
        }
      }
    }else{
      SA[k] = j;
      gamma_slot++;
      if (gamma_slot == gamma_size){
        k++;
        while(SA_slot < alfabeta_size){
          SA[k] = get_index(SA_R_prime[SA_slot], alfa_size);
          k++;
          SA_slot++;
        }
      }
    }

    k++;
  }
}

void suffix_array(int * txt, int *SA, int size, int max_key){
  //txt must have 3 0s at the end, which are not reported in the size
  int j = 0;
  int alfabeta_size = (size+2)/3 + (size+1)/3;
  int gamma_size = size/3;
  int *R = new int[alfabeta_size];
  int *R_prime = new int[alfabeta_size+1]();
  int *T_alfabeta = new int[alfabeta_size];
  int *T_gamma = new int[gamma_size];
  int alfa_size = (size+2)/3;

  int i = 0;
  int slot_alfa = 0;
  int slot_beta = 0;
  int slot_gamma = 0;

  while(i < size){
    if (i % 3 == 0){
      T_alfabeta[slot_alfa] = i;
      slot_alfa++;
    }else if(i %3 == 1){
      T_alfabeta[slot_beta + (size+2)/3] = i;
      slot_beta++;
    }else{
      T_gamma[slot_gamma] = i;
      slot_gamma++;
    }
    i++;
  }

  radix_sort(T_alfabeta, R, txt+2, alfabeta_size, max_key);
  radix_sort(R, R_prime, txt+1, alfabeta_size, max_key);
  radix_sort(R_prime, R, txt, alfabeta_size, max_key);

  int name_offset = 0;

  int unique_names = 0;
  int *current_key = new int[3]{-1,-1,-1};
  int not_unique = 0;
  i = 0;
  while(i < alfabeta_size){
    if(is_unique(txt, R, current_key, i)){
      unique_names = unique_names + 1 + name_offset;
      current_key[0] = txt[R[i]];
      current_key[1] = txt[R[i]+1];
      current_key[2] = txt[R[i]+2];
      name_offset = 0;
    }else{
      name_offset++;
      not_unique++;
    }
    if (R[i]%3 == 1){
      int index = R[i]/3;
      R_prime[index+alfa_size] = unique_names;
    }else{
      int index = (R[i])/3;
      R_prime[index] = unique_names;
    }
    i++;
  }


  int *SA_R_prime = new int[alfabeta_size]; //This will contain the suffixes of alfabeta, in order
  if (not_unique>0){
    suffix_array(R_prime, SA_R_prime, alfabeta_size,unique_names);
    int k = 0;
    while(k < alfabeta_size){
      R_prime[SA_R_prime[k]] = k+1;
      k++;
    }
  }else{
    int k = 0;
    while(k < alfabeta_size){
      SA_R_prime[R_prime[k]-1] = k;
      k++;
    }
  }

  //R_prime has become it's inverse suffix array
  //and R_prime is, therefore, also the N_i of each suffix, with respect to the
  //order in R (first the alfa elements, then the beta elements)
  //R_prime[i] corresponds to N_i of i*3 position (char) if i < alfa_size
  //and N_i of (i-alfa_size)*3 + 1 else

  int *gamma_sort = new int[gamma_size];
  int *gamma_index = new int[gamma_size];
  int gamma_key = 0;
  int g = 0;
  while(g < gamma_size){
    int c = T_gamma[g];
    int index = (c+1)/3;
    int N_i;
    if (index == alfa_size){
      N_i = 0;
    }else{
      N_i = R_prime[index];
    }
    gamma_key = std::max(gamma_key, N_i);
    gamma_index[g] = g;
    gamma_sort[g] = N_i;
    g++;
  }

  int *sorted_gammas = new int[gamma_size];
  radix_sort(gamma_index, sorted_gammas, gamma_sort, gamma_size, gamma_key);
  g = 0;
  while(g < gamma_size){
    gamma_sort[g] = T_gamma[sorted_gammas[g]];
    g++;
  }
  radix_sort(gamma_sort, sorted_gammas, txt, gamma_size, max_key);

  merge(SA, R_prime, sorted_gammas, txt, SA_R_prime, size, alfa_size, gamma_size, alfabeta_size);

  delete[] R;
  delete[] gamma_sort;
  delete[] gamma_index;
  delete[] R_prime;
  delete[] T_alfabeta;
  delete[] sorted_gammas;
  delete[] T_gamma;
}
