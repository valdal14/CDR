#ifndef CDR_H
#define CDR_H

#include <stddef.h>

#define STR_SIZE 64
#define COL_NAME_SIZE 32
#define MAX_COL_NUM 64
#define MAX_SCHEMA_MODEL_NAME_SIZE 32
#define MAX_TYPE_SIZE 16
// Types 
#define SCHEMA 0
#define COLUMN 1
#define FILEPATH 2
#define CSV 3
// Error Messages
#define ERRN01 fprintf(stderr, "Max number of columns (%d) reached for this schema\n", MAX_COL_NUM);
#define ERRN02 fprintf(stderr, "Unsupported object type\n");
#define ERRN03(schema_name) fprintf(stderr, "Schema named '%s' could not be allocated\n", schema_name);
#define ERRN04(col_name) fprintf(stderr, "Column named '%s' can't be added. Max number of columns reached\n", col_name);
#define ERRN05(cmd) fprintf(stderr, "Invalid command '%s'.Valid format is 'ADD COL (Column Name) STRING'\n", cmd); 
#define ERRN06(filepath) fprintf(stderr, "Could not open the file %s\n", filepath);
#define ERRN07(filepath) fprintf(stderr, "Cannot process %s it appears to be empty\n", filepath);
#define ERRN08(cmd) fprintf(stderr, "Invalid command: '%s'. Please type HELP for more information.\n", cmd);
// Lexer Tokens
#define TK_ADD_COL "ADD COL"
#define TK_ADD_COL_SIZE 7
#define TK_ADD_COL_REGEX "ADD COL (%[^)]) %s"
// Doc
#define MENU_PATH "./help/welcome.txt"
#define HELP_PATH "./help/help.txt"
// Shell Commands
#define EXIT 0
#define HELP 1
#define AUTO 2
#define MANUAL 3
// Report 
#define REPORT_PATH "reconciliation_report.log"

// Data Structures ===========================================================================================

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

struct Row
{
    struct Variant data[MAX_COL_NUM];
    int col_count;
};

struct Report
{
    int processed_lines;
    int success_lines;
    int failed_lines;
};

// Engine routines ===========================================================================================

/**
 * @brief Initialise the schema 
 * @param struct Schema double pointer
 * @param const char name: The name of the model 
 */
void init_schema(struct Schema **schema, const char *name);

/**
 * @brief Displays the error message related to the
 * memory allocation of a given type
 * @param int type: Defined in the Types in the cdr.h file
 * @param const char name
 * @return void
 */
void show_error(int type, const char *name);

/**
 * @brief Adds a new column to an existig Schema
 * @param struct Schema pointer
 * @param const char col_name: The name of the column
 * @param enum DataType type: The type of data stored in this column
 * @return void
 */
void add_column(struct Schema *schema, const char *col_name, enum DataType type);

/**
 * @brief Map the given string to the correct DataType
 * @param const char type_str
 * @return enum DataType 
 */
enum DataType map_string_to_type(const char *type_str);

/**
 * @brief Parses and executes the Schema
 * @param struct Schema active_schema
 * @param const char command
 * @return void
 */
void parse_and_execute(struct Schema *active_schema, const char *command);

/**
 * @brief Parse the CSV row 
 * @param struct Schema schema
 * @param char csv_line pointer
 * @param struct Row out_row pointer
 * @param const char separator: The CSV separator
 * @return void
 */
void parse_csv_row(struct Schema *schema, char *csv_line, struct Row *out_row, const char *separator);

/**
 * @brief Compares two rows
 * @param struct Row row_a
 * @param struct Row row_b
 * return int
 */
int compare_rows(struct Row *row_a, struct Row *row_b);

/**
 * @brief Execute the files reconciliation process
 * @param struct Schema schema
 * @param const char file_a pointer
 * @param const char file_b pointer
 * @param const char separator: The CSV separator
 * @param const char has_header: Whether the file has header or not
 * return struct Report
 */
struct Report run_reconciliation(struct Schema *schema, const char *file_a, const char *file_b, const char *separator, int has_header);

/**
 * @brief Infers the type based on the given value
 * @param const char str
 * @return enum DataType
 */
enum DataType infer_type(const char *str);

/**
 * @brief Builds a schema directly from a given csv file 
 * @param const char filepath pointer
 * @param const char separator
 * @return void
 */
void build_schema_from_csv(struct Schema *schema, const char *filepath, const char *separator);

/**
 * @brief Logs the failing lines to disk
 * @param FILE log_file pointer
 * @param int line 
 * @param const char file_a_name pointer
 * @param const char raw_a pointer
 * @param const char file_b_name pointer
 * @param const char raw_b pointer
 * @return void
 */
void log_mismatch(FILE *log, int line, const char *file_a_name, const char *raw_a, const char *file_b_name, const char *raw_b);

/**
 * @brief Log a success report with the statistics of the 
 * reconciliation process if no failures were detected.
 * @param FILE log_file pointer
 * return void
 */
void log_success_report(FILE *log_file, struct Report *stats);

#endif
