#include <stdio.h>
#include <stdlib.h>
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
    
    struct Schema *schema = NULL;
    init_schema(&schema, "CFS");
    printf("Schema name: %s\n", schema->model_name);  
    
    parse_and_execute(schema, "ADD COL (Transaction ID) INT");
    parse_and_execute(schema, "ADD COL (Amount) FLOAT");
    parse_and_execute(schema, "ADD COL (Cost Center) STRING");
    
    for(int i = 0; i < schema->column_count; i++)
    { 
        printf("Column's name: %s\n", schema->columns[i].name);
        printf("Column's type: %u\n", schema->columns[i].type);
    }


    free(schema);
    return 0;
}
