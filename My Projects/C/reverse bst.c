#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

//initialising Node for BST, a Node has a value and a left and right child nodes

struct _Node{
  struct _Node * left;
  struct _Node * right;
  int data;
};

Node * addNode(Node * root, int value){
  Node * current = root;
  //if the root Node is NULL that means we want to create a new BST
  //we create a new Node allocate memory to it and return the node
  if(current == NULL){
    Node * new = (Node*)malloc(sizeof(struct _Node));
    new->left = NULL;
    new->right = NULL;
    new->data = value;
    return new;
  }

  //if the root Node isnt NULL but the value is already in the tree we do nothing
  if(current->data == value){
    current = NULL;
    return current;
  }

  //if the value is higher than current data but the left child of current is NULL we assign new to the left child of current and return the new root
  if(value > current->data){
    if(current->left == NULL){
      Node * new = (Node*)malloc(sizeof(struct _Node));  
      new->left = NULL;
      new->right = NULL;
      new->data = value;
      current->left = new;
      return current->left;
    }
    //if its not null we recursively call the function
    else{
     addNode(current->left, value);
    }

  }
  //if the value is lower than current data but the right child of current is NULL we assign new to the right child of current and return the new root
  else if(value < current->data){
    if(current->right == NULL){
      Node * new = (Node*)malloc(sizeof(struct _Node));
      new->left = NULL;
      new->right = NULL;
      new->data = value;
      current->right = new;
      return current->right;
    }
    //if its not null we recursively call the function
    else{
      addNode(current->right, value);
    }
  }
};
 
//this is gonna be a long one 
Node * removeNode(Node * root, int value){
  Node* remove = root;
  Node* replace;
  Node * substitute;
  int change = 0;

  //if the root is NULL we do nothing
  if(remove == NULL){
    return remove ;
  }
  else{
    //while loop here to traverse the three (did 1 cuz while true is not a thing in C)
    while(1){
//if the value is found
    if(remove->data == value){
//if left not null but right is null make root value left node value, free left node and make root point to the left of left node
      if(remove->left != NULL && remove->right == NULL){
	remove->data = remove->left->data;
	free(remove->left);
	remove->left = NULL;
        if(change > 0){
	  return root;
	}
	else{
	  return remove;
	}
      }
//if left null but right not null make root value right node value, free right node and make root point to the right of left node
      else if(remove->left == NULL && remove->right != NULL){
	remove->data = remove->right->data;
	free(remove->right);
	remove->right = NULL;
	if(change > 0){
	  return replace;
	}
	else{
	  return remove;
	}
      }
//if they are both null you just free the Node and make the Node NULL      
      else if(remove->left == NULL && remove->right == NULL){
        if(change > 0){
	  if(replace->left != NULL){
	    if(replace->left->data == value){
	      free(remove);
	      remove = NULL;
	      replace->left = NULL;
	      return replace;
	    }
	  }
	  else if(replace->right != NULL){
	    if(replace->right->data == value){
	      free(remove);
	      remove = NULL;
	      replace->right = NULL;
	      return replace;
	    }
	  }
	}
	else{
	  free(remove);
	  remove = NULL;
	}
	return remove;
      }
      //if none of the children are NULL then we go to the left child of the current Node, then if it has a right child, we set the node as the right child until we have no more right children, then we set the node to be removed's value to the child node we are currently on free that Node and set it to null
      else{
	substitute = remove->right;
	if(substitute->left == NULL){
	  remove->data = substitute->data;
	  remove->right = substitute->right;
	  free(substitute);
	  substitute = NULL;
	  return root;
	}
	else{
	  while(substitute->left != NULL){
	    replace = substitute;
	    substitute = substitute->left;
	  }
	  if(substitute->right != NULL){
	    remove->data = substitute->data;
	    replace->left = substitute->right;
	    free(substitute);
	    substitute = NULL;
	  }
	  else{
	    remove->data = substitute->data;
	    free(substitute);
	    replace->left = NULL;
	  }
	  return root;
	}
      }
    }
    else{
      if(value > remove->data){
	replace = remove;
        remove = remove->left;
	change ++;
      }
      else{
	replace = remove;
        remove = remove->right;
	change ++;
      }
    }
    }
  }
}



void displaySubtree(Node * N){
  //this is self explanatory
  if(N!=NULL){
    displaySubtree(N->right);
    printf("%d \n", N->data);
    displaySubtree(N->left);
  }
}

int numberLeaves(Node * N){
	int count = 0;
	//we check if the current node we have has children
	//if it does we recursively add to count by calling numberleaves again
	//on the left and right children of N if they are not NULL
	if(N->left != NULL || N->right !=NULL){
		if (N->left != NULL){
			count += numberLeaves(N->left);
		}
		if (N->right != NULL){
			count += numberLeaves(N->right);
		}
	}
	//if the Node we are currently on has no children we increment count by 1
	else{
		count ++;
	}
	return count;		
};

Node* removeSubtree(Node * root, int value){
	Node *left, *right;
	Node *current = root;
	//if tree is empty do nothing
	if(root == NULL){
	  return root;
	}
	//if the root value is equal to the value we were lookking for :)
	else if(root->data == value){
	  //this saves time on the recursive call 
	  if(root->left != NULL){
	    left = root->left;
            //we recursively call removeSubtree on the left of the Node with the value of the left Node
	    left = removeSubtree(left, left->data);
	  }
	  if(root->right != NULL){
	    right = root->right;
            //we recursively call removeSubtree on the right of the Node with the value of the right Node
	    right = removeSubtree(right, right->data);
	  }
	  //we free the Node and we set the Node to null, if we dont do this we will be getting some random numbers	
	  free(root);
	  root = NULL; 
	}
	// if value is higher than the current Node value we recursively call the function but on the left Node 
	else if(root->data < value){
	  root->left = removeSubtree(root->left, value);
	}
        // if value is lower than the current Node value we recursively call the function but on the right Node 
	else{
	  root->right = removeSubtree(root->right, value);
	}
        
	return root;
}
//dont need to explain this
int nodeDepth (Node * R, Node * N){
  if(N == NULL){
    return -1;
  }
  int depth = 0;
  int find = N->data;
  Node * traverse = R;
  while(traverse->data != find){
    if(traverse->left == NULL && traverse->right == NULL){
      return -1;
    }
    else if(traverse->data < find){
      depth = depth + 1;
      traverse = traverse->left;
    }
    else{
      depth = depth + 1;
      traverse = traverse->right;
    }
    if (traverse == NULL) {
      return -1;
    }
  }
  return depth;
}
