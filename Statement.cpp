//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Statement.h"
#include "InputBuffer.h"
#include "Table.h"
#include "Cursor.h"
#include "Row.h"
#include "LeafNode.h"


PrepareResult Statement::prepare_insert(InputBuffer *inputBuffer) {
    _type = STATEMENT_INSERT;
    char *keyword = strtok(inputBuffer->buffer, " ");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");
    if(id_string==NULL || username == NULL || email == NULL){
        return PREPARE_SYNTAX_ERROR;
    }
    int id = atoi(id_string);
    if(id<0)
        return PREPARE_NEGTIVE_ID;
    if(strlen(username) > COLUMN_USERNAME_SIZE || strlen(email) > COLUMN_EMAIL_SIZE)
        return PREPARE_STRING_TOO_LONG;
    row_to_insert.id = id;
    strcpy(row_to_insert.username, username);
    strcpy(row_to_insert.email, email);
    return PREPARE_SUCESS;
}

PrepareResult Statement::prepare(InputBuffer *inputBuffer) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        return prepare_insert(inputBuffer);
    } else if (strcmp(inputBuffer->buffer, "select") == 0) {
        _type = STATEMENT_SELECT;
        return PREPARE_SUCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult Statement::execute_select(Table *table) {
    Row row;
    for(Cursor *cursor = table->begin(); cursor->cell_num!=table->end()->cell_num;  cursor->cell_num++){
        void *source = cursor->get_value();
        row.deserialize(source);
        row.show();
    }
    return EXECUTE_SUCCESS;
}

Cursor *leaf_node_find(Table *table, uint32_t root_page_num, uint32_t key){
    void *page = table->pager->get_page(root_page_num);
    LeafNode ln(page);
    uint32_t num_cells = *(uint32_t *)ln.get_num_cells();
    Cursor *cursor = new Cursor(root_page_num, 0);
    cursor->pager = table->pager;
    // Binary search. 注意返回值的区间：[min_index, max_index]
    uint32_t min_index = 0;
    uint32_t max_index = num_cells;
    while(max_index > min_index){
        uint32_t index = min_index + (max_index- min_index)/2;
        uint32_t key_at_index = *ln.get_key(index);
        if(key_at_index == key){
            cursor->cell_num = index;
            return cursor;
        }else if(key < key_at_index){
            max_index = index;
        }else if(key> key_at_index){
            min_index = index+1;
        }
    }
    cursor->cell_num = min_index;
    return cursor;
}

Cursor *table_find(Table *table, uint32_t key){
    uint32_t root_page_num = table->root_page_num;
    void *page = table->pager->get_page(root_page_num);
    Node node(page);
    if(node.get_type() == NODE_LEAF){
        return leaf_node_find(table, root_page_num, key);
    }else{
        printf("Need to implement searching an internal node\n");
        exit(EXIT_FAILURE);
    }
}

ExecuteResult Statement::execute_insert(Table *table) {
    void *_node = table->pager->get_page(table->root_page_num);
    LeafNode node(_node);
    uint32_t num_cells = *(uint32_t *)node.get_num_cells();
    if(num_cells >= LEAF_NODE_MAX_CELLS)
        return EXECUTE_TABLE_FULL;
    uint32_t key_to_insert = row_to_insert.id;
    Cursor *cursor = table_find(table, key_to_insert);
    if(cursor->cell_num < num_cells){
        uint32_t key_at_index = *node.get_key(cursor->cell_num);
        if(key_at_index == key_to_insert)
            return EXECUTE_DUPLICATE_KEY;
    }
    node.insert(cursor, key_to_insert, &row_to_insert);
//    Cursor *cursor = table->end();
//    node.insert(cursor, row_to_insert.id, &row_to_insert);
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute(Table *table) {
    switch (_type) {
        case STATEMENT_SELECT:
            return execute_select(table);
        case STATEMENT_INSERT:
            return execute_insert(table);
    }
}