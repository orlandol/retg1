
// Generated on 1min.ai with Claude 4.5 Haiku - Anthropic

#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"

/* Example data structure that contains an AVL tree node */
struct my_data {
    int value;
    struct avl_tree_node node;
};

/* Comparison function for AVL tree operations */
static int compare_nodes(const struct avl_tree_node *node1,
                         const struct avl_tree_node *node2)
{
    struct my_data *data1 = avl_tree_entry(node1, struct my_data, node);
    struct my_data *data2 = avl_tree_entry(node2, struct my_data, node);
    
    if (data1->value < data2->value)
        return -1;
    else if (data1->value > data2->value)
        return 1;
    else
        return 0;
}

/* Allocate and insert a new node into the tree */
struct my_data *avl_insert_new_node(struct avl_tree_node **root, int value)
{
    struct my_data *new_data;
    struct avl_tree_node *existing;
    
    /* Allocate memory for new node using calloc */
    new_data = calloc(1, sizeof(struct my_data));
    if (!new_data)
        return NULL;
    
    /* Initialize the data */
    new_data->value = value;
    
    /* Mark the node as unlinked before insertion */
    avl_tree_node_set_unlinked(&new_data->node);
    
    /* Insert into the tree */
    existing = avl_tree_insert(root, &new_data->node, compare_nodes);
    
    /* If a duplicate was found, free the new node and return the existing one */
    if (existing) {
        free(new_data);
        return avl_tree_entry(existing, struct my_data, node);
    }
    
    return new_data;
}

/* Remove and free a node from the tree */
void avl_remove_and_free_node(struct avl_tree_node **root, struct my_data *data)
{
    if (!data)
        return;
    
    /* Remove the node from the tree (rebalances as needed) */
    avl_tree_remove(root, &data->node);
    
    /* Mark node as unlinked */
    avl_tree_node_set_unlinked(&data->node);
    
    /* Free the allocated memory */
    free(data);
}

/* Example usage */
int main(void)
{
    struct avl_tree_node *root = NULL;
    struct my_data *node1, *node2, *node3;
    
    /* Insert some nodes */
    node1 = avl_insert_new_node(&root, 10);
    node2 = avl_insert_new_node(&root, 5);
    node3 = avl_insert_new_node(&root, 15);
    
    /* Remove and free a node */
    if (node2)
        avl_remove_and_free_node(&root, node2);
    
    /* Clean up remaining nodes */
    if (node1)
        avl_remove_and_free_node(&root, node1);
    if (node3)
        avl_remove_and_free_node(&root, node3);
    
    return 0;
}
