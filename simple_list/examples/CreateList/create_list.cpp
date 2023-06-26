//
// Created by moon on 2023/6/17.
//
#include <iostream>
#include "simple_list.h"

typedef struct {
    int a;
    int b;
    char *c;
} test;

using namespace std;

void setup() {
    // define list "a"
    simpleListDef(a);

    // init list "a"
    simpleListInit(a);

    // get list and list method
    List_t *list = simpleList(a);
    ListMethod_t *method = simpleListMethod(a);

    cout << "size : " << method->size(list) << endl;

    test t1 = {
            1,
            2,
            (char *) "t1"
    };

    test t2 = {
            3,
            4,
            (char *) "t2"
    };

    method->add(list, &t1);
    method->add(list, &t2);

    cout << "size : " << method->size(list) << endl;

    test *t3 = (test *) method->pop(list);

    cout << "a : " << t3->a << endl;
    cout << "b : " << t3->b << endl;
    cout << "c : " << t3->c << endl;
    cout << "size : " << method->size(list) << endl;

    method->remove(list, 0);
    cout << "size : " << method->size(list) << endl;
}