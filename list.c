#include "list.h"
#include <stdio.h>

list *createList(){
    list *nueva = (list*)malloc(sizeof(list));
    nueva->first = NULL;
    nueva->last = NULL;
    nueva->len = 0;

    return nueva;
}

void deleteList(list* l){
    node *actual, *siguiente;
    actual = l -> first;
    while (actual) {
        siguiente = actual -> next;
        free(actual);
        actual = siguiente;
    }
    free(l);
}

void appendElement(list *l, int e){
    node *nuevo = (node*)malloc(sizeof(node));
    nuevo -> value = e;
    nuevo -> next = NULL;
    if (!l -> first){
        l -> first = nuevo;
        nuevo -> prev = NULL;
    }
    else{
        l -> last -> next = nuevo;
        nuevo -> prev = l -> last;
    }
    l -> last = nuevo;
    l -> len++;
}

int at(list *l, int i){
    node *actual;
    int j = 0;
    for (actual = l -> first; actual != NULL; actual = actual -> next){
        if (i == j){
            return actual->value;
        }
        j++;
    }
    return -1;
}

void deleteElement(list *l, int i){
    if ((l -> len == 0) || (l -> len < i + 1)){
        return;
    }
    node *actual;
    int j = 0;
    for (actual = l -> first; actual != NULL; actual = actual -> next){
        if (i == j){
            if (actual == l -> first){
                l -> first = actual -> next;
            }
            else {
                actual -> prev -> next = actual -> next;
            }
            if (actual == l -> last){
                l -> last = actual -> prev;
            }
            else {
                actual -> next -> prev = actual -> prev;
            }
            free(actual);
            l -> len -= 1;
            return;
        }
        j++;
    }
    return;
}