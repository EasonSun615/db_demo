//
// Created by syr on 2020/4/20.
//


#include "Node.h"

void Node::set_type(NodeType node_type) {
    uint8_t value = node_type;
    *(uint8_t *)((unsigned char *)node + NODE_TYPE_OFFSET) = value;
}

NodeType Node::get_type() {
    uint8_t node_type= *(uint8_t *)((unsigned char *)node + NODE_TYPE_OFFSET);
    return (NodeType)node_type;
}
