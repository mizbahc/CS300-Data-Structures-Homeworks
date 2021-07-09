// By Mizbah Celik - 27865
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string>
#include <iomanip>

#include "HashTable.cpp"


using namespace std;


// Class which is hashable with appropriate attributes.
class Show{
	public:
		Show(vector<string> nameslist): names(nameslist), count(1),support(0){
			std::sort(names.begin(), names.end());
		}
		

		string getName() const{
			//std::sort(names.begin(), names.end());
			string result;
			for(string s: names)
				result.append(s) ;
			return result;
		}

		vector<string> & getAllNames(){
			return names;
		}

		const double getCount() const{
			return count;
		}

		const double getSupport() const{
			return support;
		}

		void setSupport(double val){
			support = val;
		}

		void setCount(int val){
			count = val;
		}

		void increaseCountByOne(){
			count = count + 1;
		}

		bool operator ==(const Show& rhs) const{
			if(rhs.names.size() != names.size())
				return false;
			for(unsigned int i= 0; i < names.size(); i++){
				if(rhs.names[i].compare( names[i]) != 0)
					return false;
			}

			return true;
		}

		bool operator !=( const Show &rhs) const {
			return !(*this == rhs);
		}


	private:
		vector<string> names; // since we can insert show pairs
		int count;
		double support;
};



template<>
class Hash<string>
{
	public:
	size_t operator()( const string & key )
	{
		size_t hashVal = 0;
		for( char ch : key )
			hashVal = 37 * hashVal + ch;
		return hashVal;
	}
};

// Function object to be used as has funciton for our Show Class.
template<>
class Hash<Show>{
	public:
		size_t operator()(const Show & item){
			static Hash<string> hf;
			return hf(item.getName());
		}
};


/*
Method declarations without body are below.
You can find more information on method definitions.
*/

bool takeInput(HashTable<Show> & table,vector<vector<string>> & wholeList, string & filename, double &numOfLines);
void createSupportedTable(HashTable<Show> & table, HashTable<Show> &supportedTable, double lineNum, double supThreshold,vector<vector<string>> & wholeList);
void calculateConfidence(HashTable<Show> &supportedTable, double lineNum, double confThreshold, vector<vector<string>> &wholeList);


//Method definitions are below.

// takes shows from input file and insert into hashtable.
bool takeInput(HashTable<Show> & table, vector<vector<string>> & wholeList, string & filename, double &numOfLines){
	ifstream input;
	numOfLines = 0; // keeping number of lines to use later.
	string line;

	input.open(filename.c_str());

	if(input.fail()){
		cout << "Could not open the file." << endl;
		return false;
	}else {
		while(getline(input,line)){
			// some files can have empty lines, maybe at the end. That if block prevents that confusion.
			if(line.length() > 0){ 
				istringstream stringStream(line);
			
				wholeList.push_back(vector<string>()); // adds an empty row to our matrix for each line.
				string value;
				// taking each value in value variable which is a string.
				while(stringStream >> value ){
					// creating a Show object with name.
					vector<string> names;
					names.push_back(value);
					Show tmp(names);
					
					// if it is already in the hashtable, increase count by one.
					// Otherwise just insert. 
					if(!table.insert(tmp)){
						Show * item = table.getObj(tmp);
						item->increaseCountByOne();
					}

					// Adding to vector to use later without re-reading file.
					int row = wholeList.size()-1; // our current row is the last row.
					(wholeList[row]).push_back(value);
				}
				numOfLines++;
			}
		}
		return true;
	}
}


void createSupportedTable(HashTable<Show> & table, HashTable<Show> & supportedTable, double lineNum, double supThreshold, vector<vector<string>> &wholeList){

	vector<vector<Show>> * list = table.getLists();
	vector<string> singles;

	for (unsigned int i = 0; i < list->size() ; i++){
		
		for (unsigned int j = 0; j < (*list)[i].size() ; j++){
			Show temp = (*list)[i][j];
			double sup = temp.getCount() / lineNum;
			if(sup >= supThreshold){
				temp.setSupport(sup);
				supportedTable.insert(temp);
				singles.push_back(temp.getName());
			}
		}
	}

	for (unsigned int i = 0; i < singles.size() ; i++){
		string first = singles[i];
		for (unsigned int j = 0; j < singles.size() ; j++){
			if(i == j)
				continue;
			
			string second = singles[j];
			int count = 0;

			for (unsigned int i = 0; i < wholeList.size(); i++){
				vector<string> line = wholeList[i];
				
				if( (find(line.begin(), line.end(), first) != line.end()) && (find(line.begin(), line.end(), second) != line.end())  ){
					count++;
				}
			}
			
			double sup2 = count / lineNum;
			if(sup2 >= supThreshold){
				vector<string> names;
				names.push_back(first);
				names.push_back(second);
				Show show(names);
				show.setSupport(sup2);
				show.setCount(count);
				supportedTable.insert(show);
			}
		}
	}


}

void calculateConfidence(HashTable<Show> &supportedTable, double lineNum, double confThreshold, vector<vector<string>> &wholeList){

	vector<vector<Show>> * list = supportedTable.getLists();
	vector<Show> itemAndPairs;
	int totalResult = 0;

	ofstream out; // file stream to write to file.
	out.open("results.txt");

	for (unsigned int i = 0; i < list->size() ; i++){
		
		for (unsigned int j = 0; j < (*list)[i].size() ; j++){
			Show temp = (*list)[i][j];
			itemAndPairs.push_back(temp);
		}
	}

	for( unsigned int i = 0; i < itemAndPairs.size() ; i++){
		Show first = itemAndPairs[i];
		for( unsigned int j = 0; j < itemAndPairs.size() ; j++){
			if(i == j)
				continue;
			
			Show second = itemAndPairs[j];
			vector<string> firstNames = first.getAllNames();
			vector<string> secondNames = second.getAllNames();

			bool hasSameItem = true;
			for( unsigned int ii = 0; ii < firstNames.size(); ii++){
				for( unsigned int jj = 0; jj < secondNames.size() ; jj++){
					if(firstNames[ii].compare(secondNames[jj]) == 0){
						hasSameItem = false;
					}
				}
			}
			
			if(!hasSameItem){
				continue;
			}
		

			int count = 0;

			for( unsigned int ii = 0; ii < wholeList.size(); ii++){
				vector<string> line = wholeList[ii];
				vector<string> allNames;
				allNames.insert(allNames.end(),firstNames.begin(),firstNames.end());
				allNames.insert(allNames.end(),secondNames.begin(),secondNames.end());

				
				int found = 0;
				for (size_t k = 0; k < allNames.size(); k++)
				{
					if( find(line.begin(), line.end(), allNames[k]) != line.end())
						found++;
				}
				
				
				if(found == allNames.size())
					count++;
			}
			
			double sup2 = count / lineNum;

			double confidence = sup2 / first.getSupport();


			if(confidence >= confThreshold){
				totalResult++;
				out << firstNames[0];
				for( unsigned int ii = 1; ii < firstNames.size(); ii++)
				{
					out << "," << firstNames[ii];
				}

				out << ";";

				out << secondNames[0];
				for( unsigned int ii = 1; ii < secondNames.size(); ii++)
				{
					out << "," << secondNames[ii];
				}

				out << ";";
				out << fixed << setprecision(2) << confidence << endl;
				
			}
		}
	}
	if(totalResult > 0){
		cout << totalResult << " rules are written to results.txt" << endl;
	}else{
		cout << "There is no rule for the given support and confidence values." << endl;
	}

}



int main(){

	HashTable<Show> table = HashTable<Show>(); // HashTable keeping shows
	HashTable<Show> supportedTable; // HashTable keeping shows and pairs above threshold.
	vector<vector<string>> wholeList; // used for keeping file input. Not to read file again.

	double lineNum;
	string fileName;
	double supThreshold, confThreshold;

	// taking file names as input.
	cout << "Please enter the transaction file name: ";
	cin >> fileName;

	cout << "Please enter support and confidence values between 0 and 1: ";
	cin >> supThreshold;
	cin >> confThreshold;

	takeInput(table,wholeList,fileName,lineNum);

	createSupportedTable(table,supportedTable,lineNum,supThreshold, wholeList);

	calculateConfidence(supportedTable,lineNum,confThreshold, wholeList);


	return 0;
}