//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdlib>
#include "MetaCommand.h"
#include "Table.h"
#include "InputBuffer.h"

MetaCommandResult MetaCommand::execute(InputBuffer *input_buffer, Table * table){
    if(strcmp(input_buffer->buffer, ".exit")== 0){
        delete table;
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

