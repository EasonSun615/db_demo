//
// Created by syr on 2020/4/15.
//


#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include "Pager.h"


Pager::Pager(const char *file_name) {
    // open仅仅是打开文件（存在一个表示文件的struct, open只是构造了一个struct)，文件的内容并没有读到内存中
    int fd = open(file_name,
                  O_RDWR |     //Read/Write mode
                  O_CREAT,  //  Create file if it does not exist
                  S_IWUSR |     // User write permission
                  S_IRUSR   // User read permission
    );
    if (fd == -1) {
        printf("Unable to open file.\n");
        exit(EXIT_FAILURE);
    }
    off_t file_length = lseek(fd, 0, SEEK_END);
    file_descriptor = fd;
    this->file_length = file_length;
    num_rows = file_length / ROW_SIZE;
    for (int i = 0; i < TABLE_MAX_PAGES; i++)
        pages[i] = NULL;
}

void Pager::page_flush(int page_num, size_t len) {
    if (pages[page_num] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }
    // 设置文件写指针
    off_t offset = lseek(file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    size_t bytes_witten = write(file_descriptor, pages[page_num], len);
    if (bytes_witten == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

Pager::~Pager() {
    //flushes the page cache to disk
    uint32_t num_full_pages = num_rows / ROWS_PER_PAGE;
    for (int i = 0; i < num_full_pages; i++) {
        // 将前num_full_pages个page写会磁盘
        if (pages[i] == NULL) {
            // 已经写会磁盘
            continue;
        }
        page_flush(i, PAGE_SIZE);
        free(pages[i]);  // 释放内存
        pages[i] = NULL;
    }
    // There may be a partial page to write to the end of the file
    // This should not be needed after we switch to a B-tree
    uint32_t num_additional_rows = num_rows % ROWS_PER_PAGE;
    if (num_additional_rows > 0) {
        uint32_t page_num = num_full_pages;
        if (pages[page_num] != NULL) {
            page_flush(page_num, num_additional_rows * ROW_SIZE);
            free(pages[page_num]);
            pages[page_num] = NULL;
        }
    }

    // 关闭db文件
    int result  = close(file_descriptor);
    if(result == -1){
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    // 貌似没啥用了
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        if(pages[i]){
            free(pages[i]);
            pages[i]=NULL;
        }
    }
}


void *Pager::get_page(uint32_t page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out of bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }
    if (pages[page_num] == NULL) {
        // Cache miss, Allocate memory and load from file
        void *page = malloc(PAGE_SIZE);

        // num_pages: 文件中总page数
        uint32_t num_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE)
            num_pages += 1;

        if (page_num <= num_pages) {
            // 说明要访问的page是已经在文件中的，所以将page从文件中复制到内存里
            lseek(file_descriptor, page_num * PAGE_SIZE, SEEK_SET);   // 设置文件的读指针
            uint32_t bytes_read = read(file_descriptor, page, PAGE_SIZE);  // 从文件fd中读PAGE_SIZE个字节到page指向的内存
            if (bytes_read == -1) {
                printf("Error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
        pages[page_num] = page;
    }
    return pages[page_num];
}