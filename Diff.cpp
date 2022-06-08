#define $ printf(">>> %d:\n", __LINE__);


#include ".\Diff.h"
#include ".\recursive_descent.h"

char* input_buffer; 


int main()
{
    FILE* in = fopen("open.txt", "rb");

    if (in == nullptr)
    {
        printf("File not found.\n");
        return -1;
    }

    size_t file_size = find_file_size(in);
    input_buffer = fill_text(in, file_size);

    printf("\n%s\n", input_buffer);

    struct Ptree tree = {};
    ptree_construct(&tree);

    struct Ptree old_tree = {};
    ptree_construct(&old_tree);

    tree.start_node = getLast(tree.start_node);

    copy_pnode(&(old_tree.start_node), tree.start_node);

    diff_cases(tree.start_node);
    
    yes_i_am_simp(tree.start_node);

    graph(tree.start_node);
    tex_dump(old_tree.start_node, tree.start_node);

    return 0;
}