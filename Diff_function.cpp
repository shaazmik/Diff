#define $ printf(">>> %d:\n", __LINE__);


#include ".\Diff.h"
#include ".\recursive_descent.h"


int ptree_construct(struct Ptree* tree)
{
    assert(tree != nullptr);

    tree->start_node = nullptr;

    tree->tree_size = 0;

    return 1;
}

void add_pnode(struct Pnode** node)
{
    *node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    (*node)->right = nullptr;
    (*node)->left  = nullptr;
    (*node)->value = Poison;
    (*node)->type  = Poison;

    return;
}


void pnode_destructor(struct Pnode **node) 
{

    if (*node == nullptr)
        return;
    
    (*node)->right = nullptr;
    (*node)->left  = nullptr;

    (*node)->value  = 0;
    (*node)->type  = 0;
    
    free (*node);

    *node = nullptr;

    return;
}


void diff_cases(struct Pnode* node)
{
    if (node->type == OPERATOR)
    {
        switch (node->value)
        {
        case '+':
            node->value = '+';
            diff_cases(node->left);
            diff_cases(node->right);
            break;

        case '-':
            node->value = '-';
            diff_cases(node->left);
            diff_cases(node->right);
            break;

        case '*':
            node = diff_multiply(node);
            diff_cases(node->left->left);
            diff_cases(node->right->right);
            break;
        
        case '/':
            node = diff_divide(node);
            diff_cases(node->left->left->left);
            diff_cases(node->left->right->right);
            break;

        case '^':
            if (node->left->value != 'e')
            {
                node = diff_power(node);
                diff_cases(node->left->right);
            }
            else
            {
                node = diff_power_e(node);
                diff_cases(node->right);
            }
            break;

        default:
            printf("Something went wrong in case\n\n");
            break;
        }
    }
    else if (node->type == VARIABLE)
    {
        node->value = 1;
        node->type  = NUMBER;
        return;
    }
    else if (node->type == NUMBER)
    {
        node->value = 0;
        return;
    }
    else if(node->type == FUNCTION_NAME)
    {
        switch (node->left->value)
        {
        case 'l':
            break;
        
        case 'c':
            node = diff_cos(node);
            diff_cases(node->right);
            break;
        
        case 's':
            node = diff_sin(node);
            diff_cases(node->right);
            break;

        default:
            break;
        }
    }
}


struct Pnode* diff_multiply(struct Pnode* node)
{
    node->value ='+';

    int u = node->left->value;
    int v = node->right->value;

    size_t u_type = node->left->type;
    size_t v_type = node->right->type;

//==========================================================================


    struct Pnode* new_node_left_left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    struct Pnode* new_node_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left->type  = u_type;
    new_node_left_left->value = u;
    new_node_left_left->left  = copy_part_tree(node->left->left);  
    new_node_left_left->right = copy_part_tree(node->left->right);


    new_node_left_right->type  = v_type;
    new_node_left_right->value = v;
    new_node_left_right->left  = node->right->left;
    new_node_left_right->right = node->right->right;


//==========================================================================


    struct Pnode* new_node_right_left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    struct Pnode* new_node_right_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));


    new_node_right_left->type  = u_type;
    new_node_right_left->value = u;
    new_node_right_left->left  = node->left->left;
    new_node_right_left->right = node->left->right;

    new_node_right_right->type  = v_type;
    new_node_right_right->value = v;
    new_node_right_right->left  = copy_part_tree(node->right->left);
    new_node_right_right->right = copy_part_tree(node->right->right);

//==========================================================================


    node->left->type  = OPERATOR;
    node->left->value = '*';
    node->left->left  = new_node_left_left;
    node->left->right = new_node_left_right;


    node->right->type  = OPERATOR;
    node->right->value = '*';
    node->right->left  = new_node_right_left;
    node->right->right = new_node_right_right;

//==========================================================================

    return node;
}


struct Pnode* diff_divide(struct Pnode* node)
{
    node->value = '/';

    int u = node->left->value;
    int v = node->right->value;

    size_t u_type = node->left->type;
    size_t v_type = node->right->type;

//==========================================================================

    node->left->type  = OPERATOR;
    node->left->value = '-';
    
    struct Pnode* new_node_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left->type  = OPERATOR;
    new_node_left_left->value = '*';
    
    struct Pnode* new_node_left_right = (struct Pnode*)calloc(1,sizeof(struct Pnode));

    new_node_left_right->type  = OPERATOR;
    new_node_left_right->value = '*';

    struct Pnode* new_node_left_left_left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    struct Pnode* new_node_left_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_left->type  = u_type;
    new_node_left_left_left->value = u;
    new_node_left_left_left->left  = copy_part_tree(node->left->left);
    new_node_left_left_left->right = copy_part_tree(node->left->right);
    
    new_node_left_left->left = new_node_left_left_left;

    new_node_left_left_right->type  = v_type;
    new_node_left_left_right->value = v;
    new_node_left_left_right->left  = node->right->left;
    new_node_left_left_right->right = node->right->right;

    new_node_left_left->right = new_node_left_left_right;

    struct Pnode* new_node_left_right_left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    struct Pnode* new_node_left_right_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_right_left->type  = u_type;
    new_node_left_right_left->value = u;
    new_node_left_right_left->left  = node->left->left;
    new_node_left_right_left->right = node->left->right;

    new_node_left_right->left = new_node_left_right_left;

    new_node_left_right_right->type  = v_type;
    new_node_left_right_right->value = v;
    new_node_left_right_right->left  = copy_part_tree(node->right->left);
    new_node_left_right_right->right = copy_part_tree(node->right->right);

    new_node_left_right->right = new_node_left_right_right;

    
//==========================================================================

    node->right->type  = OPERATOR;
    node->right->value = '^';

    struct Pnode* new_node_right_left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    struct Pnode* new_node_right_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_right_left->type  = v_type;
    new_node_right_left->value = v;
    new_node_right_left->left  = copy_part_tree(node->right->left);
    new_node_right_left->right = copy_part_tree(node->right->right);

    new_node_right_right->type  = NUMBER;
    new_node_right_right->value = 2;
    new_node_right_right->left  = nullptr;
    new_node_right_right->right = nullptr;

//==========================================================================

    node->left->left   = new_node_left_left;
    node->left->right  = new_node_left_right;
    node->right->right = new_node_right_right;
    node->right->left  = new_node_right_left;

    return node;

}

struct Pnode* diff_power(struct Pnode* node)
{
    node->value = '*';

    int u = node->left->value;
    int n = node->right->value;
    
    size_t u_type = node->left->type;
    
//==========================================================================


    struct Pnode* new_node_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    
    new_node_left_right->type  = u_type;
    new_node_left_right->value = u;
    new_node_left_right->left  = copy_part_tree(node->left->left);
    new_node_left_right->right = copy_part_tree(node->left->right);

//==========================================================================


    struct Pnode* new_node_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left->type  = OPERATOR;
    new_node_left_left->value = '^';
    
    struct Pnode* new_node_left_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_left->value = u;
    new_node_left_left_left->type  = u_type;
    new_node_left_left_left->left  = node->left->left;
    new_node_left_left_left->right = node->left->right;

    new_node_left_left->left = new_node_left_left_left;

//==========================================================================


    struct Pnode* new_node_left_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_right->value = '-';
    new_node_left_left_right->type  = OPERATOR;
    
    new_node_left_left->right = new_node_left_left_right;

    struct Pnode* new_node_left_left_right_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_right_left->value = n;
    new_node_left_left_right_left->type  = NUMBER;
    new_node_left_left_right_left->left  = nullptr;
    new_node_left_left_right_left->right = nullptr;

    new_node_left_left_right->left = new_node_left_left_right_left;

    struct Pnode* new_node_left_left_right_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_right_right->value = 1;
    new_node_left_left_right_right->type  = NUMBER;
    new_node_left_left_right_right->left  = nullptr;
    new_node_left_left_right_right->right = nullptr;

    new_node_left_left_right->right = new_node_left_left_right_right;


//==========================================================================


    node->left->type  = OPERATOR;
    node->left->value = '*';
    node->left->left   = new_node_left_left;
    node->left->right =  new_node_left_right;
    
    node->right->type  = NUMBER;
    node->right->value = n;
    node->right->left  = nullptr;
    node->right->right = nullptr;

    return node; 
}


struct Pnode* diff_power_e(struct Pnode* node)
{
    struct Pnode* copy_tree = copy_part_tree(node);

    node->value = '*';
    node->type  = OPERATOR;

    node->left = copy_tree;

    return node;
}

// struct Pnode* diff_ln(struct Pnode* node)
// {

// }

struct Pnode* diff_cos(struct Pnode* node)
{
    node->type  = OPERATOR;
    node->value = '*'; 

    size_t u_type = node->right->type;
    int u         = node->right->value;

//==========================================================================


    struct Pnode* new_node_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));
    
    new_node_left_right->type  = NUMBER;
    new_node_left_right->value = -1;
    new_node_left_right->left  = nullptr;
    new_node_left_right->right = nullptr;

//==========================================================================


    struct Pnode* new_node_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left->type  = FUNCTION_NAME;
    new_node_left_left->value = 'F';
    
//==========================================================================

    struct Pnode* new_node_left_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_left->type  = FUNCTION_MATH;
    new_node_left_left_left->value = 's';
    new_node_left_left_left->left  = nullptr;
    new_node_left_left_left->right = nullptr;

//==========================================================================

    struct Pnode* new_node_left_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left_right->type  = u_type;
    new_node_left_left_right->value = u;
    new_node_left_left_right->left  = node->right->left;
    new_node_left_left_right->right = node->right->right; 



    new_node_left_left->left  = new_node_left_left_left;
    new_node_left_left->right = new_node_left_left_right;

//==========================================================================


    struct Pnode* left_copy  = copy_part_tree(node->right->left);
    struct Pnode* right_copy = copy_part_tree(node->right->right);

    node->right->left  = left_copy;
    node->right->right = right_copy;

    node->left->left  = new_node_left_left;
    node->left->right = new_node_left_right;
    node->left->value = '*';
    node->left->type  = OPERATOR;

//==========================================================================

    return node;
}


struct Pnode* diff_sin(struct Pnode* node)
{
    node->type  = OPERATOR;
    node->value = '*'; 

    size_t u_type = node->right->type;
    int u =         node->right->value;

    struct Pnode* new_node_left_left = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_left->type  = FUNCTION_MATH;
    new_node_left_left->value = 'c';
    new_node_left_left->left  = nullptr;
    new_node_left_left->right = nullptr;

    struct Pnode* new_node_left_right = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node_left_right->type  = u_type;
    new_node_left_right->value = u;
    new_node_left_right->left  = node->right->left;
    new_node_left_right->right = node->right->right;

    struct Pnode* left_copy  = copy_part_tree(node->right->left);
    struct Pnode* right_copy = copy_part_tree(node->right->right);

    node->right->left  = left_copy;
    node->right->right = right_copy;
    node->right->type  = u_type;
    node->right->value = u;

    node->left->value  = 'F';
    node->left->type   = FUNCTION_NAME;
    node->left->left   = new_node_left_left;
    node->left->right  = new_node_left_right;

    return node;
}

struct Pnode* copy_part_tree(struct Pnode* node)
{
    if(node != nullptr)
    {
        
        struct Pnode* copied_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        copied_node->value = node->value;
        copied_node->type  = node->type;
        copied_node->left  = copy_part_tree(node->left);
        copied_node->right = copy_part_tree(node->right);
        return copied_node;
    }
    else return nullptr;

}


void copy_pnode(struct Pnode** dst, struct Pnode* src)
{
    assert(src != nullptr);

    if (*dst == nullptr)
    {
        add_pnode(dst);
    }

    (*dst)->type  = src->type;
    (*dst)->value = src->value;
    
    if (src->left != nullptr)
    {
        copy_pnode(&(*dst)->left, src->left);
    }
    
    if (src->right != nullptr)
    {
        copy_pnode(&(*dst)->right, src->right);
    }

    return;
}

void ptree_destructor(Pnode** topNode) 
{
    assert (topNode);

   // free (topNode->data);

    if (&(*topNode)->left == nullptr) 
    {
        return;
    }

    ptree_destructor(&(*topNode)->left);

    free ((*topNode)->left);

    if (&(*topNode)->right != nullptr) 
    {
        ptree_destructor(&(*topNode)->right);

        free ((*topNode)->right);
    }
}


void simplify_node(Pnode** node) 
{

    assert(node != nullptr);

    if ((*node)->type == OPERATOR) 
    {
        switch ((*node)->value) 
        {

            case '*': 

                if ((*node)->right->value == 1) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode (&tmp, (*node)->left);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
                else if ((*node)->left->value == 1) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode (&tmp, (*node)->right);
                    
                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
                else if ((*node)->right->type == NUMBER && (*node)->left->type == NUMBER) 
                {
                    int val = (*node)->left->value * (*node)->right->value;

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;
                    (*node)->value = val;
                    (*node)->type  = NUMBER;
                }
                else if ((*node)->right->value == 0 || (*node)->left->value == 0) 
                {
                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    (*node)->type = NUMBER;
                    (*node)->value = 0;
                }

            break;

            case '+':
            {   if ((*node)->right->type == NUMBER && (*node)->left->type == NUMBER) 
                {
                    (*node)->type = NUMBER;
                    (*node)->value = (*node)->left->value + (*node)->right->value;

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    //copy_pnode(node, tmp);
                }
                else if ((*node)->right->value == 0) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode(&tmp, (*node)->left);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
                else if ((*node)->left->value == 0) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode(&tmp, (*node)->right);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }

            break;
            }

            case '-':
                if ((*node)->right->type == NUMBER && (*node)->left->type == NUMBER) 
                {
                    (*node)->type = NUMBER;
                    (*node)->value = (*node)->left->value - (*node)->right->value;

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;
                }
                else if ((*node)->right->value == 0) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode(&tmp, (*node)->left);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
                else if ((*node)->left->value == 0) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode(&tmp, (*node)->right);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
            break;
            

            case '^':
            {

                if ((*node)->right->value == 1) 
                {
                    struct Pnode* tmp = (struct Pnode *)calloc (1, sizeof (struct Pnode));
                    copy_pnode(&tmp, (*node)->left);

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;

                    copy_pnode(node, tmp);
                }
                else if ((*node)->right->value == 0) 
                {
                    (*node)->type = NUMBER;
                    (*node)->value = 1;

                    ptree_destructor(&(*node)->left);
                    ptree_destructor(&(*node)->right);

                    (*node)->left = nullptr;
                    (*node)->right = nullptr;
                }
            
            break;
            }
            default:
                printf("yes you are simpy dimpy\n");
            break;
        }
    }
    return;
}

void yes_i_am_simp(struct Pnode* start_node)
{
    assert(start_node != nullptr);

    if (start_node->left != nullptr)
    {
        yes_i_am_simp(start_node->left);
    }

    if (start_node->right != nullptr)
    {
        yes_i_am_simp(start_node->right);
    }

    //graph(dump);

    simplify_node(&start_node);

    //graph(dump);

    return;
}

void tex_dump(struct Pnode* start_node, struct Pnode* diff_node) 
{
    assert(start_node);
    assert(diff_node);

    FILE *tex = fopen ("Result.tex", "w");

    fprintf (tex,   "\\documentclass{article}\n"
                    "\\usepackage[russian]{babel}\n"
                    "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n"
                    "\\usepackage{amsmath}\n"
                    "\\usepackage{graphicx}\n"
                    "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n"
                    "\\title{Жопа с ручкой}\n"
                    "\\author{Парамонов Парамон}\n"
                    "\\begin{document}\n"
                    "\\maketitle\n"
                    "\\begin{abstract}\n"
                    "Вдохновлялся преподавателем с практическим опытом...\n"
                    "\\end{abstract}\n"
                    "\\section{Интегрирование:)}\n");

    fprintf (tex, "\\[f(x) = ");
    tex_dump_node(start_node, tex);
    fprintf (tex, "\\]\n");

    fprintf (tex, "\\[(f(x))' = ");
    tex_dump_node(diff_node, tex);
    fprintf (tex, "\\]\n");

    fprintf (tex, "\\end{document}\n");

    fclose (tex);
}

void tex_dump_node(Pnode* node, FILE *tex) 
{
    assert (node);
    assert (tex);

    switch (node->type) 
    {
        case NUMBER: 
        {
            fprintf (tex, "{%d}", node->value);

            break;
        }
        case OPERATOR: 
        {
            switch (node->value) 
            {
                case '*': 
                {
                    if (node->left->type == OPERATOR && ((node->left->value == '-') || (node->left->value == '+'))) 
                    {
                        fprintf (tex, "(");

                        tex_dump_node(node->left, tex);

                        fprintf (tex, ")");
                    }
                    else 
                    {
                        tex_dump_node(node->left, tex);
                    }

                    fprintf (tex, " \\cdot ");

                    if (node->right->type == OPERATOR &&  ((node->right->value == '-') || (node->right->value == '+'))) 
                    {
                        fprintf (tex, "(");

                        tex_dump_node(node->right, tex);

                        fprintf (tex, ")");
                    }
                    else 
                    {
                       tex_dump_node(node->right, tex);
                    }

                    break;
                }
                case '/': 
                {
                    fprintf (tex, "\\frac{");

                    if (node->left->type == OPERATOR && ((node->left->value == '-') || (node->left->value == '+'))) 
                    {
                            fprintf (tex, "(");

                            tex_dump_node(node->left, tex);

                            fprintf (tex, ")");
                    }
                    else 
                    {
                         tex_dump_node(node->left, tex);
                    }

                    fprintf (tex, "}{");

                    if (node->right->type == OPERATOR && ((node->right->value == '-') || (node->right->value == '+'))) 
                    {
                        fprintf (tex, "(");

                        tex_dump_node(node->right, tex);

                        fprintf (tex, ")");
                    }
                    else 
                    {
                        tex_dump_node(node->right, tex);
                    }

                    fprintf (tex, "}");

                    break;
                }
                case '^': 
                {
                    if (node->left->type != NUMBER && node->left->type != VARIABLE) 
                    {
                        fprintf (tex, "({");

                        tex_dump_node(node->left, tex);

                        fprintf (tex, "})");
                    }
                    else 
                    {
                        tex_dump_node(node->left, tex);
                    }

                    fprintf (tex, "%s", "^");

                    fprintf (tex, "{");

                    tex_dump_node(node->right, tex);

                    fprintf (tex, "}");

                    break;
                }
                case '+': 
                {
                    tex_dump_node(node->left, tex);

                    fprintf (tex, "%c", node->value);

                    tex_dump_node(node->right, tex);

                    break;
                }
            }

            break;
        }
        case VARIABLE: 
        {
            fprintf (tex, "{%c}", node->value);

            break;
        }
        case FUNCTION_NAME: 
        {
            if (node->left->value == 'c')
            {
                fprintf(tex, "\\cos", node->left->value);
            }
            else if (node->left->value == 's')
            {
                fprintf(tex, "\\sin", node->left->value);

            }
            else
            {
                fprintf(tex, "\\ln", node->left->value);
            }

            if (node->right->type != NUMBER && node->right->type != VARIABLE) 
            {
                fprintf (tex, "{");

                tex_dump_node(node->right, tex);

                fprintf (tex, "}");
            }
            else 
            {
                tex_dump_node(node->right, tex);
            }

            break;
        }
    }

    return;
}

void graph(struct Pnode* start_node) 
{
    static int dump_number = 1;

    char cmd[100] = "0";

    assert(start_node != nullptr);

    FILE *out = fopen ("demo.txt", "wb");

    assert (out != nullptr);

    fputs ("digraph structs {\n", out);

    in_order_graph (start_node, out);

    fputs ("}", out);

    fclose (out);

    snprintf(cmd, sizeof(cmd), "dot -Tpng demo.txt -o graph_%d.png", dump_number);
    system (cmd);
   
    // memset(cmd, 0, sizeof(cmd));
    // snprintf(cmd, sizeof(cmd), "graph_%d.png", dump_number);
    // system (cmd);

    dump_number++;

    return;
}


static void in_order_graph(Pnode *node, FILE* out) 
{
    if (node->type == NUMBER) 
    {
        fprintf (out, "%d [shape=oval];\n", node);
        fprintf (out, "%d [style=filled,color=\"red\"];\n", node);
        fprintf (out, "%d [label=\"%d\"]\n", node, node->value);
    }
    else if (node->type == OPERATOR) 
    {
        fprintf (out, "%d [shape=record];\n", node);
        fprintf (out, "%d [style=filled,color=\"blue\"];\n", node);
        fprintf (out, "%d [label=\"%c\"]\n", node, node->value);
    }
    else if (node->type == VARIABLE) 
    {
        fprintf (out, "%d [shape=diamond];\n", node);
        fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node);
        fprintf (out, "%d [label=\"%c\"]\n", node, node->value);
    }
    else if (node->type == FUNCTION_NAME) 
    {
        fprintf (out, "%d [shape=star];\n", node);
        fprintf (out, "%d [style=filled,color=\"Sienna\"];\n", node);
        fprintf (out, "%d [label=\"%c\"]\n", node, node->value);
    }
    else if (node->type == FUNCTION_MATH)
    {
        fprintf (out, "%d [shape=star];\n", node);
        fprintf (out, "%d [style=filled,color=\"cyan4\"];\n", node);
        fprintf (out, "%d [label=\"%c\"]\n", node, node->value);
    }
    else 
    {
        fprintf (out, "%d [shape=oval];\n", node);
        fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node);
        fprintf (out, "%d [label=\"%c\"]\n", node, node->value); 
    }

    if (node->left != nullptr) 
    {
        fprintf (out, "\"%d\" -> \"%d\";\n", node, node->left);

        if (node->left->type == NUMBER) 
        {
            fprintf (out, "%d [shape=oval];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node->left);
            fprintf (out, "%d [label=\"%d\"]\n", node->left, node->left->value);
        }
        else if (node->left->type == OPERATOR) 
        {
            fprintf (out, "%d [shape=record];\n", node->left);
            fprintf (out, "%d [style=diagonals,color=\"green\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->value);
        }
        else if (node->left->type == VARIABLE) 
        {
            fprintf (out, "%d [shape=diamond];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->value);
        }
        else if (node->left->type == FUNCTION_NAME) 
        {
            fprintf (out, "%d [shape=star];\n", node->left);
            fprintf (out, "%d [style=dashed,color=\"Sienna\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->value);
        }
        else if(node->left->type == FUNCTION_MATH)
        {
            fprintf (out, "%d [shape=star];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"cyan4\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->value);
        }
        else 
        {
            fprintf (out, "%d [shape=oval];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->value); 
        }
        fprintf (out, "\n");
        in_order_graph (node->left, out);
    }

    if (node->right != nullptr) 
    {
        fprintf (out, "\"%d\" -> \"%d\";\n", node, node->right);

        if (node->right->type == NUMBER) 
        {
            fprintf (out, "%d [shape=oval];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node->right);
            fprintf (out, "%d [label=\"%d\"]\n", node->right, node->right->value);
        }
        else if (node->right->type == OPERATOR) 
        {
            fprintf (out, "%d [shape=record];\n", node->right);
            fprintf (out, "%d [style=diagonals,color=\"green\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->value);
        }
        else if (node->right->type == VARIABLE) 
        {
            fprintf (out, "%d [shape=diamond];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->value);
        }
        else if (node->right->type == FUNCTION_NAME) 
        {
            fprintf (out, "%d [shape=star];\n", node->right);
            fprintf (out, "%d [style=dashed,color=\"Sienna\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->value);
        }
        else if (node->right->type == FUNCTION_MATH)
        {
            fprintf (out, "%d [shape=star];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->value);     
        }
        else 
        {
            fprintf (out, "%d [shape=oval];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->value); 
        }        
        fprintf (out, "\n");

        in_order_graph (node->right, out);

    }
    
    return;
}
