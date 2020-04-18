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
    uint32_t root_page_num;

    Table(Pager *p);

    ~Table();

    Cursor *begin();

    Cursor *end();

};

#endif //DB_DEMO_TABLE_H
