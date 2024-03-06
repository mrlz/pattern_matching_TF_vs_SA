#include "utility.h"

uint64_t operation_time_in_nanos(struct timespec start, struct timespec end){
  uint64_t diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  return diff;
}

double operation_time_in_seconds(uint64_t nanos){
  double d = (double)nanos/1000000000.0;
  return d;
}

void print_array(int *arr, int n){
  int i = 0;
  std::cout << "[";
  while(i < n){
    std::cout << arr[i] << ", ";
    i++;
  }
  std::cout << "]" << std::endl;
}

void print_vector(std::vector<int> vec){
  int i = 0;
  while(i < vec.size()){
    std::cout << vec[i] << " ";
    i++;
  }
  std::cout << std::endl;
}

double sum_array(double *array, int size){
  int i = 0;
  double sum = 0.0;
  while(i < size){
    sum = sum + array[i];
    i++;
  }
  return sum;
}

int letter_number(std::string letter){
  if (letter.compare("$") == 0){
    return 0;
  }
  std::vector<char> writable(letter.begin(),letter.end());
  writable.push_back('\0');
  int ret = (int)writable[0]-96;
  if (ret > 0){
    return ret;
  }
  return ret+75;
}

void string_to_numbers(std::string source, int *dest){
  int i = 0;
  while(i < source.length()){
    dest[i] = letter_number(source.substr(i,1));
    i++;
  }
}

std::string parse_file_txt(std::string source){
std::ifstream source_file(source);
std::string str;

source_file.seekg(0,std::ios::end);
str.reserve(source_file.tellg());
source_file.seekg(0,std::ios::beg);

str.assign((std::istreambuf_iterator<char>(source_file)),std::istreambuf_iterator<char>());
return str;
}

std::vector<int> parse_file_int(std::string sourcefile){
  std::ifstream InFile(sourcefile, std::ios::binary);
  std::vector<int>numbers;

  InFile.seekg (0, InFile.end);
  int length = InFile.tellg();
  InFile.seekg (0, InFile.beg);

  int number;
  int count = 0;

  while(length > 0){
    InFile.read((char*)&number,sizeof(number));
    numbers.push_back(number);
    length = length - sizeof(number);
    count++;
  }

  InFile.close();
  return numbers;
}

void make_file(int megabytes, std::string filename, int sigma_size){

  std::random_device rand_dev; //these will produce different sequences everytime
  std::mt19937 generator(rand_dev());
  const int start = 1;
  const int end = sigma_size;
  std::uniform_int_distribution<int> uniform_distribution(start,end);

  int one_mb_items = 1024*1024/sizeof(int);
  int total = one_mb_items*megabytes;
  int i = 0;
  int number = 0;
  std::ofstream OutFile(filename, std::ios::out | std::ios::binary);
  while(i < total){
    number = uniform_distribution(generator);
    OutFile.write((char*)&number,sizeof(number));
    i++;
  }
  OutFile.close();
}

void make_random_text(int chars, int sigma_size, int *out){
  std::random_device rand_dev; //these will produce different sequences everytime
  std::mt19937 generator(rand_dev());
  const int start = 1;
  const int end = sigma_size;
  std::uniform_int_distribution<int> uniform_distribution(start,end);

  int i = 0;
  while(i < chars){
    out[i] = uniform_distribution(generator);
    i++;
  }
}

void save_string_to_file(std::string source, std::string filename){
  std::ofstream file(filename);
  file << source;
  file.close();
}

std::string clean_string(std::string source){
  std::string contents = source;
  std::transform(contents.begin(), contents.end(), contents.begin(), [] (unsigned char c) { return std::tolower(c); });
  contents.erase(std::remove_if(contents.begin(), contents.end(), std::not1(std::ptr_fun( (int(*)(int))std::isalnum )) ), contents.end());
  return contents;
}

int multiple(int words, int goal){
  int j = 1;
  while(words*j < goal){
    j++;
  }
  return j;
}
