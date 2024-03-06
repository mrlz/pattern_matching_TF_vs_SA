#include "pattern_matching.h"
#include <random>

std::random_device rand_dev; //these will produce different sequences everytime
std::mt19937 generator(rand_dev());
const int start = 1;
const int end = 36;
std::uniform_int_distribution<int> uniform_distribution(start,end);
int verbose = 1;

void simple_test_suffix(){
  std::string test_string = "supercalifragilisticoespialidosoaunquealmencionarlesuenesuperenredososupercalifragilisticoespialidoso$$$";
  int *conversion = new int[test_string.length()];
  string_to_numbers(test_string,conversion);

  int *SA = new int[test_string.length()-3];
  suffix_array(conversion, SA, test_string.length()-3, 36);

  std::string pat = "listi";
  int *pat_conv = new int[pat.length()];
  string_to_numbers(pat,pat_conv);

  std::cout << "search for pattern " << pat << " in text " << test_string << std::endl;
  std::cout << pat << " occurs " << number_of_occurences_suffix(pat_conv,conversion,SA,test_string.length(), pat.length()) << " times" << std::endl;
  std::cout << "SA is ";
  print_array(SA,test_string.length()-3);
  std::cout << std::endl;
  delete[] pat_conv;
  delete[] conversion;
  delete[] SA;
}

void ned_test_suffix(){
  struct timespec time_start, time_end;

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  std::string test_string = parse_file_txt("./textos/old/got15.txt");
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  double total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Reading: " << total_time << std::endl;

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  int *conversion = new int[test_string.length()+3];
  string_to_numbers(test_string,conversion);
  conversion[test_string.length()] = 0; // s[n] = s[n+1] = s[n+2] = 0
  conversion[test_string.length()+1] = 0;
  conversion[test_string.length()+2] = 0;
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Converting: " << total_time << std::endl;

  int *SA = new int[test_string.length()];
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  suffix_array(conversion,SA,test_string.length(),36);
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Creating SA: " << total_time << std::endl;

  std::string pat = "ned";
  int *pattern_conversion = new int[pat.length()];
  string_to_numbers(pat,pattern_conversion);

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  int occurences = number_of_occurences_suffix(pattern_conversion, conversion, SA, test_string.length(), pat.length());
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Searching for "<< pat << " :" << total_time << std::endl;
  std::cout << occurences << " " << pat << " found" << std::endl;

  delete[] pattern_conversion;
  delete[] conversion;
  delete[] SA;
}

void simple_test_automata(){
    int sigma_size = 36;
    std::string txt = "banana";
    std::string pat = "ana";

    int *conversion = new int[txt.length()+3];
    string_to_numbers(txt,conversion);
    conversion[txt.length()] = 0;
    conversion[txt.length()+1] = 0;
    conversion[txt.length()+2] = 0;

    int *conv_pat = new int[pat.length()];
    string_to_numbers(pat,conv_pat);

    int m = pat.length();

    int **Transition_function = new int*[m+1];
    Transition_function[0] = new int[(m+1) * sigma_size];
    int i = 1;
    while(i < (m+1)){ //Little trick to compress in main memory
    Transition_function[i] = Transition_function[i-1] + sigma_size;
    i++;
    }

    make_transition_function(conv_pat, m, Transition_function, sigma_size);

    std::cout << "ana occurs " << number_of_occurences_automata(m,conversion,Transition_function,txt.length()) << " times" << std::endl;
    delete[] conv_pat;
    delete[] conversion;
    delete[] Transition_function[0];
    delete[] Transition_function;
}

void ned_test_automata(){
  struct timespec time_start, time_end;
  int sigma_size = 36;
  std::string pat = "ned";
  int *conv_pat = new int[pat.length()];
  string_to_numbers(pat,conv_pat);

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  std::string test_string = parse_file_txt("./textos/old/got15.txt");
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  double total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Reading: " << total_time << std::endl;

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  int *conversion = new int[test_string.length()+3];
  string_to_numbers(test_string,conversion);
  conversion[test_string.length()] = 0; // s[n] = s[n+1] = s[n+2] = 0
  conversion[test_string.length()+1] = 0;
  conversion[test_string.length()+2] = 0;
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Converting: " << total_time << std::endl;

  int m = pat.length();


  int **Transition_function = new int*[m+1];
  Transition_function[0] = new int[(m+1) * sigma_size];
  int i = 1;
  while(i < (m+1)){ //Little trick to compress in main memory
  Transition_function[i] = Transition_function[i-1] + sigma_size;
  i++;
  }

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  make_transition_function(conv_pat, m, Transition_function, sigma_size);
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "CREATING TF: " << total_time << std::endl;

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  int occurences = number_of_occurences_automata(m,conversion,Transition_function,test_string.length());
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  std::cout << "Searching for "<< pat << " :" << total_time << std::endl;
  std::cout << occurences << " " << pat << " found" << std::endl;

  delete[] conv_pat;
  delete[] conversion;
  delete[] Transition_function[0];
  delete[] Transition_function;

}

void select_start_pos(int *start_pos, int words, int *word_size, int chars){
  int i = 0;
  int tentative = 0;
  while(i < words){
    make_random_text(1,chars,&tentative);
    tentative--;
    while(tentative + word_size[i] > chars){
      make_random_text(1,chars,&tentative);
    }
    start_pos[i] = tentative;
    i++;
  }
}

void get_time_averages(double *times, int *counts, int max_word_size){
  int i = 0;
  while (i < max_word_size){
    times[i] = times[i]/(double)counts[i];
    i++;
  }
}

void SA_Alphabet_size(double * build_time, double* search_times_by_word, double* search_times_by_word_size, int chars, int sigma_size, int *txt, int* start_pos, int* word_size, int words_to_search, int max_word_size){
  struct timespec time_start, time_end;
  int *SA = new int[chars]();
  int *counts = new int[max_word_size]();

  clock_gettime(CLOCK_MONOTONIC, &time_start);
  suffix_array(txt,SA,chars,sigma_size);
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  double total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  *build_time = total_time;

  int word_search = 0;
  // std::cout << "SA: searching " << words_to_search << " words" << std::endl;
  while(word_search < words_to_search){
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  int occurences = number_of_occurences_suffix(&(txt[start_pos[word_search]]), txt, SA, chars, word_size[word_search]);
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  search_times_by_word[word_search] = total_time;
  search_times_by_word_size[word_size[word_search]-1] = search_times_by_word_size[word_size[word_search]-1] + total_time;
  counts[word_size[word_search]-1]++;
  word_search++;
  }
  // std::cout << " SA total time by word " << sum_array(search_times_by_word, words_to_search) << " total time by m " << sum_array(search_times_by_word_size, max_word_size) << std::endl;
  get_time_averages(search_times_by_word_size,counts, max_word_size);
  delete[] counts;
  delete[] SA;
}

void DFA_Alphabet_size(double* build_time_by_word, double *build_time_by_word_size, double *search_time_by_word, double *search_time_by_word_size, int chars, int sigma_size, int *txt, int* start_pos, int* word_size, int words_to_search, int max_word_size){
  int j = 0;
  struct timespec time_start, time_end;
  int *counts = new int[max_word_size]();
  int **Transition_function = new int*[max_word_size]();
  Transition_function[0] = new int[(max_word_size) * (sigma_size+1)]();
  int i = 1;
  while(i < (max_word_size)){ //Little trick to compress in main memory
  Transition_function[i] = Transition_function[i-1] + (sigma_size+1);
  i++;
  }
  struct timespec time_start_loop, time_end_loop;
  clock_gettime(CLOCK_MONOTONIC, &time_start_loop);
  while(j < words_to_search){
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    make_transition_function(&(txt[start_pos[j]]), word_size[j], Transition_function, sigma_size);
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    double total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
    build_time_by_word[j] = total_time;
    build_time_by_word_size[word_size[j]-1] = build_time_by_word_size[word_size[j]-1] + total_time;
    counts[word_size[j]-1]++;
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    int occurences = number_of_occurences_automata(word_size[j],txt,Transition_function,chars);
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
    search_time_by_word[j] = total_time;
    search_time_by_word_size[word_size[j]-1] = search_time_by_word_size[word_size[j]-1] + total_time;
    j++;
  }
  clock_gettime(CLOCK_MONOTONIC, &time_end_loop);
  delete[] Transition_function[0];
  delete[] Transition_function;
  get_time_averages(search_time_by_word_size, counts, max_word_size);
  get_time_averages(build_time_by_word_size, counts, max_word_size);
  delete[] counts;
}

void copy_add_arrays(double *first, double* second, int size, double iterations){
  int i = 0;
  while(i < size){
    first[i] = first[i] + (second[i]/iterations);
    i++;
  }
}

double* test_alphabet_size(int words, int words_to_search, int sigma_size, int size, int iterations, int max_word_size, double *build_time_SA_avg, double *search_time_by_word_SA_avg, double *search_time_by_word_size_SA_avg, double *build_time_by_word_DFA_avg, double *build_time_by_word_size_DFA_avg, double *search_time_by_word_DFA_avg, double *search_time_by_word_size_DFA_avg){
  int chars = ceil(words*4.3);
  int *txt = new int[chars+3]();
  struct timespec time_start, time_end;
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  make_random_text(chars, sigma_size, txt);
  int *word_size = new int[words_to_search]();
  int *start_pos = new int[words_to_search]();
  *start_pos = chars;
  make_random_text(words_to_search,max_word_size,word_size); //select word sizes for N/10 words
  select_start_pos(start_pos,words_to_search,word_size,chars); //select start position of those N/10 words
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  double total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
  double *build_time_SA = new double();
  double *search_time_by_word_SA = new double[words_to_search]();
  double *search_time_by_word_size_SA = new double[max_word_size]();

  double *build_time_by_word_DFA = new double[words_to_search]();
  double *build_time_by_word_size_DFA = new double[max_word_size]();
  double *search_time_by_word_DFA = new double[words_to_search]();
  double *search_time_by_word_size_DFA = new double[max_word_size]();

  int it = 0;
  clock_gettime(CLOCK_MONOTONIC, &time_start);
  while(it < iterations){
    struct timespec time_start_in, time_end_in;
    clock_gettime(CLOCK_MONOTONIC, &time_start_in);
    SA_Alphabet_size(build_time_SA, search_time_by_word_SA, search_time_by_word_size_SA, chars, sigma_size, txt, start_pos, word_size, words_to_search, max_word_size);
    clock_gettime(CLOCK_MONOTONIC, &time_end_in);
    clock_gettime(CLOCK_MONOTONIC, &time_start_in);
    DFA_Alphabet_size(build_time_by_word_DFA, build_time_by_word_size_DFA, search_time_by_word_DFA, search_time_by_word_size_DFA,chars,sigma_size, txt, start_pos, word_size,words_to_search, max_word_size);
    clock_gettime(CLOCK_MONOTONIC, &time_end_in);
    copy_add_arrays(build_time_SA_avg, build_time_SA, 1, (double) iterations);
    copy_add_arrays(search_time_by_word_SA_avg, search_time_by_word_SA, words_to_search, (double) iterations);
    copy_add_arrays(search_time_by_word_size_SA_avg, search_time_by_word_size_SA, max_word_size, (double) iterations);
    copy_add_arrays(build_time_by_word_DFA_avg, build_time_by_word_DFA, words_to_search, (double) iterations);
    copy_add_arrays(build_time_by_word_size_DFA_avg, build_time_by_word_size_DFA, max_word_size, (double) iterations);
    copy_add_arrays(search_time_by_word_DFA_avg, search_time_by_word_DFA, words_to_search, (double) iterations);
    copy_add_arrays(search_time_by_word_size_DFA_avg, search_time_by_word_size_DFA, max_word_size, (double) iterations);
    it++;
  }
  clock_gettime(CLOCK_MONOTONIC, &time_end);
  total_time = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));

  delete build_time_SA;
  delete[] search_time_by_word_SA;
  delete[] search_time_by_word_size_SA;
  delete[] build_time_by_word_DFA;
  delete[] build_time_by_word_size_DFA;
  delete[] search_time_by_word_DFA;
  delete[] search_time_by_word_size_DFA;

  delete[] start_pos;
  delete[] word_size;
  delete[] txt;
}

void print_m_rows(std::ofstream& out, int sigma_size, int size, double *datum, int max_word_size){
int i = 0;
while(i < max_word_size){
  out << sigma_size << ", " << size << ", " << datum[i] << ", " << i+1 << std::endl;
  i++;
}
}

void print_m_rows_last_file(std::ofstream& out, int sigma_size, int size, int max_word_size, double build_time_SA , double* search_time_by_word_size_SA_avg, double* build_time_by_word_size_DFA, double* search_time_by_word_size_DFA_avg){
  int i = 0;
  while(i < max_word_size){
    out << "SA, " << build_time_SA + search_time_by_word_size_SA_avg[i] << ", " << sigma_size << ", " << size << ", " << i+1 << std::endl;
    out << "DFA, " << build_time_by_word_size_DFA[i] + search_time_by_word_size_DFA_avg[i] << ", " << sigma_size << ", " << size << ", " << i+1 << std::endl;
    i++;
  }
}

double search_threshold(double avg_bt_sa, double avg_bt_dfa, double avg_st_dfa, double avg_st_sa){
  return avg_bt_sa/(avg_st_dfa + avg_bt_dfa - avg_st_sa);
}

void print_to_files(int words_to_search, int max_word_size, int sigma_size, int n, std::ofstream& out_sigma_size, std::ofstream& out_SA_build, std::ofstream& out_SA_search, std::ofstream& out_SA_search_m, std::ofstream& out_DFA_build, std::ofstream& out_DFA_search, std::ofstream& out_DFA_build_m, std::ofstream& out_DFA_search_m, std::ofstream& out_sigma_size_m , double *build_time_SA_avg, double *search_time_by_word_SA_avg, double *search_time_by_word_size_SA_avg, double *build_time_by_word_DFA_avg, double *build_time_by_word_size_DFA_avg, double *search_time_by_word_DFA_avg, double *search_time_by_word_size_DFA_avg){

double search_time_sa = sum_array(search_time_by_word_SA_avg, words_to_search);
double build_time_dfa = sum_array(build_time_by_word_DFA_avg, words_to_search);
double search_time_dfa = sum_array(search_time_by_word_DFA_avg, words_to_search);

double threshold = search_threshold(*build_time_SA_avg, build_time_dfa/(double)words_to_search, search_time_dfa/(double)words_to_search, search_time_sa/(double)words_to_search );
out_sigma_size << "SA, " << *build_time_SA_avg + search_time_sa << ", " << sigma_size << ", " << n << ", "  << *build_time_SA_avg << ", "    << search_time_sa << ", "<< search_time_sa/(double)words_to_search << ", " << search_time_sa/(double)words_to_search + *build_time_SA_avg << ", " << threshold << std::endl;
out_sigma_size << "DFA, " << build_time_dfa + search_time_dfa << ", " << sigma_size << ", " << n << ", " << build_time_dfa << ", " << search_time_dfa << ", " <<search_time_dfa/(double)words_to_search << ", "<<(build_time_dfa + search_time_dfa)/(double)words_to_search << ", " << threshold << std::endl;
out_SA_build <<   sigma_size << ", " << n << ", " << *build_time_SA_avg  << std::endl;
out_SA_search <<    sigma_size << ", " << n << ", " << search_time_sa  << std::endl;
print_m_rows(out_SA_search_m, sigma_size, n, search_time_by_word_size_SA_avg,max_word_size);
out_DFA_build <<   sigma_size << ", " << n << ", " << build_time_dfa << std::endl;
out_DFA_search <<      sigma_size << ", " << n << ", " << search_time_dfa  << std::endl;
print_m_rows(out_DFA_build_m,sigma_size,n,build_time_by_word_size_DFA_avg,max_word_size);
print_m_rows(out_DFA_search_m,sigma_size,n,search_time_by_word_size_DFA_avg,max_word_size);
print_m_rows_last_file(out_sigma_size_m,sigma_size, n, max_word_size, *build_time_SA_avg, search_time_by_word_size_SA_avg, build_time_by_word_size_DFA_avg, search_time_by_word_size_DFA_avg);
}


void sigma_vs_size_vs_patternsize_random(std::string filename, int min_size, int max_size, int min_sigma_size, int max_sigma_size, int max_word_size, int inner_iterations, int outer_iterations){
  //In this file we'll explore the performance of each algorithm for all pairs |Σ| x n
  std::ofstream out_sigma_size(filename+"_sigma_size.csv");
  out_sigma_size << "Alg, total_time, sigma_size, n, build_time, search_time, avg_search_time_1_word, avg_total_time_1_word, search_threshold" << std::endl;
  //The following files allow us to take a deeper look into the performance (and bounds) of SA
  std::ofstream out_SA_build(filename+"_SA_build.csv");
  out_SA_build << "sigma_size, n, build_time" << std::endl;
  std::ofstream out_SA_search(filename+"_SA_search.csv");
  out_SA_search << "sigma_size, n, search_time" << std::endl;
  std::ofstream out_SA_search_m(filename+"_SA_search_m.csv");
  out_SA_search_m << "sigma_size, n, search_time, m" << std::endl;
  //The following files allow us to take a deeper look into the performance (and bounds) of DFA_Alphabet_size
  std::ofstream out_DFA_build(filename+"_DFA_build.csv");
  out_DFA_build << "sigma_size, n, build_time" << std::endl;
  std::ofstream out_DFA_search(filename+"_DFA_search.csv");
  out_DFA_search << "sigma_size, n, search_time" << std::endl;
  std::ofstream out_DFA_build_m(filename+"_DFA_build_m.csv");
  out_DFA_build_m << "sigma_size, n, build_time, m" << std::endl;
  std::ofstream out_DFA_search_m(filename+"_DFA_search_m.csv");
  out_DFA_search_m << "sigma_size, n, search_time, m" << std::endl;
  //Finally, we'll explore if the granularity of m allows us to find a 3-tuple (|Σ|, n, m) which defies the results
  //obtained in the first file
  std::ofstream out_sigma_size_m(filename+"_sigma_size_m.csv");
  out_sigma_size_m << "Alg, total_time, sigma_size, n, m" << std::endl;
  struct timespec time_start_n, time_end_n;
  std::cout << "|Σ| vs n vs m {" << std::endl;
  int size = min_size;
  while(size <= max_size){

    int words = pow(2,size);
    int words_to_search = words*0.1;
    int sigma_size = min_sigma_size;
    std::cout << "  n = " << words << " (i=" << size << "){" << std::endl;
    if (verbose != 1){
      std::cout << "    |Σ| = ";
    }
    clock_gettime(CLOCK_MONOTONIC, &time_start_n);
    while(sigma_size <= max_sigma_size){
      if (sigma_size != 2 && sigma_size != 10 && sigma_size != 20 && sigma_size != 200){
        sigma_size++;
        continue;
      }
      if (verbose == 1){
      std::cout << "    |Σ| = " << sigma_size << ", ";
    }else{
      std::cout << sigma_size << " " << std::flush;
    }
      struct timespec time_start, time_end;
      double *build_time_SA_avg_avg = new double();
      double *search_time_by_word_SA_avg_avg = new double[words_to_search]();
      double *search_time_by_word_size_SA_avg_avg = new double[max_word_size]();

      double *build_time_by_word_DFA_avg_avg = new double[words_to_search]();
      double *build_time_by_word_size_DFA_avg_avg = new double[max_word_size]();
      double *search_time_by_word_DFA_avg_avg = new double[words_to_search]();
      double *search_time_by_word_size_DFA_avg_avg = new double[max_word_size]();
      clock_gettime(CLOCK_MONOTONIC, &time_start);
      int j = 0;
      if (verbose == 1){
        std::cout << "it: ";
      }
      while(j < outer_iterations){
        if (verbose == 1){
          std::cout << j << ", " << std::flush;
        }

        double *build_time_SA_avg = new double();
        double *search_time_by_word_SA_avg = new double[words_to_search]();
        double *search_time_by_word_size_SA_avg = new double[max_word_size]();

        double *build_time_by_word_DFA_avg = new double[words_to_search]();
        double *build_time_by_word_size_DFA_avg = new double[max_word_size]();
        double *search_time_by_word_DFA_avg = new double[words_to_search]();
        double *search_time_by_word_size_DFA_avg = new double[max_word_size]();

        test_alphabet_size(words, words_to_search, sigma_size, size, inner_iterations, max_word_size, build_time_SA_avg, search_time_by_word_SA_avg, search_time_by_word_size_SA_avg, build_time_by_word_DFA_avg, build_time_by_word_size_DFA_avg, search_time_by_word_DFA_avg, search_time_by_word_size_DFA_avg);
        copy_add_arrays(build_time_SA_avg_avg, build_time_SA_avg, 1, (double) outer_iterations);
        copy_add_arrays(search_time_by_word_SA_avg_avg, search_time_by_word_SA_avg, words_to_search, (double) outer_iterations);
        copy_add_arrays(search_time_by_word_size_SA_avg_avg, search_time_by_word_size_SA_avg, max_word_size, (double) outer_iterations);
        copy_add_arrays(build_time_by_word_DFA_avg_avg, build_time_by_word_DFA_avg, words_to_search, (double) outer_iterations);
        copy_add_arrays(build_time_by_word_size_DFA_avg_avg, build_time_by_word_size_DFA_avg, max_word_size, (double) outer_iterations);
        copy_add_arrays(search_time_by_word_DFA_avg_avg, search_time_by_word_DFA_avg, words_to_search, (double) outer_iterations);
        copy_add_arrays(search_time_by_word_size_DFA_avg_avg, search_time_by_word_size_DFA_avg, max_word_size, (double) outer_iterations);

        delete build_time_SA_avg;
        delete[] search_time_by_word_SA_avg;
        delete[] search_time_by_word_size_SA_avg;
        delete[] build_time_by_word_DFA_avg;
        delete[] build_time_by_word_size_DFA_avg;
        delete[] search_time_by_word_DFA_avg;
        delete[] search_time_by_word_size_DFA_avg;
        j++;
      }
      clock_gettime(CLOCK_MONOTONIC, &time_end);
      double total_time_out_iterations = operation_time_in_seconds(operation_time_in_nanos(time_start,time_end));
      print_to_files(words_to_search, max_word_size, sigma_size, size, out_sigma_size, out_SA_build, out_SA_search, out_SA_search_m, out_DFA_build, out_DFA_search, out_DFA_build_m, out_DFA_search_m, out_sigma_size_m , build_time_SA_avg_avg, search_time_by_word_SA_avg_avg, search_time_by_word_size_SA_avg_avg, build_time_by_word_DFA_avg_avg, build_time_by_word_size_DFA_avg_avg, search_time_by_word_DFA_avg_avg, search_time_by_word_size_DFA_avg_avg);
      std::cout << "time: " << total_time_out_iterations << std::endl;
      sigma_size++;
      delete build_time_SA_avg_avg;
      delete[] search_time_by_word_SA_avg_avg;
      delete[] search_time_by_word_size_SA_avg_avg;
      delete[] build_time_by_word_DFA_avg_avg;
      delete[] build_time_by_word_size_DFA_avg_avg;
      delete[] search_time_by_word_DFA_avg_avg;
      delete[] search_time_by_word_size_DFA_avg_avg;
    }
    clock_gettime(CLOCK_MONOTONIC, &time_end_n);
    double total_time_n = operation_time_in_seconds(operation_time_in_nanos(time_start_n,time_end_n));
    std::cout << "  } total time: " << total_time_n << std::endl;
    size++;
  }
  out_sigma_size.close();
  out_SA_build.close();
  out_SA_search.close();
  out_SA_search_m.close();
  out_DFA_build.close();
  out_DFA_search.close();
  out_DFA_build_m.close();
  out_DFA_search_m.close();
  out_sigma_size_m.close();
}

int main(){

  if (1 == 0){
    std::string txt = "banana$$$";
    std::string t = "$";
    std::cout << t.compare("$") << std::endl;
    int *num_txt = new int[txt.length()];
    int *SA = new int[txt.length()-3];
    string_to_numbers(txt, num_txt);
    suffix_array(num_txt, SA, txt.length()-3,36);
    print_array(SA, txt.length()-3);
    delete[] num_txt;
    delete[] SA;
    return 0;
  }

  if (1 == 0){
    //filename, min_size, max_size, min_sigma_size, max_sigma_size, max_word_size, inner_iterations, outer_iterations
    sigma_vs_size_vs_patternsize_random("./random_word_length/experiment", 9, 25, 2, 200, 40000, 3, 1);
    return 0;
  }

  int test = 6;

  if (test == 0){
  }else if(test == 1){
    simple_test_automata();
  }else if(test == 2){
    ned_test_automata();
  }else if(test == 3){
    int i = 1;
    std::string prefix = "./textos/";
    std::string full_series = "";
    while(i < 6){
      full_series = full_series + parse_file_txt(prefix + "0" +std::to_string(i) +".txt");
      // save_string_to_file(clean_string(parse_file_txt(prefix + "0" +std::to_string(i) +".txt")),prefix +"0" +std::to_string(i) + "_clean.txt");
      i++;
    }
    save_string_to_file(clean_string(full_series), prefix+"full_series_clean.txt");
  }else if(test == 4){
    make_file(1,"prueba",36);
    std::vector<int> integers = parse_file_int("prueba");
    std::cout << "vector size: " << integers.size() << std::endl;
    print_vector(integers);
    print_array(integers.data(),integers.size());
  }else if(test == 5){
    ned_test_suffix();
  }else if(test == 6){
    simple_test_suffix();
  }

  return 0;
}
