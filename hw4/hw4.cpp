
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <chrono>
#include "strutils.h"


using namespace std;

// Keeps each line in a record of name, surname, phone and city.
struct PhoneRecord
{
	string fname;
	string lname;
	string number;
	string city;
	string fullname;

	PhoneRecord(){
		string fname = "";   
		string lname = "";
		string number = "";
		string city = "";
		fullname = "";
	}

	PhoneRecord(const string fn, const string ln, const string num,  const string ci):
	fname(fn), lname(ln), number(num), city(ci), fullname("")
	{
		fullname.append(fname);
		fullname.append(lname);
	};

	int findPartial(PhoneRecord& rhs){
		return (fullname.find(rhs.fullname));
	}

	
};

// Necessary methods to make PhoneRecord comparable.

bool operator < (PhoneRecord & lhs, PhoneRecord& rhs) {
	return lhs.fullname < rhs.fullname;
}

bool operator > (PhoneRecord & lhs, PhoneRecord& rhs) {
	return lhs.fullname > rhs.fullname;
}


bool operator == (PhoneRecord & lhs, PhoneRecord& rhs) {
	return lhs.fullname.compare(rhs.fullname) == 0;
} 

ostream& operator << (ostream& os, PhoneRecord& record) {
	os << UpperString(record.fname) << " " << UpperString(record.lname) << " "  << record.number << " " << record.city;
	return os;
}


// Sort Function Declerations.
template <typename Comparable>
void quickSort(vector<Comparable> & a);

template <typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right);

template <typename Comparable>
Comparable & medianOfThree(vector<Comparable> & a, int left, int right);


template <typename Comparable>
inline void swapTwo(Comparable & obj1, Comparable & obj2);

template <typename Comparable>
void insertionSort(vector<Comparable> & a);

template <typename Comparable>
void insertionSort(vector<Comparable> & a, int left, int right);

template<typename Comparable>
void heapSort(vector<Comparable> & a);

template<typename Comparable>
void percDown(vector<Comparable> & a, int i, int n);

template<typename Comparable>
void mergeSort(vector<Comparable> & a);

template<typename Comparable>
void mergeSort(vector<Comparable> & a, int left, int right);

template<typename Comparable>
void merge(vector<Comparable> & a, int leftPos, int rightPos, int leftEnd);

// Search Functions
template<typename Comparable>
int binarySearch(vector<Comparable> & a, Comparable & key);

template<typename Comparable>
int binarySearch(vector<Comparable> & a,  Comparable & key, int left, int right);

template<typename Comparable>
int binarySearchFull(vector<Comparable> & a,  Comparable & key, int left, int right);

template<typename Comparable>
int binarySearchPartial(vector<Comparable> & a,  Comparable & key, int left, int right);

template<typename Comparable>
int sequentialSearchFull(vector<Comparable> & a,  Comparable & key);

template<typename Comparable>
int sequentialSearchPartial(vector<Comparable> & a,  Comparable & key);


// Function Implementations

// Quick Sort
template <typename Comparable>
void quickSort(vector<Comparable> & a){
    quickSort(a, 0 , a.size()-1);
}


template <typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right){

    if(left + 10 <= right){
        Comparable & pivot = medianOfThree(a, left, right);
        int i = left;
        int j = right-1;

        for(;;){
            while(a[++i] < pivot){}
            while(a[--j] > pivot){}

            if(i < j){
                swapTwo(a[i], a[j]);
            }else{
                break;
            }
        }

        swapTwo(a[i], a[right-1]);

        quickSort(a, left, i-1);
        quickSort(a, i + 1, right);

    }
    else{
        insertionSort(a,left,right);
    }
}


// Takes median of first, last and center element. 
template <typename Comparable>
Comparable & medianOfThree(vector<Comparable> & a, int left, int right){

    int center = (left + right) / 2;

    if(a[left] > a[center])
        swapTwo(a[left], a[center]);

    if(a[left] > a[right])
         swapTwo(a[left], a[right]);
    
    if(a[center] > a[right])
        swapTwo(a[center], a[right]);

    // place pivot to right-1.
    swapTwo(a[center], a[right-1]);
    return a[right-1];
}


// Helper function two swap two array element.
template <typename Comparable>
inline void swapTwo(Comparable & obj1, Comparable & obj2){
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}



// Insertion Sort
template <typename Comparable>
void insertionSort(vector<Comparable> & a){	
	insertionSort(a,0,a.size()-1);
}


template <typename Comparable>
void insertionSort(vector<Comparable> & a, int left, int right){
    
    for (int i = left + 1; i < right + 1; i++)
    {
        Comparable tmp = a[i];
        int j;
        for (j = i; j > left && (tmp < a[j-1]); j--)
        {
                a[j] = a[j-1];
        }
        a[j] = tmp;
    }
}


// Heap Sort
template<typename Comparable>
void heapSort(vector<Comparable> & a){

	for (int i = a.size() / 2 ; i >= 0; i--)	{
		percDown(a,i, a.size());	
	}
	
	for (int i = a.size()-1; i > 0; i--){
		swapTwo(a[i], a[0]);
		percDown(a,0, i);		
	}
	
}

template<typename Comparable>
void percDown(vector<Comparable> & a, int i, int n){

	int child;
	Comparable tmp;

	for (tmp = a[i]; i *2 + 1 < n; i = child){
		child = i * 2 + 1;
		if(child != n -1 && a[child] < a[child + 1]){
			child ++;
		}
		if(tmp < a[child]){
			a[i] = a[child];
		}else{
			break;
		}
	}
	a[i] = tmp;
	
}

// Merge Sort
template<typename Comparable>
void mergeSort(vector<Comparable> & a){
	mergeSort(a,0, a.size()-1);
}

template<typename Comparable>
void mergeSort(vector<Comparable> & a, int left, int right){
	if(left < right){
		int center = (left + right) / 2;
		mergeSort(a,left, center);
		mergeSort(a,center+1,right);
		merge(a, left, center+1, right);
	}
	
}


template<typename Comparable>
void merge(vector<Comparable> & a, int leftPos, int rightPos, int rightEnd){
	int gap = rightEnd - leftPos + 1;

	for(gap = gap / 2; gap > 0; gap = gap/2){

        for(int i = leftPos; i + gap <= rightEnd; i++){
            int j = i + gap;
            if (a[i] > a[j])
                swapTwo( a[i], a[j]);
        }
    }
}


// Binary Search For Full Names (i.e.:John Doe)

template<typename Comparable>
int binarySearchFull(vector<Comparable> & a,  Comparable & key, int left, int right){
	if(left <= right){

		int mid = (left +right) / 2;

		if(a[mid] == key)
			return mid;

		if(a[mid] > key)
			return binarySearchFull(a,key,left,mid-1);
		
		return binarySearchFull(a,key, mid+1, right);
	}

	return -1;

}

// Binary Search For Partial Names.
// Returns the smallest(very first) index of partial name.
template<typename Comparable>
int binarySearchPartial(vector<Comparable> & a, Comparable & key, int left, int right){
	if(left <= right){

		int mid = (left + right) / 2;

		if(a[mid].findPartial(key) == 0 ){
			int firstindex = mid;
			int temp = binarySearchPartial(a,key,left,mid-1);
			if(temp > 0)
				return temp;

			return firstindex;

		}
			
		if(a[mid] > key)
			return binarySearchPartial(a,key,left,mid-1);
		
		return binarySearchPartial(a,key, mid+1, right);
	}

	return -1;

}


// Sequential Search
template<typename Comparable>
int sequentialSearchFull(vector<Comparable> & a,  Comparable & key){
	for (int i = 0; i < a.size(); i++){
		if(a[i] == key)
			return i;
	}
	return -1;
	
}

template<typename Comparable>
int sequentialSearchPartial(vector<Comparable> & a,  Comparable & key){
	for (int i = 0; i < a.size(); i++){
		if(a[i].findPartial(key) == 0)
			return i;
	}
	return -1;
	
}


// Main Program

bool takeInput(vector<PhoneRecord> &, string );

//Method definitions are below.

// takes phone records from input file and insert into a vector.
bool takeInput(vector<PhoneRecord>& list, string filename){
	ifstream input;
	string line;

	input.open(filename.c_str());

	if(input.fail()){
		std::cout << "Could not open the file." << endl;
		return false;
	}else {
		while(getline(input,line)){
			// some files can have empty lines, maybe at the end. That if block prevents that confusion.
			if(line.length() > 0){ 
				istringstream stringStream(line);
				string fname;
				string lname;
				string number;
				string city;

				stringStream >> fname >> lname >> number >> city;
				
				list.push_back(PhoneRecord(fname,lname,number, city));
			}
		}
		return true;
	}
}


int main(){

	string filename, searched;
	std::cout << "Please enter the filename:" << endl ;
	cin >> filename;
	
	std::cout << "Please enter the word to be queried :" << endl;
	cin >> ws; // to be able to getline.
	
	getline(cin,searched);

	istringstream sstr(searched);

	// chekcs for full or partial name.
	string fullName;
	string val;
	int wordCount = 0;
	while(sstr >> val){
		fullName.append(val);
		wordCount++;
	}

	// Record to be searced with given name.
	PhoneRecord record;
	record.fullname = fullName;


	// Vectors for each sort algorithm.
	vector<PhoneRecord> quickVector;
	vector<PhoneRecord> insertionVector;
	vector<PhoneRecord> mergeVector;
	vector<PhoneRecord> heapVector;

	takeInput(quickVector,filename);
	takeInput(insertionVector,filename);
	takeInput(mergeVector,filename);
	takeInput(heapVector,filename);


	cout << endl << "Sorting the vector copies" << endl ;
	cout << "======================================" << endl;

	auto start1 = chrono::high_resolution_clock::now();
	quickSort<PhoneRecord>(quickVector);
	auto end1 = chrono::high_resolution_clock::now();
	long long quickTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();
	std::cout << "Quick Sort Time: " << quickTime<< " Nanoseconds" << endl;


	auto start2 = chrono::high_resolution_clock::now();
	insertionSort<PhoneRecord>(insertionVector);
	auto end2 = chrono::high_resolution_clock::now();
	long long insertionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
	std::cout << "Insertion Sort Time: " << insertionTime << " Nanoseconds" << endl;


	auto start3 = chrono::high_resolution_clock::now();
	mergeSort<PhoneRecord>(mergeVector);
	auto end3 = chrono::high_resolution_clock::now();
	long long mergeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - start3).count();
	std::cout << "Merge Sort Time: " << mergeTime << " Nanoseconds" << endl;

	
	auto start4 = chrono::high_resolution_clock::now();
	heapSort<PhoneRecord>(heapVector);
	auto end4 = chrono::high_resolution_clock::now();
	long long heapTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end4- start4).count();
	std::cout << "Heap Sort Time: " << heapTime << " Nanoseconds" << endl;



	cout << endl;
	cout << "Searching for " << searched << endl ;
	cout << "======================================" << endl;


	int N = 1000; // Searching N times to get avarage search time.

	auto start5 = chrono::high_resolution_clock::now();

	int firstIndex;
	for (int i = 0; i < N; i++)
	{
		if(wordCount == 2)
			firstIndex = binarySearchFull<PhoneRecord>(insertionVector,record, 0, quickVector.size()-1);
		else
			firstIndex = binarySearchPartial<PhoneRecord>(insertionVector,record, 0, quickVector.size()-1);
	}
	auto end5 = chrono::high_resolution_clock::now();
	long long binaryTime = chrono::duration_cast<chrono::nanoseconds>(end5- start5).count();

	cout << "Search results for Binary Search:" << endl;

	if(firstIndex < 0){
		cout << UpperString(searched) << " does NOT exist in the dataset" << endl << endl;
	}

	else if(wordCount == 2){
		cout << insertionVector[firstIndex] << endl << endl;
	}
	else{
		// Since we have first index, we find next partial names also.
		for (int i = firstIndex; i < insertionVector.size(); i++)
		{
			if(insertionVector[i].findPartial(record) == 0)
				cout << insertionVector[i] << endl;
			else
				break;
		}
		cout << endl;
	}
	
	cout << "Binary Search Time: " << binaryTime / N << " Nanoseconds" << endl << endl;




	auto start6 = chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++)
	{
		if(wordCount == 2)
			firstIndex = sequentialSearchFull<PhoneRecord>(insertionVector,record);
		else
			firstIndex = sequentialSearchPartial<PhoneRecord>(insertionVector,record);
	}
	auto end6 = chrono::high_resolution_clock::now();
	long long  sequentialTime = chrono::duration_cast<chrono::nanoseconds>(end6- start6).count() ;

	cout << "Search results for Sequential Search:" << endl;
	if(firstIndex < 0){
		cout << UpperString(searched) << " does NOT exist in the dataset" << endl << endl;
	}

	else if(wordCount == 2){
		cout << insertionVector[firstIndex] << endl << endl;
	}
	else{
		for (int i = firstIndex; i < insertionVector.size(); i++)
		{
			if(insertionVector[i].findPartial(record) == 0)
				cout << insertionVector[i] << endl;
			else
				break;
		}
		cout << endl;
		
	}
	cout << "Sequential Search Time: " << sequentialTime / N << " Nanoseconds" << endl << endl;

	
	// Speed Up results.

	cout << "SpeedUp between Search Algorithms" << endl ;
	cout << "======================================" << endl;
	cout << "(Sequential Search/ Binary Search) SpeedUp = " << (double)sequentialTime / binaryTime << endl;

	cout << endl;

	cout << "SpeedUps between Sorting Algorithms" << endl ;
	cout << "======================================" << endl;
	cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << (double)insertionTime / quickTime  << endl;
	cout << "(Merge Sort / Quick Sort) SpeedUp = " << (double)mergeTime / quickTime << endl;
	cout << "(Heap Sort / Quick Sort) SpeedUp = " << (double)heapTime / quickTime  << endl;


	return 0;
}
