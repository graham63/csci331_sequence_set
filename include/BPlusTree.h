#pragma once
#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <cassert>
#include "SequenceSet.h"

class BPlusTree {
private:
	int maxSize;
	bool isLeaf;
	BPlusTree* parent;
	std::vector<int> keys;
	std::vector<BPlusTree*> children;
	std::vector<SequenceSet*> values;

	/**
	 * If does the process of spliting the b+ tree
	 * when a node is a leaf and not.
	 *
	 * @return 
	 */
	BPlusTree* split();

	/**
	 * It checks if it's possible to make borrow or merge
	 *
	 * @return instance node
	 */
	BPlusTree* bm();

	/**
	 * These function do the process of borrow and merge.
	 *
	 */
	BPlusTree* borrow_left(BPlusTree* &left, const size_t& parent_pos);
	BPlusTree* merge_left(BPlusTree* & left, const size_t& parent_pos);
	
	BPlusTree* borrow_right(BPlusTree* &right, const size_t& parent_pos);
	BPlusTree* merge_right(BPlusTree* & right, const size_t& parent_pos);

	/**
	 * Function used to get right index position 
	 * and use on key/values container.
	 * 
	 * @param a key of a SequenceSet
	 * @return a value to be a cointer position.
	 */
	size_t find_pos(const int&);
	size_t find_pos(const int&)const;

	/**
	 * It removes both key and its object from
	 * B+ tree.
	 * 
	 * @param a position inside the vector of keys/values
	 */
	void remove(const size_t& pos);

public:

	/**
	 * Constructors and destructor implementation.
	 *
	 */
	BPlusTree(const bool& isleaf = true, BPlusTree* p = NULL,const int& size = 3);
	BPlusTree(const bool& isleaf = true, const int& size = 3);
	BPlusTree(const BPlusTree&);
	BPlusTree& operator=(const BPlusTree&);
	~BPlusTree();

	/**
	 * It sets a parent for this B+ tree node
	 * 
	 * @param a BPlusTree node
	 */
	void set_parent(BPlusTree* p);

	/**
	 * It gets the max size of this B+ tree
	 *
	 * @param value to max size
	 */
	void set_max_size(const int &);

	/**
	 * It returns max size of this b+ tree.
	 * 
	 * @return max size
	 */
	int getSize() {
		return maxSize;
	}

	SequenceSet* leftest();

	//int predecessor(const int&);
	int successor(const int&);

	/**
	 * It gets a key to check if a given key
	 * is being used or not.
	 * 
	 * @param int as key
	 * @return true if key inside, false if not
	 */
	bool find(const int& key) const;
	
	/**
	 * It gets a key and searchs inside the b+ tree
	 * for the SequenceSet object and returns it.
	 *
	 * @param key as an integer
	 * @return a SequenceSet object if found
	 */
	SequenceSet* search(const int& key);

	/**
	 * It gets a SequenceSet object an int value 
	 * as its key and insert it into the b+ tree.
	 * 
	 * @param a key
	 * @param SequenceSet object
	 * @return tree with sequence set object inside
	 */
	BPlusTree* insert(const int& key, SequenceSet* value);

	/**
	 * It gets a key for a SequenceSet inside the
	 * B+ tree and delete it.
	 * 
	 * @param a key for a SequenceSet
	 * @return a reference for a tree withoud the removed object
	 */
	BPlusTree* del(const int& key);

	/**
	 * It tells if tree is empty.
	 * 
	 * @return true if empty, false if not
	 */
	bool empty();

	/**
	 * It retuns first child of an instance
	 * of BPlusTree.
	 * 
	 * @return a B+ tree child
	 */
	BPlusTree* only_child();

	/**
	 * Getter function to get values.
	 * 
	 * @return values of b+ tree
	 */
	std::vector<SequenceSet*> getValues() {
		return values;
	}

	/**
	 * It prints the tree state as astring.
	 * 
	 * @return b+ tree as a string
	 */
	std::string toString();

	/**
	 * Getter function to get keys 
	 * 
	 * @return keys container reference
	 */
	std::vector<int> getKeys() {
		return keys;
	}

	/**
	 * Friend function to handle situation 
	 * when all pages have been removed from tree.
	 * 
	 * @param a page of a tree
	 * @return new page node
	 */
	friend BPlusTree* inherit(BPlusTree* &_root);

	/**
	 * Friend function to rebuild b+ tree.
	 *
	 * @param the current b+ tree
	 * @return the new b+ tree
	 */
	friend BPlusTree* rebuildTree(BPlusTree* prevTree);
	
	/**
	 * Friend function to rebuild b+ tree. With this
	 * function you can define the tree order.
	 *
	 * @param the current b+ tree
	 * @return the new b+ tree
	 */
	friend BPlusTree* rebuildTree(BPlusTree* prevTree, int order);
};

#endif
