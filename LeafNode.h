//
// Created by syr on 2020/4/17.
//

#ifndef DB_DEMO_LEAFNODE_H
#define DB_DEMO_LEAFNODE_H

#include <cstdlib>
#include "Node.h"
#include "Row.h"
#include "Pager.h"

class Cursor;

/*
 * Leaf node header layout
 */
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;

//Update the leaf node header format to include the new field
const uint32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET = LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE + LEAF_NODE_NEXT_LEAF_SIZE;

/*
 * Lead node body layout
 */
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

//during a split we need to distribute N+1 cells between two nodes (N original cells plus one new one).
//arbitrarily choosing the left node to get one more cell if N+1 is odd.
const uint32_t LEAf_NODE_RIGHT_SPLIT_COUNT =( LEAF_NODE_MAX_CELLS + 1)/ 2;
const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT = LEAF_NODE_MAX_CELLS + 1 - LEAf_NODE_RIGHT_SPLIT_COUNT;


/**
 * @brief 叶子结点
 */
class LeafNode : public Node {
public:
    LeafNode(void *n= NULL):Node(n){}
    LeafNode(Node &_node):Node(_node.node){}
    LeafNode &operator=(const LeafNode &other){node = other.node;  return *this;}
    void *get_num_cells();
    void *get_cell(uint32_t cell_num);
    uint32_t *get_key(uint32_t cell_num);
    void *get_value(uint32_t cell_num);
    void init();
    void insert(Cursor *cursor, uint32_t key, Row *value);
    void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value);
    uint32_t get_max_key();
//    static Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);
    Cursor *find_key(Table *table,uint32_t page_num, uint32_t key);
    uint32_t get_next_leaf();
    void set_next_leaf(uint32_t next_page_num);
};

#endif //DB_DEMO_LEAFNODE_H
