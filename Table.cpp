//
// Created by syr on 2020/4/14.
//


#include <cstdlib>
#include <cstdio>
#include "Table.h"
#include "Pager.h"
#include "Row.h"
#include "LeafNode.h"


Table::Table(Pager *_pager):pager(_pager){
    root_page_num = 0;
    if(pager->num_pages == 0){
        // database file is empty, Initialize page 0 as leaf node
        // 申请一块新的page
        void *root_page = pager->get_page(0);
        // 设置 num_cells = 0
        LeafNode(root_page).init();
    }
}

Table::~Table() {
    // flushes the page cache to disk
    delete pager;
}

// 返回第row_num个row在table中的地址
//void *Table::row_slot(uint32_t row_num) {
//    uint32_t page_num = row_num / ROWS_PER_PAGE;
//    void *page = pager->get_page(page_num);
//    uint32_t row_offset = row_num % ROWS_PER_PAGE;
//    uint32_t byte_offset = row_offset * ROW_SIZE;
//    return (char *) page + byte_offset;
//}


// returns a pointer to the position described by the cursor
//void *Table::cursor_value(Cursor *cursor) {
//    void *page = pager->get_page(cursor->page_num);
//    return LeafNode(page).get_value(cursor->cell_num);
//}

Cursor *Table::begin() {
    Cursor *cursor = new Cursor;
//    cursor->row_num = 0;
//    cursor->end_of_table = (pager->num_rows == 0);
    cursor->page_num = root_page_num;
    cursor->cell_num = 0;
    cursor->pager = pager;
    return cursor;
}

Cursor *Table::end() {
    Cursor *cursor = new Cursor;
//    cursor->row_num = pager->num_rows;
//    cursor->end_of_table = true;
    cursor->page_num = root_page_num;
    cursor->cell_num = *(uint32_t *)LeafNode(pager->get_page(cursor->page_num)).get_num_cells();
    cursor->pager = pager;
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