#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char *value;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct {
	Node *head;
	Node *tail;
} StringList;

static Node *node_create(const char *value) {
	Node *node = malloc(sizeof(Node));

	size_t len = strlen(value) + 1;
	node ->value = malloc(len);
	memcpy(node->value, value, len);

	node->prev = NULL;
	node->next = NULL;
	return node;
}

static void list_insert(StringList *list, const char *value) {
	Node *node = node_create(value);

	if (list->tail == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
}

static Node *list_find(StringList *list, const char *value) {
	for (Node *node = list->head; node != NULL; node = node->next) {
		if(strcmp(node->value, value) == 0) {
			return node;
		}
	}
	return NULL;
}

static int list_delete(StringList *list, const char *value) {
	Node *node = list_find(list, value);
	if (node == NULL) {
		return 0;
	}

	if (node->prev != NULL) {
		node->prev->next = node->next;
	} else {
		list->head = node->next;
	}

	if (node->next != NULL) {
		node->next->prev = node->prev;
	} else {
		list->tail = node->prev;
	}

	free(node->value);
	free(node);
	return 1;
}

int main(void) {
	StringList list = {0};

	printf("Insert three strings:\n");
	list_insert(&list, "bacon");
	list_insert(&list, "eggs");
	list_insert(&list, "toast");
	for (Node *n = list.head; n != NULL; n = n->next) {
		printf("  %s\n", n->value);
	}

	printf("\nFind 'eggs': %s\n", list_find(&list, "eggs") ? "found" : "not found");
	printf("Find 'sausage': %s\n", list_find(&list, "sausage") ? "found" : "not found");

	printf("\nDelete 'eggs' (middle): %s\n", list_delete(&list, "eggs") ? "deleted" : "not found");
	printf("Delete 'bacon' (head): %s\n", list_delete(&list, "bacon") ? "deleted" : "not found");
	printf("Delete 'toast' (last one): %s\n", list_delete(&list, "toast") ? "deleted" : "not found");
	printf("Delete 'nothing' (already gone): %s\n", list_delete(&list, "nothing") ? "deleted" : "not found");

	printf("\nFinal list is %s\n", list.head == NULL ? "empty (correct)" : "NOT empty (bug!)");

	return 0;
}
