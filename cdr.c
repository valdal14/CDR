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
       case 2:
           ERRN06(name);
           break;
       case 3:
           ERRN07(name);
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

/**
 * @brief Parse the CSV row 
 * @param struct Schema schema
 * @param char csv_line pointer
 * @param struct Row out_row pointer
 * @param const char separator: The CSV separator
 * @return void
 */
void parse_csv_row(struct Schema *schema, char *csv_line, struct Row *out_row, const char *separator)
{
    out_row->col_count = 0;
    // granb the first token 
    char *token = NULL;

    for(int i = 0; i <  schema->column_count; i++)
    {
        // granb the first token from the csv_line
        if(i == 0)
        {
            token = strtok(csv_line, separator);
            if(token == NULL) break;
        }
        else
        {
            // grab the remaing tokens in the csv_line
            token = strtok(NULL, separator);
            if(token == NULL) break;
        }
        
        switch(schema->columns[i].type)
        {
            case TYPE_INT:
            {
                out_row->data[i].type = TYPE_INT;
                out_row->data[i].value.i_val = atoi(token);
                break;
            }
            case TYPE_FLOAT:
            {    
                out_row->data[i].type = TYPE_FLOAT;
                out_row->data[i].value.f_val = atof(token);
                break;
            }
            case TYPE_STRING:
            {
                out_row->data[i].type = TYPE_STRING;
                strncpy(out_row->data[i].value.s_val, token, STR_SIZE); 
                break;
            }
            default:
                ERRN02;
                exit(1);
        }

        // increment the row column count
        out_row->col_count += 1;
    }
}

/**
 * @brief Compares two rows
 * @param struct Row row_a
 * @param struct Row row_b
 * return int
 */
int compare_rows(struct Row *row_a, struct Row *row_b)
{
    if(row_a->col_count != row_b->col_count) return 0;

    for(int i = 0; i < row_a->col_count; i++)
    {
        switch(row_a->data[i].type)
        {
            case TYPE_INT:
                if(row_a->data[i].value.i_val != row_b->data[i].value.i_val) return 0;
                break;
            case TYPE_FLOAT:
                if(row_a->data[i].value.f_val != row_b->data[i].value.f_val) return 0;
                break;
            case TYPE_STRING:
                if(strcmp(row_a->data[i].value.s_val, row_b->data[i].value.s_val) != 0) return 0;
                break;
            default:
                return 0;
        }
    }
    
    return 1;
}

/**
 * @brief Execute the files reconciliation process
 * @param struct Schema schema
 * @param const char file_a pointer
 * @param const char file_b pointer
 * @param const char separator: The CSV separator
 * return int
 */
int run_reconciliation(struct Schema *schema, const char *file_a, const char *file_b, const char *separator)
{
    int line_number = 1;
    char line_a[256], line_b[256];
    struct Row row_a, row_b;

    FILE *fa = fopen(file_a, "r");
    if(fa == NULL) show_error(FILEPATH, file_a);
    
    FILE *fb = fopen(file_b, "r");
    if(fb == NULL) show_error(FILEPATH, file_b);

    while(fgets(line_a, sizeof(line_a), fa) != NULL && fgets(line_b, sizeof(line_b), fb) != NULL)
    {
        parse_csv_row(schema, line_a, &row_a, separator);
        parse_csv_row(schema, line_b, &row_b, separator);
        
        int are_equals = compare_rows(&row_a, &row_b);
        
        if(are_equals == 1)
        {
            printf("✅ Rows are identical\n");
        }
        else
        {
            printf("❌ [ERROR] Mismatch found on line %d\n", line_number);
        }

        line_number += 1;
    }

    fclose(fa);
    fclose(fb);
    // keep track for reporting and statistic 
    // without the need of creating a new variable.
    return line_number - 1;
}

/**
 * @brief Infers the type based on the given value
 * @param const char str
 * @return enum DataType
 */
enum DataType infer_type(const char *str)
{
    // Check first for  NULL pointers or empty strings
    if(str == NULL || *str == '\0') return TYPE_STRING;

    char *endptr;
    
    // Check for integers 
    strtol(str, &endptr, 10);
    if(str != endptr && *endptr == '\0') return TYPE_INT;
    
    // Check for floats 
    if(strchr(str, '.')) 
    {
        strtod(str, &endptr);
        if(str != endptr && *endptr == '\0') return TYPE_FLOAT;
    }

    return TYPE_STRING;
}

/**
 * @brief Builds a schema directly from a given csv file 
 * @param const char filepath pointer
 * @param const char separator
 * @return void
 */
void build_schema_from_csv(struct Schema *schema, const char *filepath, const char *separator)
{
    FILE *file = fopen(filepath, "r");
    if(file == NULL) show_error(FILEPATH, filepath);

    char header_buffer[256];
    char data_buffer[256];

    schema->column_count = 0;

    // Grab both lines
    if (fgets(header_buffer, sizeof(header_buffer), file) == NULL) show_error(CSV, filepath);
    if (fgets(data_buffer, sizeof(data_buffer), file) == NULL) show_error(CSV, filepath);
    
    fclose(file);

    // Strip newlines from both buffers
    header_buffer[strcspn(header_buffer, "\r\n")] = 0;
    data_buffer[strcspn(data_buffer, "\r\n")] = 0;

    // PARSE HEADER: Extract column's names
    char *h_tok = strtok(header_buffer, separator);
    while (h_tok != NULL && schema->column_count < MAX_COL_NUM) 
    {
        strncpy(schema->columns[schema->column_count].name, h_tok, COL_NAME_SIZE);
        schema->column_count++;
        h_tok = strtok(NULL, separator);
    }

    // PARSE DATA: Infer types
    int i = 0;
    char *d_tok = strtok(data_buffer, separator);
    while (d_tok != NULL && i < schema->column_count)
    {
        schema->columns[i].type = infer_type(d_tok);
        i++;
        d_tok = strtok(NULL, separator);
    }
}
