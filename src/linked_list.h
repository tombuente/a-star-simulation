#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "a_star.h"

typedef struct ListElement {
	Node* data;
	struct ListElement* next;
} ListElement;

ListElement* init_linked_list();
void destory_linked_list(ListElement* head);

/**
 * @brief Inserts a new Node.
 *
 * @param head
 * @param node
 * @return ListElement*
 */
ListElement* insert(ListElement* head, Node* node);

/**
 * @brief Inserts a new Node into the list, using the f-cost and h-cost to determine its position.
 *
 * @param head
 * @param node
 * @return ListElement*
 */
ListElement* insert_sorted(ListElement* head, Node* node);

/**
 * @brief Removes the first Node.
 *
 * @param head
 * @return ListElement*
 */
ListElement* remove_first(ListElement* head);

ListElement* remove_node(ListElement* head, Node* to_remove);

bool contains(ListElement* head, Node* to_find);

bool is_empty(ListElement* head);

#endif	// LINKED_LIST_H