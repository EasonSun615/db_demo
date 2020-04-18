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
//    printf("%s\n", keyword);
//    printf("%s\n", id_string);
//    printf("%s\n", username);
//    printf("%s\n", email);
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
//        _type = STATEMENT_INSERT;
//        int args_assigned = sscanf(inputBuffer->buffer, "insert %d %s %s", &(row_to_insert.id), row_to_insert.username,
//                                   row_to_insert.email);
//        if (args_assigned < 3) {
//            return PREPARE_SYNTAX_ERROR;
//        }
//        return PREPARE_SUCESS;
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

ExecuteResult Statement::execute_insert(Table *table) {
    void *_node = table->pager->get_page(table->root_page_num);
    LeafNode node(_node);
    if(*(uint32_t *)node.get_num_cells() >= LEAF_NODE_MAX_CELLS)
        return EXECUTE_TABLE_FULL;
    Cursor *cursor = table->end();
    node.insert(cursor, row_to_insert.id, &row_to_insert);
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