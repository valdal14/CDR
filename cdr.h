#ifndef CDR_H
#define CDR_H

#include <stddef.h>

#define STR_SIZE 64
#define COL_NAME_SIZE 32
#define MAX_COL_NUM 16
#define MAX_SCHEMA_MODEL_NAME_SIZE 32
// Types 
#define SCHEMA 0
#define COLUMN 1
// Error Messages
#define ERRN01 fprintf(stderr, "Max number of columns (%d) reached for this schema\n", MAX_COL_NUM);
#define ERRN02 fprintf(stderr, "Unsupported object type\n");
#define ERRN03(schema_name) fprintf(stderr, "Schema named '%s' could not be allocated\n", schema_name);
#define ERRN04(col_name) fprintf(stderr, "Column named '%s' can't be added. Max number of columns reached\n", col_name);
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

struct ColumnDef
{
    char name[COL_NAME_SIZE];
    enum DataType type;
};

struct Schema
{
    char model_name[MAX_SCHEMA_MODEL_NAME_SIZE];
    struct ColumnDef columns[MAX_COL_NUM];
    int column_count;
};

// Engine routines
void print_variant(struct Variant *v);
void init_schema(struct Schema **schema, const char *name);
void show_error(int type, const char *name);
void add_column(struct Schema *schema, const char *col_name, enum DataType type);

#endif
