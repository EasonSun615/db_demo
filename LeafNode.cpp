//
// Created by syr on 2020/4/17.
//

#include "LeafNode.h"
#include "Cursor.h"
#include "Table.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


void *LeafNode::get_num_cells(){
    return (unsigned char *)node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *LeafNode::get_cell(uint32_t cell_num) {
    return (unsigned char *)node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

uint32_t *LeafNode::get_key(uint32_t cell_num) {
    return (uint32_t *)get_cell(cell_num);
}

void *LeafNode::get_value(uint32_t cell_num) {
    return (unsigned char*)get_cell(cell_num) + LEAF_NODE_KEY_SIZE;
}

void LeafNode::init() {
    *((uint32_t *)get_num_cells()) = 0;
    set_type(NODE_LEAF);
    set_root(false);
    set_next_leaf(0);  // 0 represents no sibling
}


void LeafNode::leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value){
    /*
     * Create a new node and move half the cells over.
     * Insert the new value in one of the two nodes.
     * Update parent or create a new parent.
     */
    void *old_page = cursor->table->pager->get_page(cursor->page_num);
    LeafNode old_node(old_page);
    uint32_t new_page_num = cursor->table->pager->get_unused_page_num();
    void *new_page = cursor->table->pager->get_page(new_page_num);
    LeafNode new_node(new_page);
    new_node.init();
    /*
     * All existing keys plus new key should be divided evenly between old (left) and new (right) nodes.
     * Starting from the right, move each key to correct position.
     * 想象把新的结点插入了原来的node,这个原来的node的索引就是[0-LEAF_NODE_MAX_CELLS], 然后我们分裂这个结点
     */
    for(int32_t i = LEAF_NODE_MAX_CELLS; i>=0; i--){
        LeafNode destination_node;
        if(i<LEAF_NODE_LEFT_SPLIT_COUNT)
            destination_node = old_node;
        else
            destination_node = new_node;
        // 获取在新结点中的索引
        uint32_t new_index = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void *destination = destination_node.get_cell(new_index);
        if(i<cursor->cell_num){    // 在新插入的结点之前
            memcpy(destination, old_node.get_cell(i), LEAF_NODE_CELL_SIZE);
        }else if(i == cursor->cell_num){
            memcpy(destination_node.get_key(new_index), &key, sizeof(key));
            value->serialize(destination_node.get_value(new_index));
        }else if(i > cursor->cell_num){
            memcpy(destination, old_node.get_cell(i-1), LEAF_NODE_CELL_SIZE);
        }
    }
    // update cell count of both leaf nodes
    *((uint32_t *)old_node.get_num_cells()) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *((uint32_t *)new_node.get_num_cells()) = LEAf_NODE_RIGHT_SPLIT_COUNT;
    new_node.set_next_leaf(old_node.get_next_leaf());
    old_node.set_next_leaf(new_page_num);

    if(old_node.is_root()){
        return create_new_root(cursor->table, new_page_num);
    }else{
        printf("Need to implement updating parent after split\n");
        exit(EXIT_FAILURE);
    }
}

void LeafNode::insert(Cursor *cursor, uint32_t key, Row *value) {
    uint32_t num_cells = *(uint32_t *)get_num_cells();
    if(num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        // Since We haven’t implemented splitting yet, so we error if the node is full.
//        printf("Need to implement splitting a leaf node.\n");
//        exit(EXIT_FAILURE);
        leaf_node_split_and_insert(cursor, key, value);
        return;
    }
    if(cursor->cell_num < num_cells){
        // Make room for new cell
        // note: int 和 uint_32比较，会将int隐式转换成uint_32，即只取后面32位
        for(int i=(int)num_cells-1; i>=(int)cursor->cell_num; i--){
            memcpy(get_cell(i+1), get_cell(i),LEAF_NODE_CELL_SIZE);
        }
    }
    *((uint32_t *)get_num_cells()) = num_cells + 1;
    *(uint32_t *)get_key(cursor->cell_num) = key;
    value->serialize(get_value(cursor->cell_num));
}

uint32_t LeafNode::get_max_key() {
    uint32_t num_cells = *(uint32_t *)get_num_cells();
    return *(uint32_t *)get_key(num_cells-1);
}

Cursor *LeafNode::leaf_node_find(Table *table, uint32_t page_num, uint32_t key) {
    void *page = table->pager->get_page(page_num);
    LeafNode ln(page);
    uint32_t num_cells = *(uint32_t *) ln.get_num_cells();
    Cursor *cursor = new Cursor(page_num, 0);
    cursor->table = table;
    // Binary search. 注意返回值的区间：[min_index, max_index]
    uint32_t min_index = 0;
    uint32_t max_index = num_cells;
    while (max_index > min_index) {
        uint32_t index = min_index + (max_index - min_index) / 2;
        uint32_t key_at_index = *ln.get_key(index);
        if (key_at_index == key) {
            cursor->cell_num = index;
            return cursor;
        } else if (key < key_at_index) {
            max_index = index;
        } else if (key > key_at_index) {
            min_index = index + 1;
        }
    }
    cursor->cell_num = min_index;
    cursor->page_num = page_num;
    return cursor;
}


uint32_t LeafNode::get_next_leaf() {
    void *next_leaf_ptr = (uint8_t *)node + LEAF_NODE_NEXT_LEAF_OFFSET;
    uint32_t next_leaf = *(uint32_t *)next_leaf_ptr;
    return next_leaf;
}

void LeafNode::set_next_leaf(uint32_t next_page_num) {
    void *next_leaf_ptr = (uint8_t *)node + LEAF_NODE_NEXT_LEAF_OFFSET;
    *(uint32_t *)next_leaf_ptr = next_page_num;
}
