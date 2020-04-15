//
// Created by syr on 2020/4/15.
//

#ifndef DB_DEMO_PAGER_H
#define DB_DEMO_PAGER_H


#include "Row.h"
#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Pager{
public:
    int file_descriptor;
    uint32_t file_length;
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
    Pager(const char* file_name);
    ~Pager();
    void *get_page(uint32_t page_num);
    void page_flush(int page_num, size_t len);
};

#endif //DB_DEMO_PAGER_H
