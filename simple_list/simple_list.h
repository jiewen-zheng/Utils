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
    void *data;
    struct ListNode *next;
} ListNode_t;

/**
 * List params
 */
typedef struct {
    int _size;
    ListNode_t *root;
    ListNode_t *last;

    // Helps "get" method, by saving last position
    ListNode_t *lastNodeGot;
    int lastIndexGot;
    // isCached should be set to FALSE
    // everytime the list suffer changes
    bool isCached;
} List_t;

/**
 * @brief List definition
 */
typedef struct {
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
 * Define list
 */
/**
 * Define List
 */
#define SimpleListDef(name) \
List_t list_##name = { 0, NULL, NULL, NULL, 0, false }; \
ListMethod_t list_method_##name;

/**
 * Get list pointer.
 */
#define SimpleList(name) \
&list_##name

/**
 * Get list method
 */
#define SimpleListMethod(name) \
&list_method_##name

/**
 * List init
 */
#define SimpleListInit(name) \
do {                         \
listInit(SimpleList(name), SimpleListMethod(name)); \
} while(0)


#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LIST_H
