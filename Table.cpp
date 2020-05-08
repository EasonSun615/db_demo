//
// Created by syr on 2020/4/14.
//


#include <cstdlib>
#include <cstdio>
#include "Table.h"
#include "Pager.h"
#include "Row.h"
#include "LeafNode.h"
#include "InternalNode.h"


Table::Table(Pager *_pager):pager(_pager){
    _root_page_num = 0;
    if(pager->num_pages == 0){
        // database file is empty, Initialize page 0 as leaf node
        // 申请一块新的page
        void *root_page = pager->get_page(0);
        // 设置 num_cells = 0, 默认is_root = false
        LeafNode lf(root_page);
        lf.init();
        lf.set_root(true);
    }
}

Table::~Table() {
    // flushes the page cache to disk
    delete pager;
}


/**
 * @brief 获得执行第一个Row的Cursor
 * @return
 */
Cursor *Table::begin() {
    Cursor *cursor = find(0);  //assume 0 is the minimum possible key
    return cursor;
}

/**
 * @brief 获得指向最后一个Row的下一个位置的cursor
 * @return
 */
Cursor *Table::end() {
    Cursor *cursor = find(INT32_MAX);   // assume INT32_MAX is the maximum possible key
    return cursor;
}

/**
 * @brief find the position where the Row with this key should insert
 * @param key
 * @return
 */
Cursor *Table::find(uint32_t key){
    uint32_t root_page_num = _root_page_num;
    void *page = pager->get_page(root_page_num);
    Node node(page);
    if (node.get_type() == NODE_LEAF) {
        LeafNode leaf_node(page);
        return leaf_node.find_key(this, root_page_num, key);
    } else {
        InternalNode internal_node(page);
        return internal_node.find_key(this, key);
    }
}