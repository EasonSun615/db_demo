//
// Created by syr on 2020/4/13.
//

#ifndef DB_DEMO_INPUTBUFFER_H
#define DB_DEMO_INPUTBUFFER_H

#include <cstddef>

/**
 * @brief 负责处理从标准输入读字符串到buffer里，字符串可能是Statement或者MetaCommand
 */
class InputBuffer{
public:
     char * buffer;
     size_t buffer_length;
     size_t input_length;
     InputBuffer():buffer(NULL),buffer_length(0),input_length(0){}
     ~InputBuffer(){delete buffer;}

     void read_input();

};

#endif //DB_DEMO_INPUTBUFFER_H
