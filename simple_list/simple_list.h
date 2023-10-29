//
// Created by Monster on 2023/6/16.
//

#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

/* Private macro -------------------------------------------------------------*/
#define LIST_NODE_MAX_NUM   64

/**
 * List struct
 */
typedef struct ListNode {
    void            *data;
    struct ListNode *next;
} ListNode_t;

/**
 * List params
 */
typedef struct {
    int        _size;
    ListNode_t *root;
    ListNode_t *last;

    // Helps "get" method, by saving last position
    ListNode_t *lastNodeGot;
    int        lastIndexGot;
    // isCached should be set to FALSE
    // everytime the list suffer changes
    bool isCached;
} List_t;

/**
 * @brief List definition
 */
typedef struct {
    List_t *list;
    int (*size)(List_t *list);
    bool (*add)(List_t *list, void *obj);
    bool (*unshift)(List_t *list, void *obj);
    bool (*set)(List_t *list, int index, void *obj);
    bool (*insert)(List_t *list, int index, void *obj);
    void *(*pop)(List_t *list);
    void *(*shift)(List_t *list);
    void *(*remove)(List_t *list, int index);
    void *(*get)(List_t *list, int index);
    void (*clear)(List_t *list);
} ListMethod_t;

bool listInit(List_t *list, ListMethod_t *method);

/**
 * Get list pointer.
 */
#define GetList(name) \
    &list_##name

/**
 * Get list method
 */
#define GetMethod(name) \
    &list_method_##name

/**
 * Define List
 */
#define SimpleListDef(name)                                 \
    List_t list_##name = { 0, NULL, NULL, NULL, 0, false }; \
    ListMethod_t list_method_##name;

/**
 * \brief List init
 */
#define SimpleListInit(name)                \
    listInit(GetList(name), GetMethod(name));

/**
 * \brief define list and initialize list
 * used inside a function
 */
#define SimpleList(name)                                    \
    List_t list_##name = { 0, NULL, NULL, NULL, 0, false }; \
    ListMethod_t list_method_##name;                        \
    listInit(GetList(name), GetMethod(name));

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LIST_H
