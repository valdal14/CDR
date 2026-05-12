#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdr.h"

/**
 * @brief Prints out the Variant
 * @param struct Variant pointer
 * @return void
 */
void print_variant(struct Variant *v)
{
    switch(v->type)
    {
        case TYPE_INT:
            printf("INT: %d\n", v->value.i_val);
            break;
        case TYPE_FLOAT:
            printf("FLOAT: %.2f\n", v->value.f_val);
            break;
        case TYPE_STRING:
            printf("STRING: %s\n", v->value.s_val);
            break;
        default:
            fprintf(stderr, "Unsupported Variant type\n");
            exit(1);
    }
}

int main(void)
{
    struct Variant v1, v2, v3;
    v1.type = TYPE_INT;
    v1.value.i_val = 42;

    v2.type = TYPE_FLOAT;
    v2.value.f_val = 14.50;

    v3.type = TYPE_STRING;
    strncpy(v3.value.s_val, "Cost Center", STR_SIZE);
    
    print_variant(&v1);
    print_variant(&v2);
    print_variant(&v3);

    return 0;
}
