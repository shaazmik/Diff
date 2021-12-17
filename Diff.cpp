#define $ printf(">>> %d:\n", __LINE__);


#include ".\Diff.h"
#include ".\recursive_descent.h"

char* s; 
struct Pnode* dump = nullptr;

int main()
{
    FILE* in = fopen("open.txt", "rb");

    if (in == nullptr)
    {
        printf("File not found.\n");
        return -1;
    }

    size_t file_size = find_file_size(in);

    s = (char*)calloc(file_size + 1, sizeof(char));

    s = fill_text(in, file_size);

    printf("\n%s\n", s);

    struct Ptree tree = {};

    ptree_construct(&tree);

    tree.start_node = getG(tree.start_node);

    diff_cases(tree.start_node);
    
    dump = tree.start_node;

    yes_i_am_simp(tree.start_node);

    graph(tree.start_node);

    return 0;
}