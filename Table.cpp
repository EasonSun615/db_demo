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


Cursor *Table::begin() {
    Cursor *cursor = find(0);  //assume 0 is the minimum possible key
    return cursor;
}

Cursor *Table::end() {
    Cursor *cursor = find(INT32_MAX);   // assume INT32_MAX is the maximum possible key
    return cursor;
}

// 当cursor已经指向最后一个row的下个位置时，返回false
//bool Table::cursor_advance(Cursor *cursor) {
//    if(cursor->end_of_table)
//        return false;
//    cursor->row_num++;
//    if(cursor->row_num == end()->row_num)
//        cursor->end_of_table = true;
//    return true;
//}


// 找到key要插入的位置
Cursor *Table::find(uint32_t key){
    uint32_t root_page_num = _root_page_num;
    void *page = pager->get_page(root_page_num);
    Node node(page);
    if (node.get_type() == NODE_LEAF) {
        return LeafNode::leaf_node_find(this, root_page_num, key);
    } else {
        return InternalNode::internal_node_find(this, root_page_num, key);
    }
}