//
// Created by syr on 2020/4/17.
//


#include <cstdio>
#include <cstdlib>
#include "InternalNode.h"
#include "Table.h"
#include "LeafNode.h"

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

Cursor *InternalNode::internal_node_find(Table *table, uint32_t page_num, uint32_t key) {
    void *page = table->pager->get_page(page_num);
    InternalNode internal_node(page);
    uint32_t num_keys = *internal_node.get_num_keys();
    uint32_t min_index = 0;
    uint32_t max_index = num_keys;
    // [0, num_keys]
    while (min_index < max_index) {
        uint32_t index = min_index + (max_index - min_index) / 2;
        uint32_t key_to_right = *internal_node.get_key(index);
        if(key_to_right < key){
            min_index = index+1;
        }else if(key_to_right == index){
            min_index = max_index = index;
        }else if(key_to_right > key){
            max_index = index;
        }
    }
    uint32_t child_page_num = *internal_node.get_child(min_index);
    Node child_node(table->pager->get_page(child_page_num));
    switch(child_node.get_type()){
        case NODE_INTERNAL:
            return internal_node_find(table, child_page_num, key);
        case NODE_LEAF:
            return LeafNode::leaf_node_find(table, child_page_num, key);
    }
}

