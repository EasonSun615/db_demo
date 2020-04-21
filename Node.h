//
// Created by syr on 2020/4/17.
//

#ifndef DB_DEMO_NODE_H
#define DB_DEMO_NODE_H

#include <stdint.h>


typedef enum{NODE_INTERNAL, NODE_LEAF} NodeType;

/*
 * Common Node Header Layout
 */
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);    // why is uint8_t ? enum size is 8bit?
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_OFFSET + NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint32_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

class Node{
public:
    void *node;
public:
    Node(void *n):node(n){}

    void set_type(NodeType node_type);
    NodeType get_type();
};

#endif //DB_DEMO_NODE_H
