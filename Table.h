//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_TABLE_H
#define DB_DEMO_TABLE_H

#include <stdint.h>
#include "Row.h"
#include "Pager.h"

//#define TABLE_MAX_PAGES 100
//const uint32_t PAGE_SIZE = 4096;
//const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
//const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table {
public:
    uint32_t num_rows;
//    void * pages[TABLE_MAX_PAGES];
    Pager *pager;

    Table(const char *file_name);

    ~Table();

    // 返回第row_num个row在内存中的地址
    void *row_slot(uint32_t row_num);

    void set_num_rows(uint32_t num){
        pager->num_rows = num;
        num_rows = num;
    }

};

#endif //DB_DEMO_TABLE_H
