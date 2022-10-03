//
// Created by LONG on 2022/10/3.
//

#ifndef INC_100ASK_PROJECT1_PAGE_MANAGER_H
#define INC_100ASK_PROJECT1_PAGE_MANAGER_H

typedef struct Page {
    char *name;
    void (*run)(void *param);
    struct Page *next;
} Page;

void register_page(Page *pg);

void page_init(void);
Page *get_page(char *name);
#endif //INC_100ASK_PROJECT1_PAGE_MANAGER_H
