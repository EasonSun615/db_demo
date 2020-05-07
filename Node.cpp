//
// Created by syr on 2020/4/20.
//


#include "Node.h"
#include "Table.h"
#include <memory.h>
#include "InternalNode.h"
#include "LeafNode.h"

void Node::set_type(NodeType node_type) {
    uint8_t value = node_type;
    *(uint8_t *)((unsigned char *)node + NODE_TYPE_OFFSET) = value;
}

NodeType Node::get_type() {
    uint8_t node_type= *(uint8_t *)((unsigned char *)node + NODE_TYPE_OFFSET);
    return (NodeType)node_type;
}

bool Node::is_root() {
    uint8_t is_root = *((uint8_t *)node + IS_ROOT_OFFSET);
    return (bool)is_root;
}

void Node::set_root(bool is_root) {
    uint8_t value = (uint8_t)is_root;
    *((uint8_t *)node + IS_ROOT_OFFSET) = value;
}

uint32_t Node::get_max_key() {
    switch(get_type()){
        case NODE_LEAF:{
            LeafNode leaf_node(node);
            return leaf_node.get_max_key();
        }
        case NODE_INTERNAL:{
            InternalNode internal_node(node);
            return internal_node.get_max_key();
        }
    }
}

Cursor *Node::find_key(Table *table, uint32_t page_num, uint32_t key){
    switch(get_type()){
        case NODE_LEAF:{
            LeafNode leaf_node(node);
            return leaf_node.find_key(table, page_num, key);
        }
        case NODE_INTERNAL:{
            InternalNode internal_node(node);
            return internal_node.find_key(table, key);
        }
    }
}

void Node::create_new_root(Table *table, uint32_t right_child_page_num) {
    // *root里的内容是原来root分裂后的左半部分
    void *root =table->pager->get_page(table->_root_page_num);
    uint32_t left_child_page_num = table->pager->get_unused_page_num();
    void *left_child = table->pager->get_page(left_child_page_num);
    // set left child
    memcpy(left_child, root, PAGE_SIZE);
    Node left_child_node(left_child);
    left_child_node.set_root(false);
    left_child_node.set_parent(table->_root_page_num);
    uint32_t left_child_max_key = left_child_node.get_max_key();
    // set right child
    void *right_child = table->pager->get_page(right_child_page_num);
    Node right_child_node(right_child);
    right_child_node.set_parent(table->_root_page_num);

    // set root node
    /* Root node is a new internal node with one key and two children */
    InternalNode root_node(root);
    root_node.set_type(NODE_INTERNAL);
    root_node.set_root(true);
    *(root_node.get_num_keys()) = 1;
    *(root_node.get_right_child()) = right_child_page_num;
    *(root_node.get_child(0)) = left_child_page_num;
    *(root_node.get_key(0)) = left_child_max_key;
}


uint32_t Node::get_parent() {
    void *parent_addr = (uint8_t *)node + PARENT_POINTER_OFFSET;
    return *(uint32_t *)parent_addr;
}

void Node::set_parent(uint32_t parent_page_num){
    void *parent_addr = (uint8_t *)node + PARENT_POINTER_OFFSET;
    *(uint32_t *)parent_addr = parent_page_num;
}
