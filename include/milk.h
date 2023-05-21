#ifndef MILK_H

#include <slyertype.h>

#ifndef MILK_INTERNAL

typedef void LinkedList;
typedef void LinkedListNode;

#endif

LinkedList* 	milk_linked_list_new(size_t elementSize);
void 			milk_linked_list_add_first(LinkedList* list, void* element);
void 			milk_linked_list_free(LinkedList* list);
uint 			milk_linked_list_count(LinkedList* list);
void* 			milk_linked_list_to_array(LinkedList* list);

LinkedListNode* milk_linked_list_iterate(LinkedList* list);
void* 			milk_linked_list_node_value(LinkedListNode* node);
LinkedListNode* milk_linked_list_node_next(LinkedListNode* node);


#endif