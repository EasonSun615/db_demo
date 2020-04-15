//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Statement.h"
#include "InputBuffer.h"
#include "Table.h"
#include "Row.h"


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
    for(int i=0; i<table->pager->num_rows; i++){
        char *source = (char *)table->row_slot(i);
        row.deserialize(source);
        row.show();
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_insert(Table *table) {
    if(table->pager->num_rows > TABLE_MAX_ROWS)
        return EXECUTE_TABLE_FULL;
    char * destination = (char *)table->row_slot(table->pager->num_rows);   // 获取新增的row在table中的位置
    row_to_insert.serialize(destination);      // 将insert的row序列化到table中
    table->pager->num_rows++;
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