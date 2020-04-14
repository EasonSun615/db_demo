//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdio>
#include "Statement.h"
#include "InputBuffer.h"
#include "Table.h"
#include "Row.h"

PrepareResult Statement::prepare(InputBuffer *inputBuffer) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        _type = STATEMENT_INSERT;
        int args_assigned = sscanf(inputBuffer->buffer, "insert %d %s %s", &(row_to_insert.id), row_to_insert.username,
                                   row_to_insert.email);
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCESS;
    } else if (strcmp(inputBuffer->buffer, "select") == 0) {
        _type = STATEMENT_SELECT;
        return PREPARE_SUCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult Statement::execute_select(Table *table) {
    Row row;
    for(int i=0; i<table->num_rows; i++){
        char *source = (char *)table->row_slot(i);
        row.deserialize(source);
        row.show();
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_insert(Table *table) {
    if(table->num_rows > TABLE_MAX_ROWS)
        return EXECUTE_TABLE_FULL;
    char * destination = (char *)table->row_slot(table->num_rows);   // 获取新增的row在table中的位置
    row_to_insert.serialize(destination);      // 将insert的row序列化到table中
    table->num_rows++;
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