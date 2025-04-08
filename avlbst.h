#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

		virtual void rotationRight(AVLNode<Key,Value>* node);
		virtual void rotationLeft(AVLNode<Key,Value>* node);
		virtual void RL (AVLNode<Key,Value>* node);
		virtual void LR(AVLNode<Key,Value>* node);

		//uses if statements to decide which rotations
		// to use to rebalance tree

		//very important!
		virtual void treeBalance(AVLNode<Key,Value>* node);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{

		
    // TODO

		if(this->root_ == nullptr){
			this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
			return;
		}

	
  AVLNode<Key, Value> *parentTemp = nullptr;
  AVLNode<Key, Value> *AVLsearchTemp =  static_cast<AVLNode<Key, Value>*>(this->root_);


	while(AVLsearchTemp != nullptr){		// searching for parent node for insertion

		parentTemp = AVLsearchTemp;

		if(new_item.first < AVLsearchTemp->getKey()){
			AVLsearchTemp = AVLsearchTemp->getLeft();
		}

		else if(new_item.first > AVLsearchTemp->getKey()) {
			AVLsearchTemp = AVLsearchTemp->getRight();
		
		}
		else{			//duplicate key case
			AVLsearchTemp->setValue(new_item.second);
			return;
		}
	}

	//parent node is found, creating new node for insertion


	AVLNode<Key, Value>* insertion = new AVLNode<Key,Value>(new_item.first, new_item.second, parentTemp);
	if(parentTemp == NULL){
			this->root_ = insertion;
	}

	else if(new_item.first < parentTemp->getKey()){
		parentTemp->setLeft(insertion);
		//insertion->setParent(parentTemp);
	}

	else{
		parentTemp->setRight(insertion);
		//insertion->setParent(parentTemp);

	}

//balance

treeBalance(insertion);

}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO


	//locate target node

	AVLNode<Key,Value> *targetNode = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

//not found
	if(targetNode == nullptr){
		return;
	}

	//store child and parent

	AVLNode<Key, Value> *parentNode = targetNode->getParent();
	bool left = (parentNode) && (parentNode->getLeft() == targetNode);




	if (targetNode->getLeft() != nullptr && targetNode->getRight() != nullptr){
			AVLNode<Key,Value> *predecessorNode = static_cast<AVLNode<Key,Value>*>(this->predecessor(targetNode));

			bool ifRoot = (targetNode == static_cast<AVLNode<Key,Value>*>(this->root_));
			nodeSwap(targetNode, predecessorNode);
			if(ifRoot){
				this->root_ = predecessorNode;
			}
			//targetNode = predecessorNode;

		parentNode = targetNode->getParent();
		left = (parentNode) && (parentNode->getLeft() == targetNode);
	

}



		AVLNode<Key,Value> *childNode;

		//finding child

		if(targetNode->getLeft() != nullptr){
			childNode = targetNode->getLeft();
		}

		else if(targetNode->getRight() != nullptr){
			childNode = targetNode->getRight();
		}

		else{
			childNode = nullptr;
		}

// child stays null if node has no children

//update parent pointer


//update child parent
		if(childNode != nullptr){
			childNode->setParent(parentNode);
		}

		if(parentNode == nullptr){
					this->root_ = childNode;

		}
		else if(parentNode->getLeft() == targetNode){ // (bool left)
			parentNode->setLeft(childNode);
		}

		else {
			parentNode->setRight(childNode);
		}

		



		delete targetNode;


		//REBALANCE  

		AVLNode<Key,Value> *balanceNode = parentNode;

		while(balanceNode != nullptr){

			int8_t balDifference;

			if(left){
					balDifference = 1;			//left decreased
			}

			else{
					balDifference = -1;		//right decreased

			}

			balanceNode->updateBalance(balDifference);
			int8_t bal = balanceNode->getBalance();


	


		if(bal == 2){			//left rotation
			
			AVLNode<Key,Value> *rightNode = balanceNode->getRight();
			int8_t rBal = 0;

			if(rightNode != nullptr){
				rBal = rightNode->getBalance();
			}

			if(rBal == -1){		//RL case
				RL(balanceNode);
			}

			else{
				rotationLeft(balanceNode);
/*
				if(rBal == 0){
					break;
				}
				*/
			}

		}

		else if(bal == -2){		//right rotation (opposite of above)
			AVLNode<Key,Value> *leftNode = balanceNode->getLeft();
			int8_t lBal = 0;

			if(leftNode != nullptr){
			
				 lBal = leftNode->getBalance();

			}

			if(lBal == 1){		//LR case
				LR(balanceNode);
			}

			else{
				rotationRight(balanceNode);
/*
				if(lBal== 0){
					break;
				}
				*/
			}
}

		else if(bal == 0){

		}

			else{
				break;
			}

		



	//continue up the tree

			AVLNode<Key,Value> *parent = balanceNode->getParent();

			if(parent != nullptr){ 		
				if(parent->getLeft() == balanceNode){
					left = true;
				}

				else{
					left = false;
				}



			}

			else{
				left = false;
			}

			balanceNode = parent;
		}

	


}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotationRight(AVLNode<Key,Value>* node){

AVLNode<Key, Value>* temp = node->getLeft();
if (temp == nullptr){
	return;
}

int8_t bal = temp->getBalance();


// shifting temp's left subtree to given node's left
node->setLeft(temp->getRight());

if(temp->getRight() != nullptr){
	temp->getRight()->setParent(node);
}

//link temp to given node's parent

temp->setParent(node->getParent());
if(node->getParent() == nullptr){ // given node = root 
	this->root_ = temp;
}

else if(node == node->getParent()->getLeft()){
	node->getParent()->setLeft(temp);
}

else{
	node->getParent()->setRight(temp);
}

//given node goes to the right

temp->setRight(node);
node->setParent(temp);

//reset balances



if(bal == 0){
	node->setBalance(-1);
	temp->setBalance(1);

}
else if(bal == -1){
	node->setBalance(0);
	temp->setBalance(0);
}

else if(bal == 1){
	node->setBalance(0);
	temp->setBalance(-1);
}

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotationLeft(AVLNode<Key,Value>* node){
AVLNode<Key, Value> *temp = node->getRight();

if (temp == nullptr){
	return;
}

int8_t bal = temp->getBalance();


//reverse of right rotation 

// shifting temp's left subtree to given node's right
node->setRight(temp->getLeft());

if(temp->getLeft() != nullptr){
	temp->getLeft()->setParent(node);
}

//link temp to given node's parent

temp->setParent(node->getParent());
if(node->getParent() == nullptr){ // given node = root 
	this->root_ = temp;
}

else if(node == node->getParent()->getLeft()){
	node->getParent()->setLeft(temp);
}

else{
	node->getParent()->setRight(temp);
}

//given node goes to the left

temp->setLeft(node);
node->setParent(temp);

//reset balances


if(bal == 0){
	node->setBalance(1);
	temp->setBalance(-1);

}
else if(bal == 1){
	node->setBalance(0);
	temp->setBalance(0);
}

else if(bal == -1){
	node->setBalance(0);
	temp->setBalance(1);
}

}

template<class Key, class Value>
void AVLTree<Key, Value>::RL (AVLNode<Key,Value>* node){

//right and left rotations here

if (node == nullptr || node->getRight() == nullptr){
	return;
}

AVLNode<Key,Value> *rChild = node->getRight();
AVLNode<Key,Value> *pivot = rChild->getLeft();


int8_t bal = pivot->getBalance();		//balance of pivot

//RL rotation
rotationRight(rChild);
rotationLeft(node);



//fixing balances

if(bal == 1){		//left heavy
	node->setBalance(-1);
	rChild->setBalance(0);
}

else if(bal == -1){		//left heavy
	node->setBalance(0);
	rChild->setBalance(1);
}

else{		//balanced
	node->setBalance(0);
	rChild->setBalance(0);
}

//pivot == root after
pivot->setBalance(0);

}

template<class Key, class Value>
void AVLTree<Key, Value>::LR(AVLNode<Key,Value>* node){


//reverse of RL

if (node == nullptr || node->getLeft() == nullptr){
	return;
}

AVLNode<Key,Value> *lChild = node->getLeft();
AVLNode<Key,Value> *pivot = lChild->getRight();


int8_t bal = pivot->getBalance();		//balance of pivot

//RL rotation
rotationLeft(lChild);
rotationRight(node);



//fixing balances

if(bal == 1){		//left heavy
	node->setBalance(0);
	lChild->setBalance(-1);
}

else if(bal == -1){		//left heavy
	node->setBalance(1);
	lChild->setBalance(0);
}

else{		//balanced
	node->setBalance(0);
	lChild->setBalance(0);
}

//pivot == root after
pivot->setBalance(0);

}

template<class Key, class Value>
void AVLTree<Key,Value>::treeBalance(AVLNode<Key,Value>* node){

//walk up the tree & check balances

AVLNode<Key,Value> *temp = node->getParent();


while(temp != nullptr){


	//check if node is left or right child
if(node == temp->getLeft()){

	temp->updateBalance(-1);
	
}

	else{
		temp->updateBalance(1);
	}


	int8_t bal = temp->getBalance();


	if (bal == 0){		// no rotations needed
		break;
	}

	// left and right rotation cases :

	if (bal == -2){ 
		
		AVLNode<Key,Value> *left = temp->getLeft();
		if(left == nullptr){
			break;
		}
		int8_t lBal = left->getBalance();


		if(lBal >= 0){   
			LR(temp);
		}

		else{ 
			rotationRight(temp);
		}


		break;
	}


//opposite of other case

	if (bal == 2){ 

		AVLNode<Key,Value> *right = temp->getRight();
		if(right == nullptr){
			break;
		}
		int8_t rBal = right->getBalance();

		if(rBal <= 0){  
			RL(temp);
		}

		else{ 
			rotationLeft(temp);
		
			
		}


		break;

	}



	node = temp;
	temp = temp->getParent();

	
}



}



#endif
