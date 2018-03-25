/* bst.c template                 
 * Austin Johnson 
 * agjohns       
 * ECE 2230 Fall 2016
 * MP5
 *
 * Purpose: The purpose of this file is to provide
 *	    functions for the lab5.c drivers in order
 *	    to construct and test different binary
 *	    search trees. 
 *
 *
 * Assumptions: I assume these functions will receive
 *		as input a pointer to a binary search 
 *		tree. 
 *
 *   
 * Bugs: No bugs from the tests I have ran against it.  
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "bst.h"


/*
  Finds the tree element with the matching key and returns
  a pointer to the data block stored in that node. If the key
  does not exist in the tree the NULL is returned. 
*/
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

/*
   Creates the header block for the tree and sets the tree
   policy according to the user input. 
*/
bst_t *bst_construct (int tree_policy) {
	bst_t *T;
	T = (bst_t *) malloc(sizeof(bst_t));
	T->root = NULL;
	T->tree_policy = tree_policy;
	T->tree_size = 0;
	T->key_comp = 0;
	return T;
}

/*
   Frees all items stored in the tree by using an
   auxilliary function.  
*/
void bst_destruct (bst_t *tree) {
	bst_destruct_aux(tree->root);
	free(tree);
	tree = NULL;
}

/*
   An auxilliary function to recursively free
   the tree. 
*/
void bst_destruct_aux (bst_node_t *node) {
	if (node != NULL) {
		bst_destruct_aux(node->left);
		bst_destruct_aux(node->right);
		free(node->data_ptr);
		free(node);
	}
}

/*
   Inserts the memory block pointed to by data_t into the 
   tree with the corresponding key. It does this by using
   an auxillary function to perform the insert. It returns 0 
   if the key is already in the list and returns a 1 otherwise. 
*/
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

/*
   An auxilliary function to recursively find the location 
   to insert into the tree in order to keep it a binary search
   tree. It returns all of the nodes in the tree after the insert. 
*/
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
		if (key == (*node)->key) {
			free((*node)->data_ptr);
			(*node)->data_ptr = data;
		}
		else {	
			if (key < (*node)->key) 
				(*node)->left = bst_insert_aux(tree, &((*node)->left), key, data);
			else
				(*node)->right = bst_insert_aux(tree, &((*node)->right), key, data);
		}
		return *node;
	}
}

/*
   Removes the item in the tree with the matching key. It does
   this by using an auxilliary function to perform the deletion.
   It returns a pointer to the data block if the key is found
   and NULL otherwise. 
*/
data_t bst_remove (bst_t *tree, bst_key_t key) {
	data_t data = NULL;
	if (tree->tree_size == 1 && tree->root->key == key) {
		tree->key_comp = 1;
		data = tree->root->data_ptr;
		free(tree->root);
		tree->root = NULL;
		tree->tree_size = 0;
	}
	else {
		tree->key_comp = 0;
		bst_remove_aux (tree, tree->root, &data, key);
		if (data != NULL) tree->tree_size--;
	}
	return data;
}

/*
   An auxilliary function that recursively deletes a node in the tree
   corresponding to a given key. It returns two things --> it returns
   a pointer to the data block of the matching node and it also returns
   all of the nodes after the deletion. 
*/
bst_node_t *bst_remove_aux (bst_t *tree, bst_node_t *node, data_t *data, bst_key_t key) {
	
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
		if (*data == NULL) *data = node->data_ptr;

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
			//if tree->root only has one child
			if (tree->root->key == key && (tree->root->right == NULL
				|| tree->root->left == NULL)) tree->root = node;
			free(temp);
			return node;
		}
		//case 3 --> two children
		else {
			//find smallest key in right subtree
			bst_node_t *Rover = node->right;
			while (Rover->left != NULL) Rover = Rover->left;
			node->key = Rover->key;
			node->data_ptr = Rover->data_ptr;
			//reduce to case 1 or case 2
			node->right = bst_remove_aux(tree, node->right, data, Rover->key);						
		}
	}
	return node;
}

/*
   Returns the size of the tree. 
*/
int bst_size (bst_t *tree) {
	return tree->tree_size;
}

/*
   Returns the number of recent key comparisons. 
*/
int bst_stats (bst_t *tree) {
	return tree->key_comp;
}

/*
   Returns the internal path length of the tree
   through the use of an auxilliary function. The
   level of the tree starts at 0. 
*/
int bst_int_path_len (bst_t *tree) {
	return bst_int_path_len_aux(tree->root, 0);
}

/*
   An auxillary function that calculates the internal 
   path length of a tree starting at level 0. It returns
   the calculated internal path length.
*/
int bst_int_path_len_aux (bst_node_t *node, int level) {
	if (node == NULL) return 0;
	if (node->left == NULL && node->right == NULL) return level;
	return level + bst_int_path_len_aux(node->left, level+1) 
		     + bst_int_path_len_aux(node->right, level+1); 
}

/*
   Prints the binary search tree using an 
   auxilliary function. 
*/
void bst_debug_print_tree (bst_t *tree) {
	printf("\n");
	ugly_print(tree->root, 0);
	printf("\n");
}

/*
   A simple and not so elegant auxilliary function
   that recursively prints the tree according to its
   current level, spacing accordingly. 
*/
void ugly_print(bst_node_t *N, int level) {
	if (N == NULL) return;
	ugly_print(N->right, level+1);
	for (int i=0; i<level; i++) printf("     ");
	printf("%5d\n", N->key); /* field width is 5 */
	ugly_print(N->left, level+1);
}

/*
   A binary search tree validator that uses assertions 
   along with an auxilliary function to assert that the
   tree is still a binary search tree after it has been 
   operated on. 
*/
void bst_debug_validate (bst_t *tree) {
	int size = 0;
	assert(bst_debug_validate_rec(tree->root, INT_MIN, INT_MAX, &size) == 1);
	assert(size == tree->tree_size);
}

/*
   An auxilliary function that checks if a key is in its proper
   using a minimum and maximum values. These values are the bounds
   of the current key. It also checks that the size of the tree is 
   correct through the use of a counter. 
*/
int bst_debug_validate_rec (bst_node_t *N, int min, int max, int *count) {
	if (N == NULL) return 1;
	if (N->key <= min || N->key >= max) return 0;
	assert(N->data_ptr != NULL);
	*count += 1;
	return bst_debug_validate_rec(N->left, min, N->key, count) && 
		bst_debug_validate_rec(N->right, N->key, max, count);
}
