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

/**
 * @brief Map the given string to the correct DataType
 * @param const char type_str
 * @return enum DataType 
 */
enum DataType map_string_to_type(const char *type_str)
{
    if((strcmp(type_str, "INT") == 0) || (strcmp(type_str, "int") == 0)) return TYPE_INT;
    if((strcmp(type_str, "FLOAT") == 0) || (strcmp(type_str, "float") == 0)) return TYPE_FLOAT;
    if((strcmp(type_str, "STRING") == 0) || (strcmp(type_str, "string") == 0)) return TYPE_STRING;

    ERRN02;
    exit(1);
}

/**
 * @brief Parses and executes the Schema
 * @param struct Schema active_schema
 * @param const char command
 * @return void
 */
void parse_and_execute(struct Schema *active_schema, const char *command)
{
    char parsed_name[COL_NAME_SIZE];
    char parsed_type[MAX_TYPE_SIZE];

    if(strncmp(command, TK_ADD_COL, TK_ADD_COL_SIZE) == 0)
    {
        sscanf(command, TK_ADD_COL_REGEX, parsed_name, parsed_type);
        enum DataType type = map_string_to_type(&parsed_type[0]);
        // add column
        add_column(active_schema, parsed_name, type);
    }
    else
    {
        ERRN05(command);
    }
}










