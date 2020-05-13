//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "Statement.h"
#include "InputBuffer.h"
#include "Table.h"
#include "Cursor.h"
#include "Row.h"
#include "LeafNode.h"
#include "InternalNode.h"


/**
 * @brief 对insert语句进行预处理，将buffer中的row反序列化到Row对象中
 * @param inputBuffer
 * @return
 */
PrepareResult Statement::prepare_insert(InputBuffer *inputBuffer) {
    _type = STATEMENT_INSERT;
    /// keyword， id_string, username， email四个字符串都是在buffer里（直接将buffer中的空格改成了'\0')
    char *keyword = strtok(inputBuffer->buffer, " ");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");
    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }
    int id = atoi(id_string);
    if (id < 0)
        return PREPARE_NEGTIVE_ID;
    if (strlen(username) > COLUMN_USERNAME_SIZE || strlen(email) > COLUMN_EMAIL_SIZE)
        return PREPARE_STRING_TOO_LONG;
    row_to_insert.id = id;
    strcpy(row_to_insert.username, username);
    strcpy(row_to_insert.email, email);
    return PREPARE_SUCESS;
}

/**
 * @brief 对语句进行预处理
 * @param inputBuffer
 * @return
 */
PrepareResult Statement::prepare(InputBuffer *inputBuffer) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        return prepare_insert(inputBuffer);
    } else if (strcmp(inputBuffer->buffer, "select") == 0) {
        _type = STATEMENT_SELECT;
        return PREPARE_SUCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}


/**
 * @brief 执行select语句，遍历叶子结点，输出所有的row
 * @param table
 * @return
 */
ExecuteResult Statement::execute_select(Table *table) {
    Row row;
    for (Cursor *cursor = table->begin(); !cursor->equal(table->end()); cursor->advance()) {
        void *source = cursor->get_value();
        row.deserialize(source);
        row.show();
    }
    return EXECUTE_SUCCESS;
}


/**
 * @brief 执行insert语句，插入一个row
 * @param table
 * @return
 */
ExecuteResult Statement::execute_insert(Table *table) {
    uint32_t key_to_insert = row_to_insert.id;
    // cursor记录新插入元素的位置
    Cursor *cursor = table->find(key_to_insert);
    void *_node = table->pager->get_page(cursor->page_num);
    LeafNode leaf_node(_node);
    uint32_t num_cells = *(uint32_t *)leaf_node.get_num_cells();
    if (cursor->cell_num < num_cells) {
        uint32_t key_at_index = *leaf_node.get_key(cursor->cell_num);
        if (key_at_index == key_to_insert)
            return EXECUTE_DUPLICATE_KEY;
    }
    leaf_node.insert(cursor, key_to_insert, &row_to_insert);
//    Cursor *cursor = table->end();
//    node.insert(cursor, row_to_insert.id, &row_to_insert);
    return EXECUTE_SUCCESS;
}


ExecuteResult Statement::execute(Table *table) {
    switch (_type) {
        case STATEMENT_SELECT:
            return execute_select(table);
        case STATEMENT_INSERT:
            return execute_insert(table);
    }
}