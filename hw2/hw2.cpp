

#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <chrono>
#include <climits>

#include "BinarySearchTree.cpp"
#include "AvlTree.cpp"


using namespace std;


struct ItemNode
{
	string title;
	string info;
	ItemNode(const string _title, string _info)
		: title(_title), info(_info) {
	}

};

//to be able to compare nodes with each other according to their titles.
bool operator < (const ItemNode& lhs, const ItemNode& rhs) {
	return lhs.title < rhs.title;
}


ostream& operator << (ostream& os, const ItemNode& item) {
	os << item.title;
	return os;
}


struct SectionNodeBST
{
	string title;
	BinarySearchTree<ItemNode>* itemTree;
	SectionNodeBST(const string _title, BinarySearchTree<ItemNode>* _itemTree)
		: title(_title), itemTree(_itemTree) {
	}
};

bool operator < (const SectionNodeBST& lhs, const SectionNodeBST& rhs) {
	return lhs.title < rhs.title;
}

ostream& operator << (ostream& os, const SectionNodeBST& node) {
	os << node.title;
	return os;
}

struct SectionNodeAVL
{ 
	string title;
	AvlTree<ItemNode>* itemTree;
	SectionNodeAVL(const string _title, AvlTree<ItemNode>* _itemTree)
		: title(_title), itemTree(_itemTree) {
	}
};

bool operator < (const SectionNodeAVL& lhs, const SectionNodeAVL& rhs) {
	return lhs.title < rhs.title;
}

ostream& operator << (ostream& os, const SectionNodeAVL& node) {
	os << node.title;
	return os;
}




void createNotebooks(BinarySearchTree<SectionNodeBST>& bst , AvlTree<SectionNodeAVL>& avl);
void processMenu(BinarySearchTree<SectionNodeBST>& bst, AvlTree<SectionNodeAVL>& avl);

void displaySectionsAVL(AvlTree<SectionNodeAVL>& avl);
void displaySectionsBST(BinarySearchTree<SectionNodeBST>& bst);
void selectSection(BinarySearchTree<SectionNodeBST>& bst, AvlTree<SectionNodeAVL>& avl);
void addSection(BinarySearchTree<SectionNodeBST>& bst, AvlTree<SectionNodeAVL>& avl);
void deleteSection(BinarySearchTree<SectionNodeBST>& bst, AvlTree<SectionNodeAVL>& avl);

void showSectionMenu(SectionNodeBST& bstSection , SectionNodeAVL & avlSection);

void displayItemsAVL(SectionNodeAVL & avlSection);
void displayItemsBST(SectionNodeBST& bstSection);
void displayItemInfo(SectionNodeBST& bstSection , SectionNodeAVL & avlSection);
void addItem(SectionNodeBST& bstSection , SectionNodeAVL & avlSection);
void updateItem(SectionNodeBST& bstSection , SectionNodeAVL & avlSection);
void deleteItem(SectionNodeBST& bstSection , SectionNodeAVL & avlSection);

void printMenu();
void printSectionMenu();

//Takes sections and their items from file and create two trees.
void createNotebooks(BinarySearchTree<SectionNodeBST>& bst , AvlTree<SectionNodeAVL>& avl) {

	ifstream input;
	string line;

	input.open("data.txt");

	if (input.fail()) {
		cout << "Could not open the file." << endl;
	}	
	else {
		getline(input, line);
		while (line.length() > 0) {
			// first forming AVL tree

			auto start = chrono::high_resolution_clock::now();
			const ItemNode NOT_FOUND = ItemNode("NoSuchItemFound_999", "NoSuchItem");
			AvlTree<ItemNode>* items = new AvlTree<ItemNode>(NOT_FOUND);

			string sectionName = line;
			long long pos = input.tellg();

			getline(input, line);
			do
			{
				int len = line.length();
				line = line.substr(1);
				int index = line.find("-");
				string title = line.substr(0, index);
				string info = line.substr(index + 1);

				items->insert(ItemNode(title, info));
				line = "";

			} while (getline(input, line) && line[0] == '-');

			avl.insert(SectionNodeAVL(sectionName, items));
			auto end = chrono::high_resolution_clock::now();
			long long timer = (end - start).count() / 1000.0;
			cout << endl;
			cout << "Section \"" << sectionName << "\" has been inserted into the AVL notebook." << endl;
			cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;


			/* forming BST tree  */

			const ItemNode NOT_FOUND2 = ItemNode("NoSuchItemFound_999", "NoSuchItem");
			BinarySearchTree<ItemNode>* items2 = new BinarySearchTree<ItemNode>(NOT_FOUND2);
			auto start2 = chrono::high_resolution_clock::now();
			input.clear();
			input.seekg(pos-1);
			getline(input, line);
			do
			{
				int len = line.length();
				line = line.substr(1);
				int index = line.find("-");
				string title = line.substr(0, index);
				string info = line.substr(index + 1);

				items2->insert(ItemNode(title, info));
				line = "";

			} while (getline(input, line) && line[0] == '-');

			bst.insert(SectionNodeBST(sectionName, items2));
			auto end2 = chrono::high_resolution_clock::now();
			long long timer2 = (end2 - start2).count() / 1000.0;
			cout << "Section \"" << sectionName << "\" has been inserted into the BST notebook." << endl;
			cout << "[BST] Elapsed time: " << timer2 << " microseconds" << endl;

		}
	}
}

// Takes input from user and shows related information according to it.
void processMenu(BinarySearchTree<SectionNodeBST>& bst , AvlTree<SectionNodeAVL>& avl) {

	printMenu();
	do {
		string option;
		int input;

		cout << "Input: ";
		getline(cin, option);
		if (option.length() != 1)
			input = 0;
		else if (!isdigit(option[0]))
			input = 0;
		else
			input = stoi(option);

		cout << endl;

		switch (input) {
		case 1:
			displaySectionsAVL(avl);
			break;
		case 2:
			displaySectionsBST(bst);
			break;
		case 3:
			selectSection(bst,avl);
			break;
		case 4:
			addSection(bst,avl);
			break;
		case 5:
			deleteSection(bst,avl);
			break;
		case 6:
			cout << "Terminating..." << endl;
			return;
		default:
			cout << "Invalid option: please enter again" << endl;
		}
	} while (true);
}


void displaySectionsAVL(AvlTree<SectionNodeAVL>& avl) {
	cout << endl << "*****\n\n";
	if(!avl.isEmpty())
		avl.printTree();
	cout << "*****\n\n";
}

void displaySectionsBST(BinarySearchTree<SectionNodeBST>& bst) {
	cout << endl << "*****\n\n";
	if(!bst.isEmpty())
		bst.printTree();
	cout << "*****\n\n";
}

void selectSection(BinarySearchTree<SectionNodeBST>& bst , AvlTree<SectionNodeAVL>& avl) {
	cout << "Enter the title of the section: ";
	string sectionName;
	getline(cin,sectionName);
	//cin >> sectionName;
	SectionNodeBST bstSection = bst.find(SectionNodeBST(sectionName, NULL));
	SectionNodeAVL avlSection = avl.find(SectionNodeAVL(sectionName,NULL));
	if (bstSection.title == "NoSuchSectionAvailable_999") {
		cout << "Invalid title!\n\n" << endl;
	}	
	else {
		showSectionMenu(bstSection,avlSection);

		//showSectionMenu(bstSection.title,bstSection.itemTree);
	}

}


void showSectionMenu(SectionNodeBST& bstSection , SectionNodeAVL & avlSection) {
	cout << endl;
	cout << "Selected section -> " << bstSection.title << endl;
	printSectionMenu();
	do {
		string option;
		int input;

		cout << "Input: ";
		getline(cin,option);
		//cin >> option;
		if (option.length() != 1)
			input = 0;
		else if (!isdigit(option[0]))
			input = 0;
		else
			input = stoi(option);

		//cout << endl;

		switch (input) {
		case 1:
			displayItemsAVL(avlSection);
			break;
		case 2:
			displayItemsBST(bstSection);
			break;
		case 3:
			displayItemInfo(bstSection,avlSection);
			break;
		case 4:
			addItem(bstSection,avlSection);
			break;
		case 5:
			updateItem(bstSection,avlSection);
			break;
		case 6:
			deleteItem(bstSection,avlSection);
			break;
		case 7:
			printMenu();
			return;
		default:


			cout << "Invalid option: please enter again" << endl;
		}
	} while (true);


}


void printSectionMenu() {

	cout << "Please enter an input between [1 - 7]:" << endl;;
	cout << "1- Display the items [AVL]" << endl;
	cout << "2- Display the items [BST]" << endl;
	cout << "3- Display the information of a item" << endl;
	cout << "4- Add new item" << endl;
	cout << "5- Update the information of a item" << endl;
	cout << "6- Delete an item" << endl;
	cout << "7- Return to main menu" << endl;

}

void displayItemsAVL(SectionNodeAVL & avlSection) {
	cout << endl << "*****\n\n";
	if(!avlSection.itemTree->isEmpty())
		avlSection.itemTree->printTree();
	cout << "*****\n\n";
}

void displayItemsBST(SectionNodeBST& bstSection) {
	cout << endl << "*****\n\n";
	if(!bstSection.itemTree->isEmpty())
		bstSection.itemTree->printTree();
	cout << "*****\n\n";
}

void displayItemInfo(SectionNodeBST& bstSection, SectionNodeAVL & avlSection) {
	cout << "Enter the title of the item: ";
	string itemName;
	//cin >> itemName;
	getline(cin,itemName);

	auto startbstq = chrono::high_resolution_clock::now();
	ItemNode itembst = bstSection.itemTree->find(ItemNode(itemName, ""));
	auto endbstq = chrono::high_resolution_clock::now();
	long long timerbstq = (endbstq - startbstq).count() / 1000.0;

	auto startavlq = chrono::high_resolution_clock::now();
	ItemNode itemavl = avlSection.itemTree->find(ItemNode(itemName, ""));
	auto endavlq = chrono::high_resolution_clock::now();
	long long timeravlq = (endavlq - startavlq).count() / 1000.0;


	if (itembst.title == "NoSuchItemFound_999") {
		cout << "[AVL] Elapsed time: " << timeravlq << " microseconds" << endl;
		cout << "[BST] Elapsed time: " << timerbstq << " microseconds" << endl;

		cout << "Invalid title.\n\n" << endl;
	}	
	else {
		cout << "[AVL] Elapsed time: " << timeravlq << " microseconds" << endl;
		cout << "[BST] Elapsed time: " << timerbstq << " microseconds" << endl;

		cout << itembst.info << endl << endl;
	}


}



void addItem(SectionNodeBST& bstSection, SectionNodeAVL & avlSection) {
	cout << "Enter a title for the item: ";
	string itemName;
	getline(cin, itemName);

	auto startbstq = chrono::high_resolution_clock::now();
	
	ItemNode item = bstSection.itemTree->find(ItemNode(itemName, ""));
	
	auto endbstq = chrono::high_resolution_clock::now();
	long long timerbstq = (endbstq - startbstq).count() / 1000.0;

	auto startavlq = chrono::high_resolution_clock::now();
	ItemNode item2 = avlSection.itemTree->find(ItemNode(itemName, ""));
	auto endavlq = chrono::high_resolution_clock::now();
	long long timeravlq = (endavlq - startavlq).count() / 1000.0;

	if (item.title != "NoSuchItemFound_999") {
		cout << "Item \"" << itemName << "\" already exists in the \"" << bstSection.title << "\".\n\n" << endl;
	}
	else {
		string itemInfo;
		cout << "Enter a description for the item: ";
		getline(cin,itemInfo);

		auto startbstins = chrono::high_resolution_clock::now();
		bstSection.itemTree->insert(ItemNode(itemName, itemInfo));
		auto endbstins = chrono::high_resolution_clock::now();
		long long timerbstins = (endbstins - startbstins).count() / 1000.0;

		auto startavlins = chrono::high_resolution_clock::now();
		avlSection.itemTree->insert(ItemNode(itemName, itemInfo));
		auto endavlins = chrono::high_resolution_clock::now();
		long long timeravlins = (endavlins - startavlins).count() / 1000.0;

		cout << "[AVL] Elapsed time: " << timeravlq + timeravlins << " microseconds" << endl;
		cout << "[BST] Elapsed time: " << timerbstq + timerbstins << " microseconds" << endl;

		cout << "The new item \"" << itemName << "\" has been inserted.\n\n" << endl;
	}


}

void updateItem(SectionNodeBST & bstSection, SectionNodeAVL & avlSection) {
	cout << "Enter the title of the item: ";
	string itemName;
	getline(cin, itemName);

	auto startbstq = chrono::high_resolution_clock::now();
	ItemNode item = bstSection.itemTree->find(ItemNode(itemName, ""));
	auto endbstq = chrono::high_resolution_clock::now();
	long long timerbstq = (endbstq - startbstq).count() / 1000.0;

	auto startavlq = chrono::high_resolution_clock::now();
	ItemNode item2 = avlSection.itemTree->find(ItemNode(itemName, ""));
	auto endavlq = chrono::high_resolution_clock::now();
	long long timeravlq = (endavlq - startavlq).count() / 1000.0;

	cout << "[AVL] Elapsed time: " << timeravlq << " microseconds" << endl;
	cout << "[BST] Elapsed time: " << timerbstq << " microseconds" << endl;


	if (item.title == "NoSuchItemFound_999") {
		cout << "Item \"" << itemName << "\" does not exist in the \"" << bstSection.title << "\".\n\n" << endl;
	}
	else {

		string itemInfo;
		cout << "Enter the new information: ";
		getline(cin, itemInfo);

		//first removing old item to add updated one.
		bstSection.itemTree->remove(item);
		avlSection.itemTree->remove(item2);

		bstSection.itemTree->insert(ItemNode(itemName, itemInfo));
		avlSection.itemTree->insert(ItemNode(itemName, itemInfo));

		cout << "The content " << itemName << " has updated.\n\n" << endl;
	}
}




void deleteItem(SectionNodeBST& bstSection, SectionNodeAVL & avlSection) {
	cout << "Enter the title of the item: ";
	string itemName;
	getline(cin, itemName);

	auto startbstq = chrono::high_resolution_clock::now();
	ItemNode item = bstSection.itemTree->find(ItemNode(itemName, ""));
	auto endbstq = chrono::high_resolution_clock::now();
	long long timerbstq = (endbstq - startbstq).count() / 1000.0;


	auto startavlq = chrono::high_resolution_clock::now();
	ItemNode item2 = avlSection.itemTree->find(ItemNode(itemName, ""));
	auto endavlq = chrono::high_resolution_clock::now();
	long long timeravlq = (endavlq - startavlq).count() / 1000.0;


	if (item.title == "NoSuchItemFound_999") {
		cout << "Item \"" << itemName << "\" does not exist in the \"" << bstSection.title << "\".\n\n" << endl;
	}
	else {
		auto startbstins = chrono::high_resolution_clock::now();
		bstSection.itemTree->remove(item);
		auto endbstins = chrono::high_resolution_clock::now();
		long long timerbstins = (endbstins - startbstins).count() / 1000.0;

		auto startavlins = chrono::high_resolution_clock::now();
		avlSection.itemTree->remove(item2);
		auto endavlins = chrono::high_resolution_clock::now();
		long long timeravlins = (endavlins - startavlins).count() / 1000.0;

		cout << "[AVL] Elapsed time: " << timeravlq + timeravlins << " microseconds" << endl;
		cout << "[BST] Elapsed time: " << timerbstq + timerbstins << " microseconds" << endl;
		cout << "The item \"" << itemName << "\" has been deleted.\n\n" << endl;
	}
}

void addSection(BinarySearchTree<SectionNodeBST>& bst, AvlTree<SectionNodeAVL>& avl) {
	cout << "Enter a title for the section: ";
	string sectionName;
	getline(cin, sectionName);
	SectionNodeBST bstSection = bst.find(SectionNodeBST(sectionName, NULL));
	if (bstSection.title != "NoSuchSectionAvailable_999") {
		cout << "Section \"" << sectionName << "\" already exists.\n\n" << endl;
	}
	else {
		const ItemNode NOT_FOUND = ItemNode("NoSuchItemFound_999", "NoSuchItem");
		AvlTree<ItemNode>* items = new AvlTree<ItemNode>(NOT_FOUND);

		avl.insert(SectionNodeAVL(sectionName, items));

		const ItemNode NOT_FOUND2 = ItemNode("NoSuchItemFound_999", "NoSuchItem");
		BinarySearchTree<ItemNode>* items2 = new BinarySearchTree<ItemNode>(NOT_FOUND2);

		bst.insert(SectionNodeBST(sectionName, items2));
		cout << "The new section \"" << sectionName << "\" has been inserted.\n\n" << endl;

	}
}

void deleteSection(BinarySearchTree<SectionNodeBST>& bst , AvlTree<SectionNodeAVL>& avl) {
	cout << "Enter the title of the section: ";
	string sectionName;
	getline(cin, sectionName);
	SectionNodeBST bstSection = bst.find(SectionNodeBST(sectionName, NULL));
	if (bstSection.title == "NoSuchSectionAvailable_999") {
		cout << "Section \"" << sectionName << "\" does not exist.\n\n" << endl;
	}
	else {
		avl.remove(SectionNodeAVL(sectionName,NULL));
		bst.remove(bstSection);
		cout << "The section has been deleted.\n" << endl;
	}
}


void printMenu() {
	cout << endl;
	cout << "MENU" << endl;
	cout << "Please enter an input between [1 - 6]:" << endl;;
	cout << "1- Display the sections [AVL]" << endl;
	cout << "2- Display the sections [BST]" << endl;
	cout << "3- Select a section" << endl;
	cout << "4- Add new section" << endl;
	cout << "5- Delete a section" << endl;
	cout << "6- Exit" << endl;

}

int main() {

	// creating empty bst.
	const SectionNodeBST ITEM_NOT_FOUND_BST = SectionNodeBST("NoSuchSectionAvailable_999",NULL);
	BinarySearchTree< SectionNodeBST > bst(ITEM_NOT_FOUND_BST); 

	// creating empty bst.
	const SectionNodeAVL ITEM_NOT_FOUND_AVL = SectionNodeAVL("NoSuchSectionAvailable_999",NULL);
	AvlTree< SectionNodeAVL > avl(ITEM_NOT_FOUND_AVL);

	cout << "Welcome to the the Notes" << endl;
	createNotebooks(bst, avl); // fills empty trees with information from file.
	processMenu(bst,avl); // interacting with user until termination.

	return 0;

}
