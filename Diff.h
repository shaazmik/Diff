#ifndef DIFF_H
#define DIFF_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>




struct Pnode
{
    Pnode* left   = nullptr;

    Pnode* right  = nullptr;

    size_t type   = 0;

    int    value  = 0;
};


struct Ptree
{
    Pnode* start_node = nullptr;

    size_t tree_size = 0; 
};



enum types
{
    NUMBER        = 0XDDD0,
    VARIABLE      = 0xDDD1,
    OPERATOR      = 0XDDD2,
    FUNCTION_NAME = 0XDDD3,
    FUNCTION_MATH = 0xDDD4,
};




int ptree_construct(struct Ptree* tree);


void pnode_destructor(struct Pnode **node);


struct Pnode** pnode_constructor(struct Pnode** node,int value);


void diff_cases(struct Pnode* node);


struct Pnode* diff_multiply(struct Pnode* node);


struct Pnode* diff_divide(struct Pnode* node);


struct Pnode* diff_power(struct Pnode* node);


struct Pnode* diff_power_e(struct Pnode* node);


struct Pnode* diff_ln(struct Pnode* node);


struct Pnode* diff_cos(struct Pnode* node);


struct Pnode* diff_sin(struct Pnode* node);


struct Pnode* copy_part_tree(struct Pnode* node);


void copy_pnode(struct Pnode** dst, struct Pnode* src);


void simplify(Pnode** node); 


void graph(Pnode* head);


static void in_order_graph(Pnode *node, FILE* out);


struct Pnode* get_math_F(struct Pnode* node);


#endif
