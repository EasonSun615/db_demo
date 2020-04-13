#include <iostream>
#include <cstring>
#include "InputBuffer.h"
#include "Statement.h"
#include "MetaCommand.h"

void print_prompt(){ printf("mydb >"); }


int main(int argc, char *argv[]) {
    InputBuffer *input_buffer = new InputBuffer;
    while(true){
        print_prompt();
        input_buffer->read_input();

        if(input_buffer->buffer[0] == '.'){
            // 处理 meta-commands
            MetaCommand metacommand;
            switch(metacommand.execute(input_buffer)){
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'.\n",input_buffer->buffer);
                    continue;
            }
        }

        // 处理sql语句（statement)
        Statement statement;
        switch(statement.prepare(input_buffer)){
            case PREPARE_SUCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n",input_buffer->buffer);
                continue;
        }
        statement.execute();
        printf("Executed.\n");
    }

}


