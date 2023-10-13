#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/

struct _Node {
  int data;
  struct _Node* left;
  struct _Node* right;
};

Node* addNode(Node *root, int value)
{
  Node *newNode = malloc(sizeof(Node));
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;

  Node *temp;
  Node *prev = NULL;

  temp = root;
  while(temp) {
    prev = temp;
    if (temp->data > newNode->data)
      temp = temp->right;
    else if (temp->data < newNode->data)
      temp = temp->left;
    else {
      free(newNode);
      return NULL;
    }
  }
  if (prev==NULL)
    root = newNode;
  else {
    if (prev->data > newNode->data)
      prev->right = newNode;
    else
      prev->left = newNode;
  }

  return root;
}

Node * freeSubtree(Node *N) {
  if (N == NULL)
    return NULL;
  freeSubtree(N->left);
  freeSubtree(N->right);
  free(N);
  N = NULL;
  return N;
}

void removeSubtreeWorker(Node *R, int value) {
  if (R == NULL)
    return;

  if (R->left && R->left->data == value) {
    freeSubtree(R->left);
    R->left = NULL;
    return;
  }

  if (R->right && R->right->data == value) {
    freeSubtree(R->right);
    R->right = NULL;
    return;
  }

  if (R->data < value)
    removeSubtreeWorker(R->left, value);
  else
    removeSubtreeWorker(R->right, value);  
}

Node* removeSubtree(Node *R, int value) {
  if (R == NULL)
    return NULL;

  if (R->data == value) {
    freeSubtree(R);
    return NULL;
  }
  removeSubtreeWorker(R, value);
  return R;
}

void displaySubtree(Node *N)
{
  if (N==NULL)
    {
      return ;
    }
  displaySubtree(N->right);
  printf("%d\n", N->data);
  displaySubtree(N->left);
}

int countNodes(Node *N)
{
  int count=0;
  if(N==NULL)
    return 0;
  count = count + countNodes(N->right);
  count = count + countNodes(N->left);
  count = count + 1;
  return count;
}

// this is the most complicated task
Node* removeNode(Node* root, int value)
{
  // Ensure root isn't null.
  if (root == NULL) {
    return NULL;
  }
  	
  if (value > root->data) {  // Value is in the left sub-tree.
    root->left = removeNode(root->left, value);
  } else if (value < root->data) { // Value is in the right sub-tree.
    root->right = removeNode(root->right, value);
  } else { // Found the correct node with value
    // Check the three cases - no child, 1 child, 2 child...
    // No Children
    if (root->left == NULL && root->right == NULL) {
      free(root);
      root = NULL;
    }
    // 1 child (on the right)
    else if (root->left == NULL) {
      Node *temp = root; // save current node
      root = root->right;
      free(temp);
    }
    // 1 child (on the left)
    else if (root->right == NULL) {
      Node *temp = root; // save current node
      root = root->left;
      free(temp);
    }
    // Two children
    else {
      // find minimal value of right sub tree
      Node *temp = root->left; 
      while(temp->right != NULL) {
        temp = temp->right;
      }
      root->data = temp->data; // duplicate the node
      root->left = removeNode(root->left, root->data); // delete the duplicate node
    }
  }	
  return root; // parent node can update reference
}

int numberLeaves(Node *N) {
  if (N == NULL)
    return 0;

  if (N->left == NULL && N->right == NULL)
    return 1;

  return numberLeaves(N->left) + numberLeaves(N->right);
}

int nodeDepth(Node *R, Node *N) {
  if (R == NULL || N == NULL)
    return -1;

  if (R == N)
    return 0;

  int subDepth = nodeDepth(R->data < N->data ? R->left : R->right, N);

  if (subDepth >= 0)
    return subDepth + 1;
  else
    return -1;
}

int sum (Node *N){
	int data = 0;
	if (N != NULL){
		data = N->data;
		return data + sum(N->right) + sum(N->left);
	}
	return 0;
}

float avgSubtree(Node *N){
	
	float n = (float)sum(N) / countNodes(N);
	return n;
}

int createTreeArray(Node * N, int array [], int numNodes){
  	if (N == NULL){
  		return numNodes;
  	}
  	if(N->right != NULL){
  		numNodes = createTreeArray(N->right, array, numNodes);
  	}
  	array [numNodes] = N->data;
  	numNodes++;
  	if(N->left != NULL){
  		numNodes = createTreeArray(N->left, array, numNodes);
  	}
  	return numNodes;
}

Node * createBst(int array[],int one, int two){
	if (one > two){
		return NULL;
	}
	int middle;
	middle = (one + two)/2;
	Node *new = malloc(sizeof(Node));
	new->data = array[middle];
	new->right = createBst(array, one, middle-1);
	new->left = createBst(array, middle + 1, two);
	return new;
}


// This functions converts an unbalanced BST to a balanced BST
Node* balanceTree(Node* root)
{	
	if((root->left == NULL) && (root->right == NULL)){
		return root;
	}
	Node * new;
	int treeStore [countNodes(root)];
	createTreeArray(root, treeStore, 0);
	// TODO: Implement this function
	new = createBst(treeStore, 0,((sizeof(treeStore)/sizeof(treeStore[0]))-1));
	return new;
}

