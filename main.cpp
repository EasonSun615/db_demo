#include <iostream>
#include <cstring>
#include "InputBuffer.h"

void print_prompt(){ printf("mydb >"); }


int main(int argc, char *argv[]) {
    InputBuffer *input_buffer = new InputBuffer;
    while(true){
        print_prompt();
        input_buffer->read_input();
        if(strcmp(input_buffer->buffer, ".exit")==0) {
            exit(EXIT_SUCCESS);
        }else{
            printf("Unrecognized command '%s'.\n",input_buffer->buffer);
        }
    }

}


