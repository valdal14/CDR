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

/**
 * @brief Displays the error message related to the 
 * memory allocation of a given type
 * @param int type: Defined in the Types in the cdr.h file
 * @param const char name 
 * @return void
 */
void show_error(int type, const char *name)
{
   switch(type)
   {
       case 0:
           ERRN03(name);
           break;
       case 1:
           ERRN04(name);
           break;
       default:
           ERRN02;
           break;
   }

   exit(1);
}

/**
 * @brief Initialise the schema 
 * @param struct Schema double pointer
 * @param const char name: The name of the model 
 */
void init_schema(struct Schema **schema, const char *name)
{
    struct Schema *new_schema = (struct Schema *)malloc(sizeof(struct Schema));
    if(new_schema == NULL) show_error(SCHEMA, name);
        
    strncpy(new_schema->model_name, name, MAX_SCHEMA_MODEL_NAME_SIZE);
    new_schema->column_count = 0;

    *schema = new_schema;
}

/**
 * @brief Adds a new column to an existig Schema
 * @param struct Schema pointer
 * @param const char col_name: The name of the column
 * @param enum DataType type: The type of data stored in this column
 * @return void
 */
void add_column(struct Schema *schema, const char *col_name, enum DataType type)
{
    if(schema->column_count >= MAX_COL_NUM) show_error(COLUMN, col_name);
    strncpy(schema->columns[schema->column_count].name, col_name, COL_NAME_SIZE);
    schema->columns[schema->column_count].type = type;
    schema->column_count += 1;
}


