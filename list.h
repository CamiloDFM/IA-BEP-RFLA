#ifndef LISTH
#define LISTH

#include <stdlib.h>

typedef struct lnode {
	int value;
	struct lnode *next;
    struct lnode *prev;
} node;

typedef struct {
	node *first;
	node *last;
	int len;
} list;

// crea una lista nueva
list* createList();
// elimina una lista y sus contenidos, liberando la memoria
void deleteList(list*);
// agrega un elemento nuevo al final de la lista
void appendElement(list*, int);
// accede al elemento en la posicion indicada
int at(list*, int);
// borra el elemento indicado de la lista
void deleteElement(list*, int);
#endif
