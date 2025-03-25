#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int getLen(Node *head);


bool equalPaths(Node * root)
{
    // Add your code below
		if (root == NULL){
		return true;
	}

	if (root->left == NULL || root->right == NULL){
		if (root->left!= NULL){
			return equalPaths(root->left);
		}

		else{
			return equalPaths(root->right);
		}
	}
	
	int lCount = 0;
	int rCount = 0;


	if (root->left != NULL){

		lCount = getLen(root->left);

	}

	if (root->right != NULL){

		rCount = getLen(root->right);

	}

	return (lCount == rCount);

}

int getLen(Node *head){

	if (head == NULL){
		return 0;
	}

	int lCount = getLen(head->left);
	int rCount = getLen(head->right);
	int greatestLen = 0;

	if (lCount > rCount){
		greatestLen = lCount;
	}

	else{
		greatestLen = rCount;
	}

	return 1+greatestLen;


}

