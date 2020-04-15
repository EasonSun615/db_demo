//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_METACOMMAND_H
#define DB_DEMO_METACOMMAND_H

class InputBuffer;
class Table;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
}MetaCommandResult;

class MetaCommand{
public:
    MetaCommandResult execute(InputBuffer *input_buffer, Table *table);
};


#endif //DB_DEMO_METACOMMAND_H
