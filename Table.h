//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_TABLE_H
#define DB_DEMO_TABLE_H

#include <stdint.h>
#include "Row.h"
#include "Pager.h"
#include "Cursor.h"

class Table {
public:
    Pager *pager;
    uint32_t _root_page_num;

    Table(Pager *p);

    ~Table();

    Cursor *begin();

    Cursor *end();

    Cursor *find(uint32_t key);

};

#endif //DB_DEMO_TABLE_H
