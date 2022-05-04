#ifndef TREE_HEADER
#define TREE_HEADER
#include <stdlib.h>
#include <stdio.h>

struct node {
	int val;
	struct node *left;
	struct node *right;
	int height;
};

static inline struct node* new_node(int v)
{
    struct node* node = (struct node *) malloc(sizeof(struct node));
    node->val = v;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void inorder_traversal(struct node *root)
{
	if (root == NULL)
		return ;

	inorder_traversal(root->left);
	printf("%d -> ", root->val);
	inorder_traversal(root->right);
}

static void preorder_traversal(struct node *root) 
{
	if (root == NULL)
		return ;

	printf("%d -> ", root->val);
	preorder_traversal(root->left);
	preorder_traversal(root->right);
}

static void postorder_traversal(struct node *root)
{
	if (root == NULL)
		return ;

	postorder_traversal(root->left);
	postorder_traversal(root->right);
	printf("%d -> ", root->val);
}

#endif
