#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

/*    case of left rotate
 * 
 *    z                     y
 *     \                   / \
 *      y     == >        z   x
 *       \
 *        x
 */

static inline int max(int x, int y)
{
	return (x > y) ? x: y;
}

int height(struct node *node)
{
	return (node == NULL) ? 0 : 1+max(height(node->left), height(node->right));
}

int get_height(struct node *node)
{
	if(node == NULL)
		return 0;
	return node->height;
}

void update_height(struct node *node)
{
	node->height = 1+max(get_height(node->left), get_height(node->right));
}

// An avl tree balance infactor should be in [-1,1]
int calc_balance(struct node *node)
{
	if (node == NULL)
		return 0;
	return get_height(node->left) - get_height(node->right);
}


// use avl version newNode which has height
struct node *newNode(int v)
{
	struct node *node = (struct node *) malloc(sizeof(struct node));
	node->left = node->right = NULL;
	node->val = v;
	node->height = 1;
	return node;
}

struct node *getMinValueNode(struct node *root)
{
	struct node *current = root;
	while(current && current->left != NULL)
		current = current->left;

	return current;
}

// assume x->y->z
struct node *right_rotate(struct node *x)
{
	struct node *y = x->left;	
	struct node *tmp = y->right;

	// rotate
	y->right = x;
	x->left = tmp;

	// update height
	//x->height = height(x);
	//y->height = height(y);
	update_height(x);
	update_height(y);

	return y;
}

struct node *left_rotate(struct node *x) 
{
	struct node *y = x->right;
	struct node *tmp = y->left;	

	// rotate
	y->left = x;
	x->right = tmp;

	// update height
	// x->height = height(x);
	// y->height = height(y);
	update_height(x);
	update_height(y);

	// return new root of subtree
	return y;
}

struct node *avl_insert(struct node *root, int val)
{
	if (root == NULL)
		return newNode(val);

	if (val < root->val)
		root->left = avl_insert(root->left, val);
	else if (val > root->val)
		root->right = avl_insert(root->right, val);
	// equal value is not allowed
	else
		// nothing happened
		return root;

	//update height
	//root->height = height(root);
	update_height(root);

	// check if balanced
	int balance = calc_balance(root);

	// left left case
	if (balance > 1 && val < root->left->val)
		return right_rotate(root);

	// left right case
	if (balance > 1 && val > root->left->val){
		root->left = left_rotate(root->left);
		return right_rotate(root);
	}

	// right left case
	if (balance < -1 && val < root->right->val) {
		root->right = right_rotate(root->right);
		return left_rotate(root);
	}

	// right right case
	if (balance < -1 && val > root->right->val)
		return left_rotate(root);

	// no need to rotate, just return the old root node
	return root;
}

struct node *avl_delete(struct node *root, int val)
{
	if (root == NULL)
		return root;

	// go find the target node
	if (val < root->val)
		root->left = avl_delete(root->left, val);
	else if (val > root->val)
		root->right = avl_delete(root->right, val);
	// found
	else {
		// case of target node only has one child or no child
		if (root->left == NULL || root->right == NULL) {
			struct node *tmp = root->left ? root->left : root->right;
			// case of target node has no child
			if(tmp == NULL) {
				tmp = root;
				root = NULL;
			} else {
				// let its child be it
				memcpy(root, tmp, sizeof(struct node));
			}
			free(tmp);

		// case of target node has two children
		} else {
			//find its inorder successor
			struct node *inorder = getMinValueNode(root->right);
			root->val = inorder->val;
			root->right = avl_delete(root->right, inorder->val);
		} // end inner else
	}// end else

	// why do this again?
	// case of the single target node has been deleted, no need to rotate, just return
	if (root == NULL)
		return root;

	update_height(root);
	int balance = calc_balance(root);

	// rebalance, not same as insert
	// left left case
	if (balance > 1 && calc_balance(root->left) >= 0)
		return right_rotate(root);

	// left right case
	if (balance > 1 && calc_balance(root->left) < 0) {
		left_rotate(root->left);
		return right_rotate(root);	
	}

	// right right case
	if (balance < -1 && calc_balance(root->right) <= 0)
		return left_rotate(root);

	// right left case
	if (balance < -1 && calc_balance(root->right) > 0) {
		right_rotate(root->right);
		return left_rotate(root);
	}

	// case of balanced tree, no need to rotate
	return root;
}

int main()
{
	struct node *root = NULL;
	
	root = avl_insert(root, 10);
	root = avl_insert(root, 20);
	root = avl_insert(root, 30);
	root = avl_insert(root, 40);
	root = avl_insert(root, 50);
	root = avl_insert(root, 25);

	inorder_traversal(root);
	printf("\n[*] height of tree: %d\n", root->height);

	puts("[*] delete 40.");
	root = avl_delete(root, 40);
	inorder_traversal(root);
	return 0;
}
