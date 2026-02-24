
// Generated on 1min.ai with Claude 4.5 Haiku - Anthropic

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEVEL 16
#define PROBABILITY 0.5

typedef struct Node {
    int key;
    int value;
    struct Node **forward;
    int level;
} Node;

typedef struct SkipList {
    Node *header;
    int level;
} SkipList;

// Generate random level for new node
int randomLevel() {
    int level = 1;
    while ((rand() / (double)RAND_MAX) < PROBABILITY && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

// Allocate a new node
Node* allocateNode(int level, int key, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for node\n");
        return NULL;
    }

    node->forward = (Node **)malloc(sizeof(Node *) * (level + 1));
    if (!node->forward) {
        fprintf(stderr, "Memory allocation failed for forward pointers\n");
        free(node);
        return NULL;
    }

    node->key = key;
    node->value = value;
    node->level = level;

    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }

    return node;
}

// Free a node
void freeNode(Node *node) {
    if (node) {
        if (node->forward) {
            free(node->forward);
        }
        free(node);
    }
}

// Initialize skip list
SkipList* initSkipList() {
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    if (!list) {
        fprintf(stderr, "Memory allocation failed for skip list\n");
        return NULL;
    }

    list->level = 1;
    list->header = allocateNode(MAX_LEVEL, -1, -1);
    if (!list->header) {
        free(list);
        return NULL;
    }

    return list;
}

// Insert a key-value pair
int insert(SkipList *list, int key, int value) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    // Find position to insert
    for (int i = list->level; i >= 1; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[1];

    // Check if key already exists
    if (current && current->key == key) {
        current->value = value;
        return 0; // Update existing value
    }

    // Generate random level for new node
    int newLevel = randomLevel();

    // Update skip list level if necessary
    if (newLevel > list->level) {
        for (int i = list->level + 1; i <= newLevel; i++) {
            update[i] = list->header;
        }
        list->level = newLevel;
    }

    // Create and insert new node
    Node *newNode = allocateNode(newLevel, key, value);
    if (!newNode) {
        return -1;
    }

    for (int i = 1; i <= newLevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    return 1; // New node inserted
}

// Search for a key
Node* search(SkipList *list, int key) {
    Node *current = list->header;

    for (int i = list->level; i >= 1; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[1];

    if (current && current->key == key) {
        return current;
    }

    return NULL;
}

// Remove a node with given key
int remove_node(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = list->header;

    // Find position of node to remove
    for (int i = list->level; i >= 1; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[1];

    // Key not found
    if (!current || current->key != key) {
        return 0;
    }

    // Remove node from all levels
    for (int i = 1; i <= list->level; i++) {
        if (update[i]->forward[i] == current) {
            update[i]->forward[i] = current->forward[i];
        }
    }

    freeNode(current);

    // Update skip list level if necessary
    while (list->level > 1 && !list->header->forward[list->level]) {
        list->level--;
    }

    return 1;
}

// Display skip list (for debugging)
void display(SkipList *list) {
    printf("\nSkip List (Level: %d):\n", list->level);

    for (int i = list->level; i >= 1; i--) {
        printf("Level %d: ", i);
        Node *node = list->header->forward[i];
        while (node) {
            printf("[%d:%d] -> ", node->key, node->value);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
    printf("\n");
}

// Free entire skip list
void freeSkipList(SkipList *list) {
    if (!list) return;

    Node *current = list->header->forward[1];
    while (current) {
        Node *next = current->forward[1];
        freeNode(current);
        current = next;
    }

    freeNode(list->header);
    free(list);
}

// Example usage
int main() {
    srand(time(NULL));

    SkipList *skiplist = initSkipList();
    if (!skiplist) {
        return 1;
    }

    printf("Inserting elements...\n");
    insert(skiplist, 3, 30);
    insert(skiplist, 6, 60);
    insert(skiplist, 7, 70);
    insert(skiplist, 9, 90);
    insert(skiplist, 12, 120);
    insert(skiplist, 15, 150);
    insert(skiplist, 18, 180);

    display(skiplist);

    printf("Searching for key 6: ");
    Node *found = search(skiplist, 6);
    if (found) {
        printf("Found - Value: %d\n", found->value);
    } else {
        printf("Not found\n");
    }

    printf("\nRemoving key 6...\n");
    remove_node(skiplist, 6);
    display(skiplist);

    printf("Searching for key 6 after removal: ");
    found = search(skiplist, 6);
    if (found) {
        printf("Found - Value: %d\n", found->value);
    } else {
        printf("Not found\n");
    }

    freeSkipList(skiplist);

    return 0;
}
