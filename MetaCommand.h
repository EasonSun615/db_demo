//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_METACOMMAND_H
#define DB_DEMO_METACOMMAND_H

#include <stdint.h>
class InputBuffer;
class Table;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
}MetaCommandResult;

/**
 * @brief 负责执行一些命令，包括：.btree, .constants, .exit
 */
class MetaCommand{
public:
    MetaCommandResult execute(InputBuffer *input_buffer, Table *table);
    void print_tree(Table *table, uint32_t page_num, uint32_t indentation_level);
    void print_constants();
};


#endif //DB_DEMO_METACOMMAND_H
