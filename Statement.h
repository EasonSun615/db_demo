//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_STATEMENT_H
#define DB_DEMO_STATEMENT_H

#include "Row.h"

class InputBuffer;
class Table;

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef enum{
    PREPARE_SUCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_NEGTIVE_ID,
    PREPARE_STRING_TOO_LONG
}PrepareResult;

typedef enum{
    EXECUTE_SUCCESS,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_TABLE_FULL
}ExecuteResult;


class Statement{
public:
    StatementType _type;
    Row row_to_insert;     // only used by insert statement
    // prepare函数：根据输入的语句，设置成员变量
    PrepareResult prepare(InputBuffer * inputBuffer);
    // execute函数：根据设置好的成员变量，去执行语句
    ExecuteResult execute(Table * table);
    ExecuteResult execute_select(Table *table);
    ExecuteResult execute_insert(Table *table);
    PrepareResult prepare_insert(InputBuffer * inputBuffer);
};

#endif //DB_DEMO_STATEMENT_H
