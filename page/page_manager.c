//
// Created by LONG on 2022/10/3.
//
#include <stdio.h>
#include <string.h>
#include "../include/page_manager.h"

static Page *pageHead = NULL;

extern void main_page_register(void);

void register_page(Page *pg)
{
    pg->next = pageHead;
    pageHead = pg;
}

void page_init(void)
{
    main_page_register();
}

Page *get_page(char *name)
{
    Page *pg = pageHead;
    while (pg) {
        if (strcmp(name, pg->name) == 0) {
            return pg;
        }
        pg = pg->next;
    }
    return NULL;
}





