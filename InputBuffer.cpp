//
// Created by syr on 2020/4/13.
//

#include "InputBuffer.h"
#include <iostream>

void InputBuffer::read_input() {
    size_t bytes_read = getline(&buffer, &buffer_length, stdin);
    if(bytes_read<=0){
        std::cout<<"Error reading input"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 忽略换行符
    input_length = bytes_read-1;
    buffer[bytes_read-1] = '\0';
}

