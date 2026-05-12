#ifndef CDR_H
#define CDR_H

#include <stddef.h>

#define STR_SIZE 64

// Data Structures 
enum DataType
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
};

union DataValue
{
    int i_val;
    float f_val;
    char s_val[STR_SIZE];
};

struct Variant
{
    enum DataType type;
    union DataValue value;
};

// Engine routines
void print_variant(struct Variant *v);

#endif
