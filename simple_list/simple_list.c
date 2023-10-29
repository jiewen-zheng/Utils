//
// Created by Monster on 2023/6/16.
//

#include "simple_list.h"
#include "stdlib.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static ListNode_t *getNode(List_t *list, int index) {
    int _pos = 0;

    ListNode_t *current = list->root;
    // Check if the node trying to get is
    // immediatly AFTER the previous got one
    if (list->isCached && list->lastIndexGot <= index) {
        _pos = list->lastIndexGot;
        current = list->lastNodeGot;
    }

    while (_pos < index && current) {
        current = current->next;

        _pos++;
    }

    // Check if the object index got is the same as the required
    if (_pos == index) {
        list->isCached = true;
        list->lastIndexGot = index;
        list->lastNodeGot = current;

        return current;
    }

    return NULL;
}

/**
 * Returns current size of LinkedList
 */
int size(List_t *list) {
    return list->_size;
}

/**
 * Adds a object in the end of the LinkedList
 * - Increment _size;
 * @param obj
 * @return obj add result, true of false.
 */
static bool add(List_t *list, void *obj) {
    // Exceed maximum quantity
    if (list->_size > LIST_NODE_MAX_NUM) {
        return false;
    }

    ListNode_t *tmp = (ListNode_t *) malloc(sizeof(ListNode_t));
    if (!tmp) {
        return false;
    }

    tmp->data = obj;
    tmp->next = NULL;

    if (list->root) {
        // Already have elements inserted
        list->last->next = tmp;
        list->last = tmp;
    } else {
        // First element being inserted
        list->root = tmp;
        list->last = tmp;
    }

    list->_size++;
    list->isCached = false;

    return true;
}

/**
 * Adds a object in the start of the LinkedList
 * - Increment _size
 * @param obj
 * @return
 */
static bool unshift(List_t *list, void *obj) {
    // Exceed maximum quantity
    if (list->_size > LIST_NODE_MAX_NUM) {
        return false;
    }

    if (list->_size == 0)
        return add(list, obj);

    ListNode_t *tmp = (ListNode_t *) malloc(sizeof(ListNode_t));
    if (!tmp) {
        return false;
    }

    tmp->next = list->root;
    tmp->data = obj;
    list->root = tmp;

    list->_size++;
    list->isCached = false;

    return true;
}

/**
 * Set the object at index
 * @param list
 * @param index
 * @param obj
 * @return
 */
static bool set(List_t *list, int index, void *obj) {
    // Check if index position is in bounds
    if (index < 0 || index >= list->_size)
        return false;

    getNode(list, index)->data = obj;
    return true;
}

/**
 * Insert the object at index
 * @param list
 * @param index
 * @param obj
 * @return
 */
static bool insert(List_t *list, int index, void *obj) {
    // Exceed maximum quantity
    if (list->_size > LIST_NODE_MAX_NUM) {
        return false;
    }

    // Check if index position is in bounds
    if (index < 0 || index >= list->_size)
        return false;

    if (index == 0 || list->_size <= 1) {
        return unshift(list, obj);
    } else if (index == list->_size - 1) {
        return add(list, obj);
    } else {
        // Already have elements inserted
        ListNode_t *tmp = (ListNode_t *) malloc(sizeof(ListNode_t));
        if (!tmp) {
            return false;
        }

        ListNode_t *_last = getNode(list, index - 1);
        ListNode_t *_next = _last->next;

        tmp->data = obj;
        tmp->next = _next;
        _last->next = tmp;
    }

    list->_size++;
    list->isCached = false;
    return true;
}

/**
 * Remove last object
 * @return
 */
static void *pop(List_t *list) {

    if (list->_size <= 0) {
        return NULL;
    }

    if (list->_size >= 2) {
        ListNode_t *tmp = getNode(list, list->_size - 2); // Penultimate
        void *ret = tmp->next->data;
        free(tmp->next);
        tmp->next = NULL;
        list->last = tmp;
        list->_size--;
        return ret;
    } else {
        // Only one element left on the list
        void *ret = list->root->data;
        free(list->root);
        list->root = NULL;
        list->last = NULL;
        list->_size = 0;
        return ret;
    }
}

/**
 * Remove first object
 * @return
 */
static void *shift(List_t *list) {
    if (list->_size <= 0) {
        return NULL;
    }

    if (list->_size > 1) {
        ListNode_t *_next = list->root->next;
        void *ret = list->root->data;
        free(list->root);
        list->root = _next;
        list->_size--;
        list->isCached = false;

        return ret;
    } else {
        // Only one left, then pop()
        return pop(list);
    }
}

/**
 * Remove object at index
 * If index is not reachable, returns false, else decrement _size
 * @param index
 * @return Delete data on a node
 */
static void *remove(List_t *list, int index) {

    if (index < 0 || index >= list->_size) {
        return NULL;
    }

    if (index == 0) {
        return shift(list);
    }

    if (index == list->_size - 1) {
        return pop(list);
    }

    ListNode_t *tmp = getNode(list, index - 1);
    ListNode_t *toDelete = tmp->next;
    void *ret = toDelete->data;
    tmp->next = toDelete->next;

    // release none memory
    free(toDelete);

    list->_size--;
    list->isCached = false;
    return ret;
}

/**
 * Get the index element on the list
 * @param index
 * @return accessible Element or false
 */
static void *get(List_t *list, int index) {
    ListNode_t *tmp = getNode(list, index);

    return (tmp ? tmp->data : NULL);
}

/**
 * Clear the entire array
 */
static void clear(List_t *list) {
    while (size(list) > 0)
        shift(list);
}

/**
 * Init list params
 */
bool listInit(List_t *list, ListMethod_t *method) {
    if (!list || !method) {
        return false;
    }

    list->_size = 0;
    list->root = NULL;
    list->last = NULL;
    list->lastNodeGot = NULL;
    list->lastIndexGot = 0;
    list->isCached = false;

    method->size = size;
    method->add = add;
    method->set = set;
    method->insert = insert;
    method->unshift = unshift;
    method->pop = pop;
    method->shift = shift;
    method->remove = remove;
    method->get = get;
    method->clear = clear;

    return true;
}
