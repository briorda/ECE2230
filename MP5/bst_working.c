/* bst.c template                 
 * Austin Johnson 
 * agjohns       
 * ECE 2230 Fall 2016
 * MP5
 *
 * Purpose:    
 *
 *
 * Assumptions: 
 *
 *   
 * Bugs: 
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"


data_t bst_access (bst_t *tree, bst_key_t key) {
	bst_node_t *Rover = tree->root;	
	//tree is empty
	if (tree->tree_size == 0)
		return NULL;	
	while (Rover != NULL && Rover->key != key) {
		tree->num_recent_key_comparisons++;
		if (key > Rover->key)
			Rover = Rover->right;
		else
			Rover = Rover->left;
	}
	return Rover == NULL ? NULL : Rover->data_ptr;
}


bst_t *bst_construct (int tree_policy) {
	bst_t *T;
	T = (bst_t *) malloc(sizeof(bst_t));
	T->root = NULL;
	T->tree_policy = tree_policy;
	T->tree_size = 0;
	T->num_recent_key_comparisons = 0;
	return T;
}


void bst_destruct (bst_t *tree) {
	bst_node_t *N = tree->root;
	bst_node_t *temp = NULL;
	while (N != NULL) {
		temp = N;
		N = N->left != NULL ? N->left : N->right;
		free(temp->data_ptr);
		free(temp);
	}
	free(N);
	free(tree);
}


int bst_insert (bst_t *tree, bst_key_t key, data_t data) {
	data_t found = bst_access (tree, key);
	bst_insert_aux (&tree->root, key, data);
	if (found == NULL) {
		tree->tree_size++;
		return 1;
	}
	else 
		return 0;
}

bst_node_t *bst_insert_aux (bst_node_t **node, bst_key_t key, data_t data) {
	if (*node == NULL) { 
		//allocate new node
		bst_node_t *leaf = (bst_node_t *) malloc(sizeof(bst_node_t));
		leaf->data_ptr = data;
		leaf->key = key;
		leaf->left = NULL;
		leaf->right = NULL;
		*node = leaf;
		return *node;
	}
	else {
		if (key == (*node)->key) {
			(*node)->data_ptr = data;
		}	
		else if (key < (*node)->key)
			(*node)->left = bst_insert_aux(&((*node)->left), key, data);
		else
			(*node)->right = bst_insert_aux(&((*node)->right), key, data);
		return *node;
	}
}

data_t bst_remove (bst_t *tree, bst_key_t key) {
	data_t data = NULL;
	bst_remove_aux (tree->root, &data, key);
	return data;
}

bst_node_t *bst_remove_aux (bst_node_t *node, data_t *data, bst_key_t key) {

	bst_node_t *temp = node;
	if (*data == NULL && node->key == key) *data = node->data_ptr;

	//base case
	if (node == NULL) return NULL;
	
	//key is smaller than current node
	else if (key < node->key) node->left = bst_remove_aux(node->left, data, key);
	
	//key is greater than current node
	else if (key > node->key) node->right = bst_remove_aux(node->right, data, key);

	//found key
	else {
		//case 1 --> no child
		if (node->left == NULL && node->right == NULL) {
			free(node);
			temp = NULL;
		}
		//case 2 --> one child
		else if (node->left == NULL || node->right == NULL) {
			temp = node;
			if (node->left == NULL) node = node->right;
			else node = node->left;
			free(temp);
			temp = node; //get new node
		}
		//case 3 --> two children
		else {
			//find smallest key in right subtree
			bst_node_t *Rover = node;
			while (Rover->right != NULL) Rover = Rover->right;
			node->key = Rover->key;
			//reduce to case 1 or case 2
			node->right = bst_remove_aux(node->right, data, Rover->key);						
		}
	}
	return temp;
}


int bst_size (bst_t *tree) {

	return 0;
}


int bst_stats (bst_t *tree) {

	return 0;
}


int bst_int_path_len (bst_t *tree) {

	return 0;
}

void ugly_print(bst_node_t *N) {
	if (N == NULL) return;
	ugly_print(N->left);
	printf("%5d \n", N->key); /* field width is 5 */
	ugly_print(N->right);
}

void bst_debug_print_tree (bst_t *tree) {
	ugly_print(tree->root);
}


void bst_debug_validate (bst_t *tree) {

}

