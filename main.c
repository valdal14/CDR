#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdr.h"

/**
 * @brief Shows the welcome text to inform the user
 * on how to use the program
 * @param void(*on_file_open)(FILE *file) callback
 * @return void
 */
void show_menu(void(*on_file_open)(FILE *file))
{
    FILE *menu = fopen(MENU_PATH, "r");
    if(menu == NULL) show_error(FILEPATH, "welcome.txt");
    on_file_open(menu);
}

/**
 * @brief Show the help menu 
 * @param void(*on_file_open)(FILE *file) callback
 * @return void
 */
void show_help(void(*on_file_open)(FILE *file))
{
    FILE *help = fopen(HELP_PATH, "r");
    if(help == NULL) show_error(FILEPATH, "help.txt");
    on_file_open(help);
}

/**
 * @brief Shows the stats about the number of lines that were processed
 * @param struct Report pointer
 * @return void
 */
void show_stats(struct Report *stats)
{
    printf("----------------------------------------------------\n");
    printf("📊 Total Lines Processed : %d\n", stats->processed_lines);
    printf("✅ Successful Lines      : %d\n", stats->success_lines);
    printf("❌ Failed Lines          : %d\n", stats->failed_lines);
    printf("📄 Report saved to       : reconciliation_report.log\n");
    printf("----------------------------------------------------\n");
}

/**
 * @brief Reads the content from the given FILE pointer
 * @param FILE file pointer
 * @return void
 */
void read(FILE *file)
{
    char lines[256];

    while(fgets(lines, sizeof(lines), file) != NULL)
    {
        printf("%s", lines);
    }

    printf("\n");
    fclose(file);
}

/**
 * @brief Starts the MANUAL process for the reconciliation
 * @return void
 */
void init_manual_process(void)
{
    struct Schema *schema = NULL;
    init_schema(&schema, "Manual Schema");

    char file_a[256], file_b[256], sep[16];
    char cmd_buffer[256];
   
    printf("Enter schema commands (Type 'RUN' to finish building):\n");
    printf("Example: ADD COL (Transaction ID) INT\n");
    printf("After you have completed the schema type RUN to continue\n");
    while(1)
    {
        printf("CDR 📄 schema> ");

        if(fgets(cmd_buffer, sizeof(cmd_buffer), stdin) != NULL)
        {
            // Strip the newline
            cmd_buffer[strcspn(cmd_buffer, "\r\n")] = 0;
            if(strlen(cmd_buffer) == 0) continue;

            if (strcmp(cmd_buffer, "RUN") == 0) break;
                    
            parse_and_execute(schema, cmd_buffer);
        }
    }

    printf("\nEnter File A, File B, and Separator (space separated):\n> ");
    scanf("%255s %255s %15s", file_a, file_b, sep);

    // Flush the leftover newline from the buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n⚙️ Executing Engine...\n");
    // Assuming manual headless dumps don't have headers
    struct Report stats = run_reconciliation(schema, file_a, file_b, sep, 0); 
            
    show_stats(&stats);
    
    free(schema);
}

/**
 * @brief Starts the AUTOMATIC process for the reconciliation
 * @return void
 */
void init_automatic_process(void)
{
    struct Schema *schema = NULL;
    init_schema(&schema, "Manual Schema");

    char file_a[256], file_b[256], sep[16];
    printf("\n[ AUTO-INFERENCE MODE ]\n");
    printf("Enter File A, File B, and Separator (space separated):\n> ");
            
    scanf("%255s %255s %15s", file_a, file_b, sep);

    // Flush the leftover newline from the buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    build_schema_from_csv(schema, file_a, sep);
   
    printf("\n⚙️ Executing Engine...\n");
    
    struct Report stats = run_reconciliation(schema, file_a, file_b, sep, 1); // 1 = Skip Headers!
    
    show_stats(&stats);
    
    free(schema);
}

/**
 * @brief Handles the incoming buffer and the logic for the commands
 * execution flow.
 * @param char cmd The buffer read 
 * @param int cmd_type The type of the command
 * @return void
 */
void handle_command(char *cmd, int cmd_type)
{
    switch(cmd_type)
    {
        case 0:
            exit(1);
        case 1:
            show_help(read);
            break;
        case 2:
            init_automatic_process();
            break;
        case 3:
            init_manual_process();
            break;
        default:
            ERRN08(cmd);
            break;
    }
}

/**
 * @brief Program Loop - Read user commands
 * @return void
 */
void get_user_input(void)
{
    char buffer[256];
    
    while(1)
    {
        printf("CDR> ");

        if(fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            
            if(strcmp(buffer, "EXIT") == 0 || strcmp(buffer, "exit") == 0)
            {
                handle_command(buffer, EXIT);
            }
            else if(strcmp(buffer, "HELP") == 0 || strcmp(buffer, "help") == 0)
            {
                handle_command(buffer, HELP);
            }
            else if(strcmp(buffer, "AUTO") == 0 || strcmp(buffer, "auto") == 0)
            {
                handle_command(buffer, AUTO);
            }
            else if(strcmp(buffer, "MANUAL") == 0 || strcmp(buffer, "manual") == 0)
            {
                handle_command(buffer, MANUAL);
            }
            else
            {
                handle_command(buffer, -1);
            }
        }
    }
}

int main(void)
{
    show_menu(read);
    get_user_input();
    return 0;
}
