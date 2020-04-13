//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdio>
#include "Statement.h"
#include "InputBuffer.h"

PrepareResult Statement::prepare(InputBuffer *inputBuffer) {
    if(strncmp(inputBuffer->buffer, "insert", 6)==0){
        _type = STATEMENT_INSERT;
        return PREPARE_SUCESS;
    }else if(strcmp(inputBuffer->buffer, "select")==0){
        _type = STATEMENT_SELECT;
        return PREPARE_SUCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void Statement::execute() {
    switch(_type){
        case STATEMENT_SELECT:
            printf("This is where we would do a select.\n");
            break;
        case STATEMENT_INSERT:
            printf("This is where we would do an insert.\n");
            break;
    }
}