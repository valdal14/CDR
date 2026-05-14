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

    int total_lines = run_reconciliation(schema, "bucket_a.csv", "bucket_b.csv", ","); 
    printf("-------------------------------\n");
    printf("📊 Analysed %d lines\n", total_lines);

    free(schema);

    // Testing build schema from csv 
    struct Schema *new_schema = NULL;
    init_schema(&new_schema, "Auto_CFS");

    // Build it automatically!
    build_schema_from_csv(new_schema, "bucket_a_with_header.csv", ",");

    // Prove it worked
    for(int i = 0; i < new_schema->column_count; i++)
    {
        printf("Auto-Column: %s | Type: %u\n", new_schema->columns[i].name, new_schema->columns[i].type);
    }
}
