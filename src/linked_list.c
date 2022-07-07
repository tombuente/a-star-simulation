#include "linked_list.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a_star.h"

/**
 * @brief Allocate memory for a new list element.
 *
 * @return ListElement*
 */
ListElement* new_list_element();

ListElement* init_linked_list() {
	return new_list_element();
}

void destory_linked_list(ListElement* head) {
	ListElement* cur = head;

	while (head != NULL) {
		cur = head;
		head = head->next;
		free(cur);
	}
	return;
}

ListElement* new_list_element() {
	ListElement* list_element = malloc(sizeof(ListElement));
	list_element->data = NULL;
	list_element->next = NULL;

	return list_element;
}

ListElement* insert(ListElement* head, Node* node) {
	// Head is empty
	if (!head->data) {
		head->data = node;

		return head;
	}

	ListElement* new_node = new_list_element();
	new_node->data = node;
	new_node->next = head;

	return new_node;
}

ListElement* insert_sorted(ListElement* head, Node* node) {
	// Head is empty
	if (head->data == NULL) {
		head->data = node;
		return head;
	}

	ListElement* new_node = new_list_element();
	new_node->data = node;

	ListElement* cur = head;

	// Head is bigger, append in front
	if (cur->data->f > node->f) {
		head = new_node;
		new_node->next = cur;
		return head;
	}
	if (cur->data->f == node->f) {
		if (cur->data->h > node->h) {
			head = new_node;
			new_node->next = cur;
			return head;
		}
	}

	while (true) {
		if (cur->next) {
			if (cur->next->data->f > node->f) {
				ListElement* temp = cur->next;
				cur->next = new_node;
				new_node->next = temp;
				break;
			}
			if (cur->next->data->f == node->f) {
				if (cur->next->data->h > node->h) {
					ListElement* temp = cur->next;
					cur->next = new_node;
					new_node->next = temp;
					break;
				}
			}
			cur = cur->next;
		} else {
			cur->next = new_node;
			break;
		}
	}

	return head;
}

ListElement* remove_first(ListElement* head) {
	ListElement* new_head;
	if (!head->next) {
		new_head = new_list_element();
	} else {
		new_head = head->next;
	}
	free(head);
	return new_head;
}

ListElement* remove_node(ListElement* head, Node* to_remove) {
	if (head->data == to_remove) {
		return remove_first(head);
	}

	ListElement* cur = head;

	do {
		if (cur->next->data == to_remove) {
			ListElement* temp_to_remove = cur->next;
			cur->next = cur->next->next;
			free(temp_to_remove);
			break;
		}
		cur = cur->next;
	} while (cur);

	return head;
}

bool contains(ListElement* head, Node* to_find) {
	ListElement* cur = head;

	do {
		if (cur->data == to_find) {
			return true;
		}
		cur = cur->next;
	} while (cur);
	return false;
}

bool is_empty(ListElement* head) {
	if (head->data == NULL) {
		return true;
	}

	return false;
}