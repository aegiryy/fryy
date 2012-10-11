#ifndef _LIST_H
#define _LIST_H

#define LIST_HEAD(type) \
    type * next;\
    type * prev;

typedef struct _list_t
{
    LIST_HEAD(struct _list_t);
} list_t;

#define CDLIST_ADD(list, elem)\
    if ((list) == 0)\
    {\
        (list) = (elem);\
        (list)->next = (list);\
        (list)->prev = (list);\
    }\
    else\
    {\
        (elem)->prev = (list)->prev;\
        (elem)->next = (list);\
        (list)->prev->next = (elem);\
        (list)->prev = (elem);\
    }

/* elem cannot be NULL */
#define CDLIST_REMOVE(elem)\
    if ((elem)->next == (elem))\
        (elem) = 0;\
    else\
    {\
        (elem)->prev->next = (elem)->next;\
        (elem)->next->prev = (elem)->prev;\
    }\

/* LSLIST uses *prev since *next is used for schedule */
#define LSLIST_ADD(list, elem)\
    if ((list) == 0)\
    {\
        (list) = (elem);\
        (list)->prev = 0;\
    }\
    else\
    {\
        (elem)->prev = (list);\
        (list) = (elem);\
    }

#endif
