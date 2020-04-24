//
// Created by syr on 2020/4/17.
//


#include <cstdio>
#include <cstdlib>
#include "InternalNode.h"

//uint32_t *get_right_child();
//void *get_cell(uint32_t cell_num);
//uint32_t *get_child(uint32_t child_num);
//uint32_t *get_key(uint32_t key_num);

uint32_t *InternalNode::get_num_keys() {
    return (uint32_t *) ((uint8_t *) node + INTERNAL_NODE_NUM_LEYS_OFFSET);
}

uint32_t *InternalNode::get_right_child() {
    return (uint32_t *)((uint8_t *)node + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}


void *InternalNode::get_cell(uint32_t cell_num){
    return (uint8_t *)node + INTERNAL_NODE_HEADER_SIZE + INTERNAL_NODE_CELL_SIZE * cell_num;
}

uint32_t *InternalNode::get_child(uint32_t child_num){
    uint32_t num_keys = *get_num_keys();
    if(child_num > num_keys){
        printf("Tried to access child_num %d > num_keys %d\n",child_num, num_keys);
        exit(EXIT_FAILURE);
    }else if(child_num == num_keys){
        return get_right_child();
    }else{
        return (uint32_t *)get_cell(child_num);
    }
}

uint32_t *InternalNode::get_key(uint32_t key_num) {
    return (uint32_t *)((uint8_t *)get_child(key_num) + INTERNAL_NODE_CHILD_SIZE);
}

uint32_t InternalNode::get_max_key() {
    uint32_t num_keys = *get_num_keys();
    return *get_key(num_keys-1);
}

void InternalNode::init(){
    *((uint32_t *)get_num_keys()) = 0;
    set_type(NODE_INTERNAL);
    set_root(false);
}

