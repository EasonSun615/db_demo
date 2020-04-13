//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_STATEMENT_H
#define DB_DEMO_STATEMENT_H

class InputBuffer;

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef enum{
    PREPARE_SUCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;


class Statement{
public:
    StatementType _type;
    // prepare函数：根据输入的语句，设置成员变量
    PrepareResult prepare(InputBuffer * inputBuffer);
    // execute函数：根据设置好的成员变量，去执行语句
    void execute();
};

#endif //DB_DEMO_STATEMENT_H
