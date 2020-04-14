//
// Created by syr on 2020/4/14.
//


#include <cstdlib>
#include <cstdio>
#include "Table.h"
#include "Row.h"


Table::Table(){
    num_rows = 0;
    for(int i=0; i<TABLE_MAX_PAGES; i++)
        pages[i] = NULL;
}

Table::~Table(){
    for(int i=0; i<TABLE_MAX_PAGES; i++)
        // 释放page内存
        free(pages[i]);
}

// 返回第row_num个row在table中的地址
void *Table::row_slot(uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void *page = pages[page_num];
    if (page == NULL) {
        // 申请page内存
        pages[page_num] = malloc(PAGE_SIZE);
        page = pages[page_num];
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return (char *)page + byte_offset;
}