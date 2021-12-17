
#define $ded32 printf(">>> %d:\n", __LINE__);

#include ".\Diff.h"
#include ".\recursive_descent.h"

#define SYNTAX_ERROR                    \
        printf("Syntax Error\n");       \
        abort();                        \



extern char* s;



size_t find_file_size(FILE *in)
{
    assert (in != nullptr);

    int size_of_file = 0;

    fseek (in, 0, SEEK_END);

    size_of_file = ftell (in);

    fseek (in, 0, SEEK_SET);

    return (size_of_file);
}

char* fill_text(FILE* in, size_t file_size)
{
    assert(in != nullptr);

    char* text_array = (char*) calloc(file_size + 1, sizeof(char));

    if (text_array)
    {
        int n = fread(text_array, sizeof(char), file_size, in);
        *(text_array + n) = '\0';
        //(text_array + file_size + 1) ='\0';
        fseek(in, 0, SEEK_SET);
    }
    else
    {
        fseek(in, 0, SEEK_SET);
        return nullptr;
    }

    return text_array;
}


char* get_math_word(char* s)
{
    char* math_f = (char*)calloc(Math_function_len, sizeof(char));

    strncpy(math_f, s, Math_function_len);

    math_f[Math_function_len - 1] = '\0';

    if (strcmp("sin", math_f) == 0)
    {
        return math_f;
    }
    else if (strcmp("cos", math_f) == 0)
    {
        return math_f;
    }
    else
    {
        return nullptr;
    }
}

struct Pnode* input_operator(struct Pnode* node, char oper)
{
    node->type  = OPERATOR;
    node->value = oper;

    return node;
}

struct Pnode* getG(struct Pnode* node)
{
    struct Pnode* main_node = getE(node);

    if (*s == '$')
    {
        return main_node;
    }
    else SYNTAX_ERROR;

}


struct Pnode* getE(struct Pnode* node)
{
    struct Pnode* left_node = getT(node);

    if (*s == '+')
    {
        s++;

        struct Pnode* father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        father_node->left  = left_node;

        struct Pnode* right_node  = getT(node);

        father_node->right = right_node;

        father_node->type  = OPERATOR;

        father_node->value = '+';


        if (*s == '+')
        {
            struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            mega_father_node->type  = OPERATOR;

            mega_father_node->value = '+';

            mega_father_node->left  = father_node;

            assert(mega_father_node != nullptr);

            s++;

            mega_father_node->right = getE(node);

            return mega_father_node;
        }
        else
        {
            if (*s == '-')
            {
                struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                mega_father_node->type  = OPERATOR;

                mega_father_node->value = '-';

                mega_father_node->left  = father_node;

                assert(mega_father_node != nullptr);

                s++;

                mega_father_node->right = getE(node);

                return mega_father_node;
            }
        }

        return father_node;
    } 
    else
    {
        if (*s == '-')
        {
            s++;

            struct Pnode* father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            father_node->left  = left_node;

            struct Pnode* right_node  = getT(node);

            father_node->right = right_node;

            father_node->type  = OPERATOR;

            father_node->value = '-';

            if (*s == '-')
            {
                struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                mega_father_node->type  = OPERATOR;

                mega_father_node->value = '-';

                mega_father_node->left  = father_node;

                assert(mega_father_node != nullptr);

                s++;

                mega_father_node->right = getE(node);

                return mega_father_node;
            }
            else
            {
                if (*s == '+')
                {
                    struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                    mega_father_node->type  = OPERATOR;

                    mega_father_node->value = '+';

                    mega_father_node->left  = father_node;

                    assert(mega_father_node != nullptr);

                    s++;

                    mega_father_node->right = getE(node);

                    return mega_father_node;
                }
            }

            return father_node;
        }
        else
        {
            return left_node;
        }
    }
}

struct Pnode* getT(struct Pnode* node)
{
    struct Pnode* left_node   = getPw(node);

    if (*s == '*')
    {
        s++;

        struct Pnode* father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        father_node->left  = left_node;

        struct Pnode* right_node  = getPw(node);

        father_node->right = right_node;

        father_node->type  = OPERATOR;

        father_node->value = '*';

        if (*s == '*')
        {
            struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            mega_father_node->type  = OPERATOR;

            mega_father_node->value = '*';

            mega_father_node->left  = father_node;

            assert(mega_father_node != nullptr);

            s++;

            mega_father_node->right = getT(node);

            return mega_father_node;
        }
        else
        {
            if (*s == '/')
            {
                struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                mega_father_node->type  = OPERATOR;

                mega_father_node->value = '/';

                mega_father_node->left  = father_node;

                assert(mega_father_node != nullptr);

                s++;

                mega_father_node->right = getT(node);

                return mega_father_node;
            }
        }
        
        return father_node;
    } 
    else
    {
        if (*s == '/')
        {
            s++;

            struct Pnode* father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            father_node->left  = left_node;

            struct Pnode* right_node  = getPw(node);

            father_node->right = right_node;

            father_node->type  = OPERATOR;

            father_node->value = '/';

            if (*s == '/')
            {
                struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                mega_father_node->type  = OPERATOR;

                mega_father_node->value = '/';

                mega_father_node->left  = father_node;

                assert(mega_father_node != nullptr);

                s++;

                mega_father_node->right = getT(node);

                return mega_father_node;
            }
            else
            {
                if (*s == '*')
                {
                    struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

                    mega_father_node->type  = OPERATOR;

                    mega_father_node->value = '*';

                    mega_father_node->left  = father_node;

                    assert(mega_father_node != nullptr);

                    s++;

                    mega_father_node->right = getT(node);

                    return mega_father_node;
                }
            }

            return father_node;
        }
        else
        {
            return left_node;
        }
    }

}

struct Pnode* getPw(struct Pnode* node)
{
    struct Pnode* left_node = getP(node);

    if (*s == '^')
    {
        s++;

        struct Pnode* father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        father_node->left  = left_node;

        struct Pnode* right_node  = getP(node);

        father_node->right = right_node;

        father_node->type  = OPERATOR;

        father_node->value = '^';

        if (*s == '^')
        {
            struct Pnode* mega_father_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            mega_father_node->type  = OPERATOR;

            mega_father_node->value = '^';

            mega_father_node->left  = father_node;

            assert(mega_father_node != nullptr);

            s++;

            mega_father_node->right = getPw(node);

            return mega_father_node;
        }
        
        return father_node;  
    }
    else
    {
        return left_node;
    }
}

struct Pnode* getP(struct Pnode* node)
{
    if (*s == '(')
    {
        s++;

        struct Pnode* new_node = getE(node);

        if (*s != ')')
        {
            SYNTAX_ERROR;
        }
        else
        {
            s++;
            
            return new_node;
        }
    }
    else return get_math_F(node);
}


struct Pnode* get_math_F(struct Pnode* node)
{
    char* math_func = get_math_word(s);

    if (math_func != nullptr)
    {
        struct Pnode* func_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        s+= 3;

        func_node->value = 'F';
        func_node->type  = FUNCTION_NAME;
        func_node->left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        if (math_func[0] == 's')
        {
            func_node->left->value  = 's';
        }
        else
        {
            func_node->left->value = 'c';
        }
        func_node->left->type   = FUNCTION_MATH;
        func_node->right        = getP(node);

        return func_node;
    }
    else 
    {
        if (*s == 'l' && *(s + 1) == 'n')
        {
            struct Pnode* func_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            s+= 2;

            func_node->value = 'F';
            func_node->type  = FUNCTION_NAME;
            func_node->left  = (struct Pnode*)calloc(1, sizeof(struct Pnode));

            func_node->left->value  = 'l';
            func_node->left->type   = FUNCTION_MATH;
            func_node->right        =  getP(node); 

            return func_node;
        }
        else
        {
            return getV(node);
        }
    }
}


struct Pnode* getV(struct Pnode* node)
{
    if ( *s < '0' || '9' < *s && *s != '$' && *s != '^')
    {

        struct Pnode* new_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

        new_node->left   = nullptr;
        new_node->right  = nullptr;
        new_node->type   = VARIABLE;
        new_node->value  = *s;

        s++;

        return new_node;
    }
    else
    {

        struct Pnode* new_node = getN(node);

        return new_node;
    }
}

struct Pnode* getN(struct Pnode* node)
{

    int val = 0;

    const char* old = s;

    while ('0' <= *s && *s <= '9')
    {
        val = val * 10 + (*s - '0');

        s++;
    }

    if (old == s) 
    {   
        SYNTAX_ERROR;
    }
    
    struct Pnode* new_node = (struct Pnode*)calloc(1, sizeof(struct Pnode));

    new_node->type  = NUMBER;
    new_node->value = val;
    new_node->left  = nullptr;
    new_node->right = nullptr;

    return new_node;
}
