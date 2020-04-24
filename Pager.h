//
// Created by syr on 2020/4/15.
//

#ifndef DB_DEMO_PAGER_H
#define DB_DEMO_PAGER_H


#include "Row.h"
#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
//const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
//const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

/*
 * Pager的操作都是以page为单位
 * 1. 负责根据page_num获得page在内存中的地址，如果page不在内存中的话，负责从文件中read
 * 2. 负责将page flush到磁盘上
 */
class Pager{
public:
    int file_descriptor;
    uint32_t file_length;
//    uint32_t num_rows;
    uint32_t num_pages;
    void *pages[TABLE_MAX_PAGES];
    Pager(const char* file_name);
    ~Pager();
    void *get_page(uint32_t page_num);
    void page_flush(int page_num);
    uint32_t get_unused_page_num();
};

#endif //DB_DEMO_PAGER_H
