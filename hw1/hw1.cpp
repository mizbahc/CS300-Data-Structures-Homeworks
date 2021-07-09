

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <chrono>
#include <ctime>

#include "randgen.h"

#include "Stack.h"
#include "Stack.cpp"

using namespace std;

// stackNode that keeps pawns and their informations.
struct stackNode
{
	int value;
	int x_coordinate;
	int y_coordinate;
	bool visited;
	bool has_pawn;
	bool addedLast;

	stackNode(int x, int y, bool _visited, bool _has_pawn){
		value = 0;
		x_coordinate = x;
		y_coordinate = y;
		visited = _visited;
		has_pawn = _has_pawn;
		addedLast = false;
	}

};


/*
Method declarations without body are below.
You can find more information on method definitions.
*/

void takeBoard(vector<vector<stackNode*>> & matrix, int &N);
void takePawns(vector<vector<stackNode*>> & board, int& pawnNum, Stack<stackNode*> & pawnStack, int N);
bool rescueKing(vector<vector<stackNode * >> & board, Stack<stackNode * > & rescueStack, int N,stackNode * & knight,stackNode *& king);
void printMatrix(vector<vector<stackNode*>> & matrix);
vector<stackNode*> getAvailableNeighbours(const stackNode * current, int N,vector<vector<stackNode * >> board);
stackNode * isAvailable(int x, int y, vector<vector<stackNode * >> board);
bool isRowFull(int i, vector<vector<stackNode * >> board);
bool isColumnFull(int j, vector<vector<stackNode * >> board);
bool isDiagonalFull(int x, int y, vector<vector<stackNode * >> board);
vector<stackNode * > getValidNeighbours(const stackNode * current,vector<vector<stackNode * >> board);
stackNode* isValidPlace(int x, int y,vector<vector<stackNode * >> board);

//Method definitions are below.

// Creates a matrix of stackNode * with N dimension.
void takeBoard(vector<vector<stackNode *  >> & matrix, int &N)
{
	cout << "Please enter the size of the board (n): ";
	cin >> N;
	while (N < 4 || N > 11) // Asks user until correct size is entered.
	{
		cout << "Wrong size, Please enter size of board 4 to 11: ";
		cin >> N;
	}

	for (int i = 0; i < N; i++)
	{
		matrix.push_back(vector<stackNode*>());
		for (int j = 0; j < N; j++)
		{
			stackNode * temp = new stackNode(j + 1, N - i, false, false);
			matrix[i].push_back(temp);
		}
	}
}

// Takes at most 3* N pawns and puts them on board
// Starts from (1,1) coordinate for placing pawns.
void takePawns(vector<vector<stackNode * >> & board, int & pawnNum, Stack<stackNode * > & pawnStack, int N){
	cout << "Please enter the number of the pawns (p): ";
	cin >> pawnNum;

	while (pawnNum < 0 || pawnNum > 3 * N){
		cout << "Wrong! Please enter number of pawns at most " << 3 * N << " : ";
		cin >> pawnNum;
	}
	if (pawnNum == 0){
		return;
	}

	stackNode* startNode = board[N - 1][0]; // Node at (1,1) coordinate.
	startNode->has_pawn = true;
	startNode->visited = true;
	startNode->value = 1;
	pawnStack.push(startNode);

	int numOfPlaced = 1;
	srand((unsigned)time(0)); // To prevent rand() generating same sequence.
	//RandGen random ;
	while (numOfPlaced < pawnNum){
		stackNode* current = pawnStack.top(); // Getting last node a pawn placed to.
		vector<stackNode*> availablePlaces = getAvailableNeighbours(current,N, board);
		int len = availablePlaces.size();
		if (len == 0){ // if no neigbour is suitable for placing new pawn.
			if (current == startNode){
				// making old visited places available again to restart from start node and go a different direction.
				for (unsigned int i = 0; i < N; i++){
					for (unsigned int j = 0; j < N; j++){
						board[i][j]->addedLast = false;
					}
				}
			}
			else{
				stackNode* removed = pawnStack.topAndPop();
				removed->has_pawn = false;
				removed->visited = false;
				removed->value = 0;
				removed->addedLast = true;
				numOfPlaced--;
			}
		}
		else{
			int r = rand() % len; 
			//int r = random.RandInt(len);
			stackNode* selectedNode = availablePlaces[r]; // gets a random available neighbour for new pawn.

			selectedNode->has_pawn = true;
			selectedNode->visited = true;
			selectedNode->value = 1;
			pawnStack.push(selectedNode);
			numOfPlaced++;
		}
	}
	// Clears unnecessary flags remained from pawn placement.
	for (unsigned int i = 0; i < N; i++){
		for (unsigned int j = 0; j < N; j++){
			board[i][j]->addedLast = false;
			board[i][j]->visited = false;
		}
	}
}

// Returns true if knight can reach king. False otherwise.
bool rescueKing(vector<vector<stackNode * >> & board, Stack<stackNode * > & rescueStack, int N,stackNode * & knight,stackNode *& king){
	cout << "Enter the position of knight (x,y): ";
	int knight_x,knight_y;
	cin >> knight_x >> knight_y;

	while (!isValidPlace(knight_x,knight_y,board)){
		cout << "Wrong! Please enter a valid position of knight (x,y): ";
		cin >> knight_x >> knight_y;
	}

	stackNode * knight_node = board[N-knight_y][knight_x-1];
	rescueStack.push(knight_node); // adds knight node to stack keeping track of movements of it.
	knight = knight_node;
	knight_node->has_pawn = true;
	knight_node->visited = true;

	cout << "Enter the position of king (x,y): ";
	int king_x,king_y;
	cin >> king_x >> king_y;

	while (!isValidPlace(king_x,king_y,board)){
		cout << "Wrong! Please enter a valid position of king (x,y): ";
		cin >> king_x >> king_y;
	}
	stackNode * king_node = board[N-king_y][king_x-1];
	king = king_node;

	stackNode * lastRemoved = nullptr;

	stackNode* current = rescueStack.top();
	vector<stackNode*> validPlaces = getValidNeighbours(current, board);
	int len = validPlaces.size();

	bool found = false;

	// while starting position has available movements continues to move toward king
	while (len != 0 || current != knight_node){
		if (len == 0){
			stackNode* removed = rescueStack.topAndPop();
			removed->addedLast = true;
			current = rescueStack.top();// look for previous position it came from.
			validPlaces = getValidNeighbours(current, board);
			len = validPlaces.size();

		}
		else{
			int r = rand() % len;
			stackNode* selectedNode = validPlaces[r]; // moves randomly to an available place.
			selectedNode->visited = true;
			rescueStack.push(selectedNode);

			if(selectedNode == king_node){ // if reaches king, no more movements needed. breaks while loop
				found = true;
				break;
			}

			// since it didnot reach king, continues from current node's position.
			current = rescueStack.top();
			validPlaces = getValidNeighbours(current, board);
			len = validPlaces.size();
		}
	}
	return found;

}

// Taking a node as first parameter and returns a vector of available neighbours of it for pawn placement.
vector<stackNode * > getAvailableNeighbours(const stackNode * current, int N,vector<vector<stackNode * >> board)
{
	int x = current->x_coordinate;
	int y = current->y_coordinate;
	vector<stackNode*> result; // keeps avaiable nodes for new placement.

	// checks all neigbouring coordinates of current node.
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			x = x + i;
			y = y+ j;
			if (x > 0 && x < N+1 && y > 0 && y < N+1){ // if coordinate is on our board.
				stackNode* temp = isAvailable(x , y ,board);
				if (temp != nullptr)
					result.push_back(temp);
			}
			x = current->x_coordinate;
			y = current->y_coordinate;

		}
	}

	return result;
}

// returns node if it has no pawn already or not visited recently. 
// returns nullptr otherwise.
stackNode* isAvailable(int x, int y,vector<vector<stackNode * >> board)
{
	int N = board.size();
	int i = N - y;
	int j = x - 1;
	stackNode* current = board[i][j];

	// Using && instead of || for short-circuit evaluation.
	if (!(current->addedLast == false && current->has_pawn == false && current->visited == false))
		return nullptr;

	// checks if row colunn and diagonal has 3 or less pawns already.
	bool addable = !isRowFull(i,board) && !isColumnFull(j,board) && !isDiagonalFull(i, j,board);

	if (addable)
		return current;

	return nullptr;
}

// Checks row for maximum number of 3 pawns
bool isRowFull(int i,vector<vector<stackNode * >> board)
{
	int N = board.size();
	int total = 0;
	for (int k = 0; k < N; k++)
	{
		if (board[i][k]->has_pawn == true)
			total++;
	}
	if (total > 2)
		return true;
	return false;
}

// Checks column for maximum number of 3 pawns
bool isColumnFull(int j, vector<vector<stackNode * >> board)
{
	int N = board.size();
	int total = 0;
	for (int k = 0; k < N; k++)
	{
		if (board[k][j]->has_pawn == true)
			total++;
	}
	if (total > 2)
		return true;
	return false;
}

// Checks diagonal for maximum number of 3 pawns
bool isDiagonalFull(int x, int y, vector<vector<stackNode * >> board)
{
	int N = board.size();

	// First checks secondary diagonal
	int totalSecondary = 0;
	int i = x;
	int j = y;
	while (i >= 0 && i< N && j < N && j >= 0)
	{

		if (board[i][j]->has_pawn == true)
			totalSecondary++;
		i--;
		j++;
	}

	i = x;
	j = y;
	while (i >= 0 && i< N && j < N && j >= 0)
	{

		if (board[i][j]->has_pawn == true)
			totalSecondary++;

		i++;
		j--;
	}

	if (totalSecondary > 2)
		return true;


	// Checks principal diagonal.
	int totalPrincipal = 0;
	i = x;
	j = y;
	while (i >= 0 && i< N && j < N && j >= 0)
	{

		if (board[i][j]->has_pawn == true)
			totalPrincipal++;
		i--;
		j--;
	}

	i = x;
	j = y;
	while (i >= 0 && i< N && j < N && j >= 0)
	{

		if (board[i][j]->has_pawn == true)
			totalPrincipal++;

		i++;
		j++;
	}

	if (totalPrincipal > 2)
		return true;

	return false;
}

// Gets available nodes for knight's next move.
vector<stackNode * > getValidNeighbours(const stackNode * current,vector<vector<stackNode * >> board)
{
	int x = current->x_coordinate;
	int y = current->y_coordinate;
	vector<stackNode*> result;
	stackNode * temp = nullptr;

	temp = isValidPlace(x+1, y+2,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x+1, y-2,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x-1, y+2,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x-1, y-2,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x+2, y+1,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x+2, y-1,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x-2, y+1,board);
	if(temp != nullptr)
		result.push_back(temp);

	temp = isValidPlace(x-2, y-1,board);
	if(temp != nullptr)
		result.push_back(temp);

	return result;
}

// Returns node if it has no pawn and not visited before. 
// Returns nullptr if it is not available.
stackNode* isValidPlace(int x, int y,vector<vector<stackNode * >> board)
{
	int N = board[0].size();
	if (!(x > 0 && x < N+1 && y > 0 && y < N+1)){
		return nullptr;        
	}

	int i = N - y;
	int j = x - 1;
	stackNode* candidate = board[i][j];

	if (!(candidate->addedLast == false && candidate->has_pawn == false && candidate->visited == false))
		return nullptr;

	return candidate;
}
//Takes a 2D vector as its single parameter and prints like a matrix
void printMatrix(vector<vector<stackNode * >> & matrix)
{
	int N = matrix.size();
	for ( int i = 0; i < N; i++)
	{
		for ( int j = 0; j < N; j++)
		{
			cout << matrix[i][j]->value << " ";
		}
		cout << endl;
	}
	cout << endl;
}

int main(){
	auto start = chrono::high_resolution_clock::now();
	int N; // Size of board.
	vector<vector<stackNode * >> board;

	int pawnNum;
	Stack<stackNode * > pawnStack;

	Stack<stackNode * > rescueStack;
	stackNode * knight;
	stackNode * king;

	takeBoard(board, N);
	takePawns(board, pawnNum, pawnStack,N);
	auto end = chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	cout << duration << endl;

	cout << "The matrix:" << endl;
	printMatrix(board);

	bool found = rescueKing(board,rescueStack,N, knight,king);

	ofstream out; // file stream to write to file.
	out.open("output.txt");

	out << "N = " << N << endl;
	out << "P = " << pawnNum << endl;
	out << endl;
	out << "Pawn Locations\n";
	while(!pawnStack.isEmpty()){
		stackNode* temp = pawnStack.topAndPop();
		out << temp->x_coordinate << " " << temp->y_coordinate << endl;
	}

	out << endl;
	out << "Knight = " << knight->x_coordinate << " " << knight->y_coordinate << endl;
	out << "King = " << king->x_coordinate << " " << king->y_coordinate << endl;
	out << endl;

	if(found){
		cout << "Path found, see the output file!" <<  endl;
		Stack<stackNode * > outStack; // new stack to be able to show steps in correct order.
		while(!rescueStack.isEmpty()){
			outStack.push(rescueStack.topAndPop()); 
		}
		while(!outStack.isEmpty()){
			stackNode* temp = outStack.topAndPop();
			out << temp->x_coordinate << " " << temp->y_coordinate << endl;
		}
	}else{
		out << "No Way!" << endl;
		cout << "No Way!" << endl;
	}

	out << "\nMatrix" << endl;

	for ( int i = 0; i < N; i++)
	{
		for ( int j = 0; j < N; j++)
		{
			out << board[i][j]->value << " ";
		}
		out << endl;
	}

	out.close(); // closing output file stream.
	
	return 0;
}
