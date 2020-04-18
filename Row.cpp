//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <stdio.h>
#include "Row.h"


void Row::serialize(void *destination) {
    memcpy((char *)destination + ID_OFFSET, &id, ID_SIZE);
    strncpy((char *)destination+USERNAME_OFFSET, username, USERNAME_SIZE);
    strncpy((char *)destination+EMAIL_OFFSET, email, EMAIL_SIZE);
}


void Row::deserialize(void *source) {
    memcpy(&id, (char *)source+ID_OFFSET, ID_SIZE);
    memcpy(username, (char *)source+USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(email, (char *)source+EMAIL_OFFSET, EMAIL_SIZE);
}


void Row::show(){
    printf("(id:%d, username:%s, email:%s)\n", id, username, email);
}