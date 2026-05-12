#include <stdio.h>
#include <stdlib.h>
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
