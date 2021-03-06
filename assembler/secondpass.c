#include "assembler.h"

extern FILE * original_f;
extern FILE * post_macro_f;
extern FILE * copy_f;
extern const char * command_names[16];

int error_found_flag = 0;

void second_pass(symbol_linked_list * symbol_list, data_linked_list * data_list, address_linked_list * address_list)
{
    struct stat sb;

    char * file_contents;
    char * previous_word;

    const char * macro_start_keyword = "macro";
    const char * macro_end_keyword = "endm";
    const char * keywords[5] = {".data",
                                ".string",
                                ".extern",
                                ".entry",
                                ":"};

    if (stat("copy.am", &sb) == -1)
    {
        perror("stat error\n");
        exit(0);
    }   

    file_contents = malloc(sb.st_size);
    previous_word = malloc(sb.st_size);

    strcpy(previous_word, "");

    /* STEP 1 go over lines, if eof goto step 7 */
    while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
    {
        if (!strcmp(file_contents, macro_start_keyword))
        {
            while (strcmp(file_contents, macro_end_keyword))
            {
                fscanf(post_macro_f, " %[^\n ]", file_contents);
            }
            continue; /* going for next word after finding end of macro definition */
        }

        if (file_contents[0] == ',')
            file_contents = chop_first_n_characters(file_contents, 1);
        if (file_contents[strlen(file_contents) - 1] == ',')
            file_contents[strlen(file_contents) - 1] = '\0';

        /* STEP 4, check if entry, else go to step 6 */
        if (!strcmp(file_contents, keywords[3])) /* checking for .entry */
        {
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            if (file_contents[0] == ',')
                file_contents = chop_first_n_characters(file_contents, 1);
            if (file_contents[strlen(file_contents) - 1] == ',')
                file_contents[strlen(file_contents) - 1] = '\0';

            /* STEP 5 add entry attribute */
            if (!find_symbol_and_change_entry(symbol_list ,file_contents))
            {
                fprintf(stderr,"error : symbol %s wasn't found\n", file_contents);
                error_found_flag = 1;
            }
        }
    }
    convert_address_to_data(address_list, data_list, symbol_list);

    /* STEP 7 */
    if (error_found_flag)
    {
        fprintf(stderr, "errors were found during second pass, stopping program\n");
        exit(0);
    }
    bubble_sort_data_list(data_list->head);
    build_final_files(data_list, symbol_list, address_list);

    fclose(post_macro_f);
    fclose(original_f);
    fclose(copy_f);
    remove("copy.am");
}

void convert_address_to_data(address_linked_list * address_list, data_linked_list * data_list, symbol_linked_list * symbol_list)
{
    address_node * temp_address_node;
    symbol_node * temp_symbol_node;

    word_with_operands word_with;
    word_without_operands word_without;

    temp_address_node = address_list->head;

    reset_words(&word_with, &word_without);

    if (temp_address_node == NULL)
        return;
    
    while (temp_address_node != NULL)
    {
        if ((temp_symbol_node = find_symbol_with_name(symbol_list, temp_address_node->string)) != NULL)
        {
            word_without.R = 1;
            word_without.opcode = temp_symbol_node->base_address;
            add_to_data_list(data_list, temp_address_node->address, 0, word_with, word_without);
            word_without.opcode = temp_symbol_node->offset;
            add_to_data_list(data_list, temp_address_node->next_address, 0, word_with, word_without);
            reset_words(&word_with, &word_without);
        }
        else
        {
            error_found_flag = 1;
            fprintf(stderr, "didn't find symbol in list with the name %s\n", temp_address_node->string);
        }
        temp_address_node = temp_address_node->next;
    }
}