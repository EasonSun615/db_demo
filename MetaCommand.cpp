//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "MetaCommand.h"
#include "Table.h"
#include "InputBuffer.h"
#include "LeafNode.h"

void print_constants(){
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void print_leaf_node(void *node){
    LeafNode lf(node);
    uint32_t num_cells = *(uint32_t *)lf.get_num_cells();
    printf("leaf (size %d)\n", num_cells);
    for (uint32_t i = 0; i < num_cells; i++) {
        uint32_t key= *(uint32_t *)lf.get_key(i);
        printf("  - %d : %d\n", i, key);
    }
}

MetaCommandResult MetaCommand::execute(InputBuffer *input_buffer, Table * table){
    if(strcmp(input_buffer->buffer, ".exit")== 0){
        delete table;
        exit(EXIT_SUCCESS);
    }else if (strcmp(input_buffer->buffer, ".btree") == 0) {
        printf("Tree:\n");
        print_leaf_node(table->pager->get_page(0));
        return META_COMMAND_SUCCESS;
    }else if(strcmp(input_buffer->buffer, ".constants") == 0){
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

