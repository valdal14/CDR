#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdr.h"

int main(void)
{
	struct Schema *schema = NULL;
	init_schema(&schema, "CFS");
	parse_and_execute(schema, "ADD COL (Transaction ID) INT");
	parse_and_execute(schema, "ADD COL (Amount) FLOAT");
	parse_and_execute(schema, "ADD COL (Cost Center) STRING");

	// Assuming mock_csv is a line from the csv file we will read
	char mock_csv[] = "42,14.50,Engineering";

	// Declare a blank row
	struct Row my_row;

	// The parser does the tokenizing and the casting!
	parse_csv_row(schema, mock_csv, &my_row, ",");

	// Verify
	for(int i = 0; i < my_row.col_count; i++)
	{
	    print_variant(&my_row.data[i]);
	}
	
    free(schema);
    return 0;
}
