#include <string.h>
#include "cdr.h"

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
