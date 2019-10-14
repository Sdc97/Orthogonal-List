/*
Class: Orthogonal List
Author: Steven Calvert
Description: Holds and allows operations on 
orthogonal lists, to represent a more space efficient
sparse matrix.
*/

#pragma once
#include "nodes.h"
#include <cstddef>
#include <iomanip>
#include <string>
using namespace std;
class OList
{
private:
  // Define row and column size
  int rowSize = 0;
  int columnSize = 0; 
  // Create 5 pointers for matrix access
  headNode* headColumn; // Point to the initial head column node
  headNode* headRow; // Point to initial head row node
  headNode* currRow; // Header node iterator pointer
  headNode* currColumn; // Header node iterator pointer
  internalNode* currElement; // Internal node iterator
public:
	int getRowSize() { return rowSize; }; 
	int getColumnSize() { return columnSize; };
	void insert(int record, int row, int column);
	void insertHelp(internalNode* temp);
	void insertColumn(internalNode* temp, headNode* column);
	void insertRow(internalNode* temp, headNode* row);
	void placeInternalR(internalNode* temp, internalNode* curr);
	void placeInternalD(internalNode* temp, internalNode* curr);
	void printMatrix();
	string deleteElement(int row, int column);
	int deleteHelp(internalNode* temp);
	internalNode* findElement(int row, int column);
	void transpose();
	void addMatrix(OList addend);
	OList multMatrix(OList multiplier);
	OList(int record, int row, int column);
	OList();
	~OList();
};

/*
  Secondary Constructor: OList
  Input: initial record(int), number of rows(int), number of columns(int)
  Output: None
  Initialized the List by creating new head nodes for the rows and columns. Inserts
  the initial element given.
*/
inline OList::OList(int record, int row, int column)
{
	headColumn = currColumn = new headNode();
	headRow = currRow = new headNode();
	currElement = NULL;
	insert(record, row, column);

}


/*
  Default Constructor: OList
  Input: None
  Output: None
  Creates new head nodes for rows and columns.
*/
OList::OList()
{
	headColumn = currColumn = new headNode();
	headRow = currRow = new headNode();
	currElement = NULL;
}

/*
  Insert member function
  Input: record to be inserted(int), row(int), and column(int)
  Output: None
  Creates a new internal node for the record with the proper row and column.
  Calls the insertHelp member function to complete the insertion. Updates the
  row and column sizes accordingly.
*/
void OList::insert(int record, int row, int column) {
	internalNode* temp = new internalNode();
	temp->value = record;
	temp->row = row;
	temp->column = column;
	if (column > columnSize) {
		columnSize = column;
	}
	if (row > rowSize) {
		rowSize = row;
	}
	insertHelp(temp);

}

/*
  insertHelp member function
  Input: new record to be inserted(internalNode*)
  Output: None
  Transient function. Passes the node through to the proper insertion for column and
  rows.
*/
inline void OList::insertHelp(internalNode* temp)
{
	insertColumn(temp, headColumn);
	insertRow(temp, headRow);
}

/*
  insertColumn member function
  Input: New record to be inserted(internalNode*), and current column header(headNode*)
  Output: None
  Searches column head nodes for correct column, then passes the node forward to
  placeInternalD member function to iterate through internal nodes. Recursively iterate
  through head nodes.
*/
inline void OList::insertColumn(internalNode* temp, headNode* column)
{
	if (column->next == NULL) {
		column->next = new headNode();
		column->next->key = temp->column;
		column->next->toInternal = temp;
	}
	else if (temp->column < column->next->key) {
		headNode* newHead = new headNode();
		newHead->key = temp->column;
		newHead->next = column->next;
		column->next = newHead;
		newHead->toInternal = temp;
	}
	else if (temp->column == column->next->key) {
		if (temp->row < column->next->toInternal->row) {
			temp->down = column->next->toInternal;
			column->next->toInternal->up = temp;
			column->next->toInternal = temp;
		}
		else {
			placeInternalD(temp, column->next->toInternal);
		}
	}
	else {
		return insertColumn(temp, column->next);
	}
}

/*
  insertRow member function
  Input: New record to be inserted(internalNode*), and current row header(headNode*)
  Output: None
  Searches row head nodes for correct row, then passes the node forward to
  placeInternalR member function to iterate through internal nodes. Recursively iterate
  through head nodes.
*/
inline void OList::insertRow(internalNode* temp, headNode* row)
{
	if (row->next == NULL) {
		row->next = new headNode();
		row->next->key = temp->row;
		row->next->toInternal = temp;
	}
	else if (temp->row < row->next->key) {
		headNode* newHead = new headNode();
		newHead->key = temp->row;
		newHead->next = row->next;
		row->next = newHead;
		newHead->toInternal = temp;
	}
	else if (temp->row == row->next->key) {
		if (temp->column < row->next->toInternal->column) {
			temp->right = row->next->toInternal;
			row->next->toInternal->left = temp;
			row->next->toInternal = temp;
		}
		else {
			placeInternalR(temp, row->next->toInternal);
		}
	}
	else {
		return insertRow(temp, row->next);
	}
}

/*
  placeInternalR member function
  Input: New record(internalNode*), and current internal node(internalNode*)
  Output: None
  Searches through internal nodes, iterating in the "right" direction to find the
  correct location for the record.
*/
inline void OList::placeInternalR(internalNode* temp, internalNode* curr)
{
	if (curr->right == NULL) {
		curr->right = temp;
		temp->left = curr;
	}
	else if (temp->column < curr->right->column) {
		temp->left = curr;
		temp->right = curr->right;
		curr->right->left = temp;
		curr->right = temp;
	}
	else {
		return placeInternalR(temp, curr->right);
	}
}

/*
  placeInternalD member function
  Input: New record(internalNode*), and current internal node(internalNode*)
  Output: None
  Searches through internal nodes, iterating in the "down" direction to find the
  correct location for the record.
*/
inline void OList::placeInternalD(internalNode* temp, internalNode* curr)
{
	if (curr->down == NULL) {
		curr->down = temp;
		temp->up = curr;
	}
	else if (temp->row < curr->down->row) {
		temp->up = curr;
		temp->down = curr->down;
		curr->down->up = temp;
		curr->down = temp;
	}
	else {
		return placeInternalD(temp, curr->down);
	}
}

/*
  printMatrix member function
  Input: None
  Output: None
  Prints the current matrix in a standard form, ensures that the values are aligned correctly
  by using output stream manipulation.
*/
inline void OList::printMatrix()
{
	currRow = headRow->next;
	currColumn = headColumn;
	for (int i = 1; i <= rowSize; i++) {
		if (i < currRow->key) {
			for (int j = 0; j < columnSize; j++) {
				cout << setw(4) << right << 0;
			}
			cout << endl << endl;
		}
		else if (i == currRow->key) {
			currElement = currRow->toInternal;
			for (int j = 1; j <= columnSize; j++) {
				if (currElement == NULL) {
					cout <<setw(4) << right << 0;
				}
				else {
					if (j < currElement->column) {
						cout << setw(4) << right << 0;
					}
					else {
						cout << setw(4) << right << currElement->value;
						currElement = currElement->right;
					}
				}
			}
			currRow = currRow->next;
			cout << endl << endl;
		}
	}

}

/*
  deleteElement member function
  Input: row(int), column(int)
  Output: Message to confirm deletion(string)
  Searches for an element with the given row and column, and removes it from
  the matrix, updating all other pointers, sizes, and head nodes accordingly. 
  Outputs a message indicating what the element was and its location if found,
  else indicates that there is no node at that location.
*/
inline string OList::deleteElement(int row, int column)
{
	int current;
	currRow = headRow;
	currColumn = headColumn;
	if (row == 1) {
		currElement = currRow->next->toInternal;
		if (column == 1) {
			currRow->next->toInternal = currElement->right;
			currElement->right->left = NULL;
			currColumn->next->toInternal = currElement->down;
			currElement->down->up = NULL;
			current = currElement->value;
			delete currElement;
			return "Removed element " + to_string(current) + " at position (" + to_string(row) + ", " + to_string(column) + ")";
		}
		else {
			for (int i = currColumn->next->key; i < rowSize; i = currColumn->next->key) {
				if (column < i) {
					return "Element not in matrix.";
				}
				else if (column == i) {

					currColumn->next->toInternal = currElement->down;
					currElement->down->up = NULL;
					currColumn->toInternal->right = currColumn->next->next->toInternal;
					currColumn->next->next->toInternal->left = currColumn->toInternal;
					current = currElement->value;
					delete currElement;
					return "Removed element " + to_string(current) + " at position (" + to_string(row) + ", " + to_string(column) + ")";
				}
				currColumn = currColumn->next;
				currElement = currElement->right;
			}
		}
	}
	for (int i = currRow->next->key; i < rowSize; i = currRow->next->key) {
		if (row < i) {
			return "Element not in matrix.";
		}
		else if (row == i) {
			currElement = currRow->next->toInternal;
			for (int j = currElement->column; j < columnSize ; j = currElement->column) {
				if (column < j) {
					return "Element not in matrix.";
				}
				else if (column == j) {
					current = currElement->value;
					deleteHelp(currElement);
					return "Removed element " + to_string(current) + " at position (" + to_string(row) + ", " + to_string(column) + ")";
				}
				currElement = currElement->right;
				if (currElement == NULL) {
					break;
				}
			}
		}
		currRow = currRow->next;
	}
	return "Element not in matrix";
}

/*
  deleteHelp member function
  Input: Record to be deleted(internalNode*)
  Output: 0(this can be changed to void)
  Ensures all of the pointers connected to the node are pointed to
  the next element.
  *****TODO: Cover edge cases of elements on the edges of the matrix.
*/
inline int OList::deleteHelp(internalNode* temp)
{
	temp->left->right = temp->right;
	temp->right->left = temp->left;
	temp->up->down = temp->down;
	temp->down->up = temp->up;
	delete temp;
	return 0;
}

/*
  findElement member function
  Input: row(int), column(int)
  Output: Record if found(internalNode*), else NULL
  Finds an element based on its position in the matrix, and returns a
  pointer to the node if it is found, else returns a null pointer.
*/
inline internalNode* OList::findElement(int row, int column)
{
	currRow = headRow->next;
	while (currRow != NULL) {
		currElement = currRow->toInternal;
		while (currElement != NULL) {
			if (row == currElement->row && column == currElement->column) {
				return currElement;
			}
			currElement = currElement->right;
		}
		currRow = currRow->next;
	}
	return NULL;
}

/*
  transpose member function
  Input: None
  Output: None
  Transforms the matrix into its traspose by changing row and column head pointers.
*/
inline void OList::transpose()
{
	currRow = headRow->next;
	while (currRow != NULL) {
		currElement = currRow->toInternal;
		while (currElement != NULL) {
			internalNode* tempNode = new internalNode();
			tempNode->copy(*currElement);
			currElement->right = tempNode->down;
			currElement->left = tempNode->up;
			currElement->down = tempNode->right;
			currElement->up = tempNode->left;
			currElement->row = tempNode->column;
			currElement->column = tempNode->row;
			currElement = currElement->down;
			delete tempNode;
		}
		currRow = currRow->next;
	}
	int tempHold = columnSize;
	columnSize = rowSize;
	rowSize = tempHold;
	headNode* temp = headColumn;
	headColumn = headRow;
	headRow = temp;
	return;
}

/*
  addMatrix member function
  Input: Matrix to be added(OList)
  Output: None
  Adds the given matrix to the current one, adhering to matrix addition rules.
*/
inline void OList::addMatrix(OList addend)
{
	if (rowSize != addend.getRowSize() || columnSize != addend.getColumnSize()) {
		cout << "Matrices are different sizes." << endl;
		return;
	}
	for (int i = 1; i <= rowSize; i++) {
		for (int j = 1; j <= columnSize; j++) {
			internalNode* temp = addend.findElement(i, j);
			if (temp != NULL) {
				currElement = findElement(i, j);
				if (currElement == NULL) {
					insert(temp->value, temp->row, temp->column);
				}
				else {
					currElement->value += temp->value;
				}
			}
		}
	}
	return;
}

/*
  multMatrix member function
  Input: Matrix to be multiplied(OList)
  Output: New matrix as the product of the current and parameter(OList)
  Multiplies the current matrix by the parameter to create a new matrix. Uses standard
  matrix multiplication rules.
*/
inline OList OList::multMatrix(OList multiplier)
{
	if (columnSize != multiplier.getRowSize()) {
		cout << "Incorrect dimensions of multiplier." << endl;
		return OList();
	}
	OList temp;
	int total = 0;
	currRow = headRow->next;
	while (currRow != NULL) {
		currElement = currRow->toInternal;
		if (currElement != NULL) {
			for (int i = 1; i <= columnSize+1; i++) {
				while (currElement != NULL) {
					internalNode* tempNode = multiplier.findElement(currElement->column, i);
					if (tempNode != NULL) {
						total += currElement->value * tempNode->value;
					}
					currElement = currElement->right;
				}
				if (total > 0) {
					temp.insert(total, currRow->key, i);
				}
				total = 0;
				currElement = currRow->toInternal;
			}
		}
		currRow = currRow->next;
	}

	return temp;
}

/*
  TODO: Run deconstructor through delete element until matrix is empty, then delete header nodes.
*/
OList::~OList()
{
}
