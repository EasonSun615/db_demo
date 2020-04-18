//
// Created by syr on 2020/4/17.
//

#include "LeafNode.h"
#include "Cursor.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

void *LeafNode::get_num_cells(){
    return (unsigned char *)node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *LeafNode::get_cell(uint32_t cell_num) {
    return (unsigned char *)node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

void *LeafNode::get_key(uint32_t cell_num) {
    return get_cell(cell_num);
}

void *LeafNode::get_value(uint32_t cell_num) {
    return (unsigned char*)get_cell(cell_num) + LEAF_NODE_KEY_SIZE;
}

void LeafNode::init() {
    *(uint32_t *)get_num_cells() = 0;
}

void LeafNode::insert(Cursor *cursor, uint32_t key, Row *value) {
    uint32_t num_cells = *(uint32_t *)get_num_cells();
    if(num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        // Since We haven’t implemented splitting yet, so we error if the node is full.
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }
    if(cursor->cell_num < num_cells){
        // Make room for new cell
        for(int i=num_cells-1; i>=cursor->cell_num; i--){
            memcpy(get_cell(i+1), get_cell(i),LEAF_NODE_CELL_SIZE);
        }
    }
    *(uint32_t *)get_num_cells() += 1;
    *(uint32_t *)get_key(cursor->cell_num) = key;
    value->serialize(get_value(cursor->cell_num));
}