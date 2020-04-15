#include <iostream>
#include <cstring>
#include "InputBuffer.h"
#include "Statement.h"
#include "MetaCommand.h"
#include "Table.h"

inline void print_prompt(){ printf("mydb >"); }


int main(int argc, char *argv[]) {
    if(argc<2){
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }
    char *filename = argv[1];
    InputBuffer *input_buffer = new InputBuffer;
    Table *table = new Table(filename);
    while(true){
        print_prompt();
        input_buffer->read_input();

        if(input_buffer->buffer[0] == '.'){
            // 处理 meta-commands
            MetaCommand metacommand;
            switch(metacommand.execute(input_buffer, table)){
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
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n",input_buffer->buffer);
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
            case PREPARE_NEGTIVE_ID:
                printf("Id must be positive.\n");
                continue;
        }
        switch(statement.execute(table)){
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;
        }
    }
}


