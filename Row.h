//
// Created by syr on 2020/4/14.
//

#ifndef DB_DEMO_ROW_H
#define DB_DEMO_ROW_H

#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255


/**
 * @brief 负责row(就是tuple)的序列化和反序列化; 定义row序列化相关的一些常量
 */
class Row {
public:
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];

    // 将row序列化到destination
    void serialize(void *destination);

    // 将source的内容反序列化到row
    void deserialize(void *source);

    void show();

};

// define the compact representation of a row:
#define size_of_attribute(Class, Attribute)  sizeof(((Class *)0) -> Attribute)
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


#endif //DB_DEMO_ROW_H
