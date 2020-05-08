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
#include "InternalNode.h"

void MetaCommand::print_constants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}


// helper function for indent(缩进）
void indent(uint32_t level) {
    for (int i = 0; i < level; i++)
        printf("  ");
}

void MetaCommand::print_tree(Table *table, uint32_t page_num, uint32_t indentation_level) {
    void *page = table->pager->get_page(page_num);
    Node node(page);
    uint32_t num_keys, child;

    switch (node.get_type()) {
        case NODE_LEAF: {
            LeafNode lf(node);
            num_keys = *(uint32_t *) lf.get_num_cells();
            indent(indentation_level);
            printf("- leaf (size %d)\n", num_keys);
            for (int i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                printf("- %d\n", *lf.get_key(i));
            }
            break;
        }
        case NODE_INTERNAL: {
            InternalNode internal_node(node);
            num_keys = *internal_node.get_num_keys();
            indent(indentation_level);
            printf("- internal (size %d)\n", num_keys);
            for (int i = 0; i < num_keys; i++) {
                child = *internal_node.get_child(i);
                print_tree(table, child, indentation_level + 1);
                indent(indentation_level + 1);
                printf("- key %d\n", *internal_node.get_key(i));
            }
            child = *internal_node.get_right_child();
            print_tree(table, child, indentation_level + 1);
            break;
        }
    }
}

MetaCommandResult MetaCommand::execute(InputBuffer *input_buffer, Table *table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        delete table;
        exit(EXIT_SUCCESS);
    } else if (strcmp(input_buffer->buffer, ".btree") == 0) {
        printf("Tree:\n");
//        print_leaf_node(table->pager->get_page(0));
        print_tree(table, 0 ,0);
        return META_COMMAND_SUCCESS;
    } else if (strcmp(input_buffer->buffer, ".constants") == 0) {
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

