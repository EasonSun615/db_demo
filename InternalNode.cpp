//
// Created by syr on 2020/4/17.
//


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "InternalNode.h"
#include "Table.h"
#include "LeafNode.h"

uint32_t *InternalNode::get_num_keys() {
    return (uint32_t *) ((uint8_t *) node + INTERNAL_NODE_NUM_KEYS_OFFSET);
}

uint32_t *InternalNode::get_right_child() {
    return (uint32_t *) ((uint8_t *) node + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}


void *InternalNode::get_cell(uint32_t cell_num) {
    return (uint8_t *) node + INTERNAL_NODE_HEADER_SIZE + INTERNAL_NODE_CELL_SIZE * cell_num;
}

uint32_t *InternalNode::get_child(uint32_t child_num) {
    uint32_t num_keys = *get_num_keys();
    if (child_num > num_keys) {
        printf("Tried to access child_num %d > num_keys %d\n", child_num, num_keys);
        exit(EXIT_FAILURE);
    } else if (child_num == num_keys) {
        return get_right_child();
    } else {
        return (uint32_t *) get_cell(child_num);
    }
}

uint32_t *InternalNode::get_key(uint32_t key_num) {
    return (uint32_t *) ((uint8_t *) get_child(key_num) + INTERNAL_NODE_CHILD_SIZE);
}

uint32_t InternalNode::get_max_key() {
    uint32_t num_keys = *get_num_keys();
    return *get_key(num_keys - 1);
}

void InternalNode::init() {
    *((uint32_t *) get_num_keys()) = 0;
    set_type(NODE_INTERNAL);
    set_root(false);
}

/**
 * @brief Return the index of the child which should contain the given key
 * @param key
 * @return
 */
uint32_t InternalNode::find_child(uint32_t key) {
    uint32_t num_keys = *get_num_keys();
    uint32_t min_index = 0;
    uint32_t max_index = num_keys;
    // [0, num_keys]
    while (min_index < max_index) {
        uint32_t index = min_index + (max_index - min_index) / 2;
        uint32_t key_to_right = *get_key(index);
        if (key_to_right < key) {
            min_index = index + 1;
        } else if (key_to_right == key) {
            min_index = max_index = index;
        } else if (key_to_right > key) {
            max_index = index;
        }
    }
    return min_index;
}


/**
 * @brief find the position of the key in leaf node
 * @param table
 * @param page_num
 * @param key
 * @return
 */
//Cursor *InternalNode::internal_node_find(Table *table, uint32_t page_num, uint32_t key) {
//    void *page = table->pager->get_page(page_num);
//    InternalNode internal_node(page);
//    uint32_t child_index = internal_node_find_child(table, page_num, key);
//    uint32_t child_page_num = *internal_node.get_child(child_index);
//    Node child_node(table->pager->get_page(child_page_num));
//    switch (child_node.get_type()) {
//        case NODE_INTERNAL:
//            return internal_node_find(table, child_page_num, key);
//        case NODE_LEAF:
//            return LeafNode::leaf_node_find(table, child_page_num, key);
//    }
//}


/**
 * @brief recursive to find the key in leaf_node
 * @param table
 * @param page_num
 * @param key
 * @return
 */
Cursor *InternalNode::find_key(Table *table, uint32_t key) {
    uint32_t child_index = find_child(key);
    uint32_t child_page_num = *get_child(child_index);
    Node child_node(table->pager->get_page(child_page_num));
    return child_node.find_key(table, child_page_num, key);
}




//void update_key(uint32_t old_key, uint32_t new_key);
//void insert(uint32_t page_num, Table *table);

void InternalNode::update_key(uint32_t old_key, uint32_t new_key) {
    uint32_t index = find_child(old_key);
    if(index==*get_num_keys()){
        /// 当最右叶子结点分裂时，需要在update_key中插入key, 在insert中只插入最右child即可
        /// 需要通过父节点的最后一个key来找到新插入的key的位置
        uint32_t *key_before = get_key(*get_num_keys()-1);
        *(uint32_t *)((uint8_t *)key_before+ INTERNAL_NODE_CELL_SIZE) = new_key;
    }else {
        *get_key(index) = new_key;
    }
}


/**
 * @brief Add a new child/key pair to this internal_node of parent_page
 * @param child_page_num
 * @param table
 */
void InternalNode::insert(uint32_t child_page_num, Table *table, bool right_most) {
    /// first step: get max key of child node
    void *page = table->pager->get_page(child_page_num);
    Node child_node(page);
    uint32_t child_max_key = child_node.get_max_key();
    /// second step: find the position that the new key should be inserted
    uint32_t index = find_child(child_max_key);
    uint32_t original_num_keys = *get_num_keys();
    *get_num_keys() = original_num_keys + 1;
    if (original_num_keys + 1 >= INTERNAL_NODE_MAX_CELLS) {
        printf("Need to implement splitting internal node\n");
        exit(EXIT_FAILURE);
    }
    /// third step: insert the key
    uint32_t right_child_page_num = *get_right_child();
    if (right_most) {
        /** Replace right child */
        *get_child(original_num_keys) = right_child_page_num;
        *get_right_child() = child_page_num;
    } else {
        /** Make room for the new cell */
        for (int32_t i = original_num_keys; i > index; i--) {
            void *destination = get_cell(i);
            void *source = get_cell(i - 1);
            memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);
        }
        *get_child(index) = child_page_num;
        *get_key(index) = child_max_key;
    }
}

