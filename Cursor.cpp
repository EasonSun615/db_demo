//
// Created by syr on 2020/4/18.
//

#include "Cursor.h"
#include "Table.h"
#include "LeafNode.h"


void *Cursor::get_value() {
    void *page = table->pager->get_page(page_num);
    return LeafNode(page).get_value(cell_num);
}

void Cursor::advance() {
    void *page = table->pager->get_page(page_num);
    LeafNode leaf_node(page);
    uint32_t num_cells = *(uint32_t *)leaf_node.get_num_cells();
    cell_num += 1;
    if(cell_num == num_cells && leaf_node.get_next_leaf()){
        page_num = leaf_node.get_next_leaf();
        cell_num = 0;
    }
}

bool Cursor::equal(const Cursor *other) {
    return table == other->table && page_num == other->page_num && cell_num == other->cell_num;
}