//
// Created by syr on 2020/4/16.
//

#ifndef DB_DEMO_CURSOR_H
#define DB_DEMO_CURSOR_H

#include <cstdint>

class Table;


/**
 * @brief 负责指向一个Row, 类似于STL中的iterator
 */
class Cursor {
public:

    uint32_t page_num;
    uint32_t cell_num;
//    Pager *pager;
    Table *table;

    Cursor() {}

    Cursor(int page, int cell) : page_num(page), cell_num(cell) {}

    // returns a pointer to the position described by the cursor
    void *get_value();

    void advance();

    bool equal(const Cursor *other);

};

#endif //DB_DEMO_CURSOR_H
