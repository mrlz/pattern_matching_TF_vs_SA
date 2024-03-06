### What is this repository for? ###
C++ implementation of pattern matching using O(n) construction suffix array and O(|∑|m) construction deterministic finite automata. Search is O(mlog(n)) for suffix array (double binary search of first and last appearance) and O(n) for the automata (must check the whole text).

### How do I get set up? ###
Compile with:
g++ -std=c++11 experiments.cpp pattern_matching.cpp utility.cpp