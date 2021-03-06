#include "assembler.h"

symbol_linked_list * create_empty_symbol_list()
{
    symbol_linked_list * new_head;
    new_head = (symbol_linked_list *)malloc(sizeof(symbol_linked_list));
    new_head->head = NULL;

    return new_head;
}

void free_symbol_list(symbol_node * head)
{
    symbol_node * temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

symbol_node * add_to_symbol_list(symbol_linked_list * list, char * new_symbol, int new_decimal_value, int new_base_adress, int new_offset, struct attributes new_attributes)
{
    symbol_node * new_node;
    symbol_node * temp;

    new_node = (symbol_node *)malloc(sizeof(symbol_node));

    new_node->symbol =(char *)malloc(MAX_LENGTH * sizeof(char));
    strcpy(new_node->symbol, new_symbol);

    new_node->decimal_value = new_decimal_value;
    new_node->base_address = new_base_adress;
    new_node->offset = new_offset;
    new_node->symbol_attributes = new_attributes;

    new_node->next = NULL;

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else
    {
        temp = list->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    return (list->head);
}

void print_symbol_list(symbol_linked_list * list)
{
    symbol_node * temp;
    temp = list->head;

    while(temp != NULL)
    {
        printf("symbol name is %s\n", temp->symbol);
        printf("decimal value is %d\n", temp->decimal_value);
        printf("base adress is %d\n", temp->base_address);
        printf("offset is %d\n", temp->offset);
        printf("external %d code %d entry %d data %d\n", temp->symbol_attributes.external, temp->symbol_attributes.code, temp->symbol_attributes.entry, temp->symbol_attributes.data);
    
        temp = temp->next;
    }
}

int find_symbol_and_change_entry(symbol_linked_list * list, char * symbol_name)
{
    symbol_node * temp;

    temp = list->head;

    while (temp != NULL)
    {
        if (!strcmp(symbol_name, temp->symbol))
        {
            temp->symbol_attributes.entry = 1;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

symbol_node * find_symbol_with_name(symbol_linked_list * list, char * symbol_name)
{
    symbol_node * temp;
    temp = list->head;

    while (temp != NULL)
    {
        if (!strcmp(temp->symbol, symbol_name))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int get_number_of_symbol_nodes(symbol_linked_list * list)
{
    symbol_node * temp;
    int count = 0;

    temp = list->head;

    while (temp != NULL)
    {
        temp = temp->next;
        count++;
    }
    
    return count;
}