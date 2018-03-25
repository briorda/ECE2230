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
#include <limits.h>
#include "bst.h"


data_t bst_access (bst_t *tree, bst_key_t key) {
	bst_node_t *Rover = tree->root;	
	tree->key_comp = 0;
	//tree is empty
	if (tree->tree_size == 0)
		return NULL;	
	while (Rover != NULL && Rover->key != key) {
		tree->key_comp++;
		if (key > Rover->key) {
			tree->key_comp++;
			Rover = Rover->right;
		}
		else {
			Rover = Rover->left;
			tree->key_comp++;
		}
	}
	return Rover == NULL ? NULL : Rover->data_ptr;
}


bst_t *bst_construct (int tree_policy) {
	bst_t *T;
	T = (bst_t *) malloc(sizeof(bst_t));
	T->root = NULL;
	T->tree_policy = tree_policy;
	T->tree_size = 0;
	T->key_comp = 0;
	return T;
}


void bst_destruct (bst_t *tree) {
	bst_destruct_aux(tree->root);
	free(tree);
	tree = NULL;
}

void bst_destruct_aux (bst_node_t *node) {
	if (node != NULL) {
		bst_destruct_aux(node->left);
		bst_destruct_aux(node->right);
		//free(node->data_ptr);
		free(node);
		node = NULL;
	}
}

int bst_insert (bst_t *tree, bst_key_t key, data_t data) {
	tree->key_comp = 0;
	data_t found = bst_access (tree, key);
	bst_insert_aux (tree, &tree->root, key, data);
	if (found == NULL) {
		tree->tree_size++;
		return 1;
	}
	else 
		return 0;
}

bst_node_t *bst_insert_aux (bst_t *tree, bst_node_t **node, bst_key_t key, data_t data) {
	tree->key_comp++;
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
		tree->key_comp++;
		if (key == (*node)->key) 
			(*node)->data_ptr = data;
		else {	
			if (key < (*node)->key) 
				(*node)->left = bst_insert_aux(tree, &((*node)->left), key, data);
			else
				(*node)->right = bst_insert_aux(tree, &((*node)->right), key, data);
		}
		return *node;
	}
}

data_t bst_remove (bst_t *tree, bst_key_t key) {
	tree->key_comp = 0;
	data_t data = NULL;
	bst_remove_aux (tree, tree->root, &data, key);
	if (data != NULL) tree->tree_size--;
	return data;
}

bst_node_t *bst_remove_aux (bst_t *tree, bst_node_t *node, data_t *data, bst_key_t key) {

	if (*data == NULL && node->key == key) *data = node->data_ptr;

	//base case
	if (node == NULL) return NULL;
	
	//key is smaller than current node
	else if (key < node->key) {
		tree->key_comp++;
		node->left = bst_remove_aux(tree, node->left, data, key);
	}
	//key is greater than current node
	else if (key > node->key) {
		tree->key_comp++;
		node->right = bst_remove_aux(tree, node->right, data, key);
	}
	//found key
	else {
		tree->key_comp++;

		//case 1 --> no child
		if (node->left == NULL && node->right == NULL) {
			free(node);
			return NULL;
		}
		//case 2 --> one child
		else if (node->left == NULL || node->right == NULL) {
			bst_node_t *temp = node;
			if (node->left == NULL) node = node->right;
			else node = node->left;
			free(temp);
			return node;
		}
		//case 3 --> two children
		else {
			//find smallest key in right subtree
			bst_node_t *Rover = node->right;
			while (Rover->left != NULL) Rover = Rover->left;
			node->key = Rover->key;
			//reduce to case 1 or case 2
			node->right = bst_remove_aux(tree, node->right, data, Rover->key);						
		}
	}
	return node;
}


int bst_size (bst_t *tree) {
	return tree->tree_size;
}


int bst_stats (bst_t *tree) {
	return tree->key_comp;
}


int bst_int_path_len (bst_t *tree) {
	return bst_int_path_len_aux(tree->root, 0);
}

int bst_int_path_len_aux (bst_node_t *node, int level) {
	if (node == NULL) return 0;
	if (node->left == NULL && node->right == NULL) return level;
	return level + bst_int_path_len_aux(node->left, level+1) 
		     + bst_int_path_len_aux(node->right, level+1); 
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
	int size = 0;
	assert(bst_debug_validate_rec(tree->root, INT_MIN, INT_MAX, &size) == 1);
	assert(size == tree->tree_size);
}

int bst_debug_validate_rec (bst_node_t *N, int min, int max, int *count) {
	if (N == NULL) return 1;
	if (N->key <= min || N->key >= max) return 0;
	assert(N->data_ptr != NULL);
	*count += 1;
	return bst_debug_validate_rec(N->left, min, N->key, count) && 
		bst_debug_validate_rec(N->right, N->key, max, count);
}
