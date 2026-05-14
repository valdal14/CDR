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

    // Testing type inference
    printf("Type of '42': %d\n", infer_type("42"));
    printf("Type of '14.50': %d\n", infer_type("14.50"));
    printf("Type of 'Engineering': %d\n", infer_type("Engineering"));
}
