/*
* Milk is a standalone module for various "generic" collections.
*
* All add functions defined by Milk will perform a shallow copy of the passed value.
* All get value functions defined by Milk will return a pointer to the internal value being stored, thus edits to that value directly
* will be reflected in the internal structure. 
* These values should NOT be freed.
* All to_array functions defined by Milk will return a shallow copy of the internal values, however. 
* These arrays SHOULD be freed manually.
* 
* */

#ifndef MILK_H

#include <slyertype.h>

#ifndef MILK_INTERNAL

typedef void LinkedList;
typedef void LinkedListNode;

#endif

/// @brief Creates a new empty linked list.
/// @param elementSize The size of each element passed via add.
/// @return A reference to the created linked list.
LinkedList* 	milk_linked_list_new(size_t elementSize);

/// @brief Prepends the given element to the start of the linked list.
/// @param list The linked list we're adding this element to.
/// @param element The new element we're saving in this linked list.
void 			milk_linked_list_add_first(LinkedList* list, void* element);

/// @brief Free this linked list, all its nodes and internal values.
/// @param list The list we wish to free.
void 			milk_linked_list_free(LinkedList* list);

/// @brief Returns the number of elements in the passed linked list.
/// @param list The list we wish to query.
/// @return The number of elements this list contains.
uint 			milk_linked_list_count(LinkedList* list);

/// @brief Converts the given linked list to an array with milk_linked_list_count(list) elements.
/// @param list The list we wish to convert to an array.
/// @return A pointer to the 0th element of the array.
void* 			milk_linked_list_to_array(LinkedList* list);


/// @brief Begins iteration over given linked list. Returns NULL if this LinkedList is empty.
/// @param list The list we wish to iterate over.
/// @return The 0th element's LinkedListNode.
LinkedListNode* milk_linked_list_iterate(LinkedList* list);

/// @brief Retrieves value from current linked list node.
/// @param node The node we wish to retrieve a value from.
/// @return The value this node contains.
void* 			milk_linked_list_node_value(LinkedListNode* node);

/// @brief Iterates to next node in linked list. Returns NULL if node was the last node.
/// @param node The current node.
/// @return The next node in the iteration.
LinkedListNode* milk_linked_list_node_next(LinkedListNode* node);


#endif