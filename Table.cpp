//
// Created by syr on 2020/4/14.
//


#include <cstdlib>
#include <cstdio>
#include "Table.h"
#include "Pager.h"
#include "Row.h"


// db_open
Table::Table(const char *file_name) {
    pager = new Pager(file_name);
}

Table::~Table() {
    // flushes the page cache to disk
    delete pager;
}

// 返回第row_num个row在table中的地址
void *Table::row_slot(uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void *page = pager->get_page(page_num);
//    if (page == NULL) {
//        // 申请pager的pages内存
//        pager->pages[page_num] = malloc(PAGE_SIZE);
//        page = pager->pages[page_num];
//    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return (char *) page + byte_offset;
}