#include <string>
#include <vector>
#include <algorithm>

using namespace std;


template <typename Key>
class Hash
{
    public:
        size_t operator() (const Key& k) const;
};


template <typename HashedObj>
class HashTable
{
    public:
        HashTable(int size = 101);
        bool contains(const HashedObj& x) const;
        void makeEmpty();
        bool insert(const HashedObj& x);
        //bool insert(HashedObj&& x);
        bool remove(const HashedObj& x);
        HashedObj * getObj(const HashedObj & x);
        vector<vector<HashedObj>> * getLists();


    private:
        int size;
        vector<vector<HashedObj>> theLists;
        int currentSize;
        size_t myhash(const HashedObj& x) const;
};



//constructor
template <typename HashedObj>
HashTable<HashedObj>::HashTable( int newSize ) :size(newSize) , currentSize(0){
    theLists = vector<vector<HashedObj>>(size);
}

// to access all lists
template <typename HashedObj>
 vector<vector<HashedObj>> * HashTable<HashedObj>::getLists(){
     return & theLists;
 }

template <typename HashedObj>
size_t HashTable<HashedObj>::myhash( const HashedObj & x ) const
{
    static Hash<HashedObj> hf;
    return hf( x ) % (theLists.size());
}

template <typename HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
    for (auto& thisList : theLists)
        thisList.clear();
}

template <typename HashedObj>
bool HashTable<HashedObj>::contains(const HashedObj& x) const
{
    auto& whichList = theLists[myhash(x)];
    return find(whichList.begin(), whichList.end(), x) != whichList.end();
}

template <typename HashedObj>
bool HashTable<HashedObj>::remove(const HashedObj& x)
{
    auto& whichList = theLists[myhash(x)];
    auto itr = find(whichList.begin(), whichList.end(), x);

    if (itr == whichList.end())
        return false;

    whichList.erase(whichList.begin() + itr);
    --currentSize;
    return true;
}

template <typename HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj& x)
{
    auto& whichList = theLists[myhash(x)];
    if (find(whichList.begin(), whichList.end(), x) != whichList.end())
        return false;

    whichList.push_back(x);
    ++currentSize;
    return true;
}

// find method for hash table. Returns a pointer to item.
template <typename HashedObj>
HashedObj * HashTable<HashedObj>::getObj(const HashedObj& x) 
{
    auto& whichList = theLists[myhash(x)];
    auto itr = find(whichList.begin(), whichList.end(), x);

    if (itr == whichList.end())
        return nullptr;

    HashedObj * temp;
    temp = & (whichList[itr - whichList.begin()]);
    return temp;
}