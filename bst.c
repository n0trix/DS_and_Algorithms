#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct node *insert_node(struct node *root, int key)
{
	if (root == NULL)
		return new_node(key);

	if(key < root->val)
		root->left = insert_node(root->left, key);
	else
		root->right = insert_node(root->right, key);

	return root;
}

// find min value node of subree/tree
struct node *getMinValueNode(struct node *root)
{
	struct node *current = root;
	while(current && current->left != NULL)
		current = current->left;

	return current;
}

//delete a node
struct node *delete_node(struct node *root, int key)
{
	// case of  target node not found 
	if( root == NULL ) return root; // or return NULL;
	
	if( key < root->val )
		root->left = delete_node(root->left, key);
	else if( key > root->val )
		root->right = delete_node(root->right, key);

	// find target node
	else
	{
		if( root->left != NULL && root->right != NULL )
		{
			// case of target node has two children
			// we need find its inorder successor
			struct node *inorder_successor = getMinValueNode(root->right);
			root->val = inorder_successor->val;
			root->right = delete_node(root->right, inorder_successor->val);
		} else if (root->left == NULL) {
			struct node *retNode = root->right;
			free(root);
			return retNode;
		}
		else {
			struct node *retNode = root->left;
			free(root);
			return retNode;
		}
	}
	return root;
}

int main()
{
	//test
	
	struct node *root = NULL;
	root = insert_node(root, 1);
	root = insert_node(root, 3);
	root = insert_node(root, 5);
	root = insert_node(root, 6);
	root = insert_node(root, 9);
	root = insert_node(root, 2);

	puts("[*] build a tree...");
	inorder_traversal(root);
	puts("");

	puts("[*] delete 6.");
	root = delete_node(root, 6);

	puts("[*] now tree is:");
	inorder_traversal(root);

	return 0;
}
