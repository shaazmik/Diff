#ifndef RECURSIVE_H
#define RECURSIVE_H

#include "Diff.h"


const int Math_function_len = 4;

//================================

size_t find_file_size(FILE *in);

char* fill_text(FILE* in, size_t file_size);

char* get_math_word(char* s);

struct Pnode* input_operator(struct Pnode* node, char oper);

struct Pnode* getG(struct Pnode* node);

struct Pnode* getE(struct Pnode* node);

struct Pnode* getT(struct Pnode* node);

struct Pnode* getPw(struct Pnode* node);

struct Pnode* getP(struct Pnode* node);

struct Pnode* get_math_F(struct Pnode* node);

struct Pnode* getV(struct Pnode* node);

struct Pnode* getN(struct Pnode* node);

//================================



#endif