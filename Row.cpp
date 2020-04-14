//
// Created by syr on 2020/4/14.
//

#include <cstring>
#include <stdio.h>
#include "Row.h"


void Row::serialize(char *destination) {
    memcpy(destination + ID_OFFSET, &id, ID_SIZE);
    memcpy(destination+USERNAME_OFFSET, username, USERNAME_SIZE);
    memcpy(destination+EMAIL_OFFSET, email, EMAIL_SIZE);
}


void Row::deserialize(char *source) {
    memcpy(&id, source+ID_OFFSET, ID_SIZE);
    memcpy(username, source+USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(email, source+EMAIL_OFFSET, EMAIL_SIZE);
}


void Row::show(){
    printf("(id:%d, username:%s, email:%s)\n", id, username, email);
}