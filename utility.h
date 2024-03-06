#include<stdio.h>
#include<string.h>
#include<string>
#include<vector>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <random>
#include <cctype>
#include <algorithm>
#include <functional>

#define BILLION 1000000000L

int letter_number(std::string letter);
void print_array(int *arr, int n);
double operation_time_in_seconds(uint64_t nanos);
uint64_t operation_time_in_nanos(struct timespec start, struct timespec end);
void string_to_numbers(std::string source, int *dest);
std::string parse_file_txt(std::string source);
std::vector<int> parse_file_int(std::string sourcefile);
void make_file(int megabytes, std::string filename, int sigma_size);
void print_vector(std::vector<int> vec);
std::string clean_string(std::string source);
void save_string_to_file(std::string source, std::string filename);
int multiple(int words, int goal);
void make_random_text(int chars, int sigma_size, int *out);
double sum_array(double *array, int size);
