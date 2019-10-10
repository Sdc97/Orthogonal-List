/*
Structs: InternalNode and headNode
Author: Steven Calvert
Description: headNode is used for column and row headers,
which permit access to their respective internal nodes. The
internalNode struct holds pointers in each direction, as well
as the nodes position in the matrix and its value. For usage with
the OList class developed for project 3&4.
*/
#include <cstddef>
#pragma once

struct internalNode {
public:
	void copy(internalNode temp) {
		row = temp.row;
		column = temp.column;
		value = temp.value;
		up = temp.up;
		down = temp.down;
		right = temp.right;
		left = temp.right;
	}
	int row = 0;
	int column = 0;
	int value = 0;
	internalNode* up = NULL;
	internalNode* down = NULL;
	internalNode* right = NULL;
	internalNode* left = NULL;
};

struct headNode {
public:
	int key = 0;
	internalNode* toInternal = NULL;
	headNode* next = NULL;
};