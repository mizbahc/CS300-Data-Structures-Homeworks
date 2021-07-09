#include <string>
#include <iostream>

using namespace std;

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
    Comparable 	element;
    AvlNode* left;
    AvlNode* right;
    int        		height;

    AvlNode(const Comparable& theElement,
        AvlNode* lt, AvlNode* rt, int h = 0)
        : element(theElement), left(lt), right(rt), height(h) { }

    friend class AvlTree<Comparable>;
};


template <class Comparable>
class AvlTree
{
public:
    explicit AvlTree(const Comparable& notFound);
    AvlTree(const AvlTree& rhs);
    ~AvlTree();

    const Comparable& findMin() const;
    const Comparable& findMax() const;
    const Comparable& find(const Comparable& x) const;
    bool isEmpty() const;
    void printTree() const;

    void makeEmpty();
    void insert(const Comparable& x);
    void remove(const Comparable& x);

    const AvlTree& operator=(const AvlTree& rhs);
private:
    AvlNode<Comparable>* root;
    const Comparable ITEM_NOT_FOUND;

    const Comparable& elementAt(AvlNode<Comparable>* t) const;

    void insert(const Comparable& x, AvlNode<Comparable>*& t) const;
    void remove(const Comparable& x, AvlNode<Comparable>*& t) const;

    AvlNode<Comparable>* findMin(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* findMax(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* find(const Comparable& x, AvlNode<Comparable>* t)
        const;
    void makeEmpty(AvlNode<Comparable>*& t) const;
    void printTree(AvlNode<Comparable>* t) const;
    //AvlNode<Comparable>* clone(AvlNode<Comparable>* t) const;

    // Avl manipulations
    int height(AvlNode<Comparable>* t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<Comparable>*& k2) const;
    void rotateWithRightChild(AvlNode<Comparable>*& k1) const;
    void doubleWithLeftChild(AvlNode<Comparable>*& k3) const;
    void doubleWithRightChild(AvlNode<Comparable>*& k1) const;
};

/**
* Construct the tree.
*/
template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable& notFound) :
    ITEM_NOT_FOUND(notFound), root(NULL)
{}


/**
* Checks if it is empty or not
* Return true if it is empty.
*/
template <class Comparable>
bool AvlTree<Comparable>::isEmpty() const
{
    return root == NULL;
}

/**
* Internal method to get element field in node t.
* Return the element field or ITEM_NOT_FOUND if t is NULL.
*/
template <class Comparable>
const Comparable& AvlTree<Comparable>::
elementAt(AvlNode<Comparable>* t) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

/**
* Find item x in the tree.
* Return the matching item or ITEM_NOT_FOUND if not found.
*/
template <class Comparable>
const Comparable& AvlTree<Comparable>::
find(const Comparable& x) const
{
    return elementAt(find(x, root));
}

/**
* Internal method to find an item in a subtree.
* x is item to search for.
* t is the node that roots the tree.
* Return node containing the matched item.
*/
template <class Comparable>
AvlNode<Comparable>*
AvlTree<Comparable>::
find(const Comparable& x, AvlNode<Comparable>* t) const
{
    while (t != NULL)
        if (x < t->element)
            t = t->left;
        else if (t->element < x)
            t = t->right;
        else
            return t; // Match
    return NULL; // No match
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable& AvlTree<Comparable>::findMin() const
{
    return elementAt(findMin(root));
}

/**
* Internal method to find the smallest item in a subtree t.
* Return node containing the smallest item.
*/
template <class Comparable>
AvlNode<Comparable>*
AvlTree<Comparable>::findMin(AvlNode<Comparable>* t)
const
{
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable& AvlTree<Comparable>::findMax() const
{
    return elementAt(findMax(root));
}

/**
* Internal method to find the largest item in a subtree t.
* Return node containing the largest item.
*/
template <class Comparable>
AvlNode<Comparable>*
AvlTree<Comparable>::findMax(AvlNode<Comparable>* t)
const
{
    if (t != NULL)
        while (t->right != NULL)
            t = t->right;
    return t;
}



/**
        * Return the height of node t, or -1, if NULL.
*/
template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable>* t)
const
{
    if (t == NULL)
        return -1;

    return t->height;
}

/**
        * Return maximum of lhs and rhs.
*/
template <class Comparable>
int AvlTree<Comparable>::max(int lhs, int rhs)
const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}


/**
* Insert x into the tree; duplicates are ignored.
*/
template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable& x)
{
    insert(x, root);
}

/**
* Internal method to insert into a subtree.
* x is the item to insert.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Comparable>
void AvlTree<Comparable>::insert
(const Comparable& x, AvlNode<Comparable>*& t) const
{
    if (t == NULL)
        t = new AvlNode<Comparable>(x, NULL, NULL);

    else if (x < t->element) {
        // X should be inserted to the left tree!
        insert(x, t->left);

        // Check if the left tree is out of balance (left subtree grew in height!)
        if (height(t->left) - height(t->right) == 2)
            if (x < t->left->element)  // X was inserted to the left-left subtree!
                rotateWithLeftChild(t);
            else			     // X was inserted to the left-right subtree!
                doubleWithLeftChild(t);
    }

    else if (t->element < x)
    {    // Otherwise X is inserted to the right subtree
        insert(x, t->right);
        if (height(t->right) - height(t->left) == 2)
            // height of the right subtree increased
            if (t->right->element < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
    }
    else
        ;  // Duplicate; do nothing

  // update the height the node
    t->height = max(height(t->left), height(t->right)) + 1;
}

/**
         * Rotate binary tree node with left child.
         * For AVL trees, this is a single rotation for case 1.
         * Update heights, then set new root.
         */
template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(
    AvlNode<Comparable>*& k2) const
{
    AvlNode<Comparable>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}


/**
        * Rotate binary tree node with right child.
        * For AVL trees, this is a single rotation for case 4.
        * Update heights, then set new root.
        */
template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild
(AvlNode<Comparable>*& k1) const
{
    AvlNode<Comparable>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}


/**
       * Double rotate binary tree node: first left child.
       * with its right child; then node k3 with new left child.
       * For AVL trees, this is a double rotation for case 2.
       * Update heights, then set new root.
       */
template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild
(AvlNode<Comparable>*& k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}


/**
         * Double rotate binary tree node: first right child.
         * with its left child; then node k1 with new right child.
         * For AVL trees, this is a double rotation for case 3.
         * Update heights, then set new root.
         */
template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild
(AvlNode<Comparable>*& k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}




/**
* Remove x from the tree. Nothing is done if x is not found.
*/
template <class Comparable>
void AvlTree<Comparable>::remove
(const Comparable& x)
{
    remove(x, root);
}

/**
* Internal method to remove from a subtree.
* x is the item to remove.
* t is the node that roots the subtree.
* Set the new root of the subtree.
*/
template <class Comparable>
void AvlTree<Comparable>::remove
(const Comparable& x, AvlNode<Comparable>*& t) const
{
    if (t == nullptr)
        return;
    // Item not found; do nothing
    if (x < t->element)
        remove(x, t->left);
    else if (t->element < x)
        remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) // Two children
    {
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else
    {
        AvlNode<Comparable>* oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }

    //balance
    if (t == nullptr)
        return;
    if (height(t->left) - height(t->right) > 1)
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    else
        if (height(t->right) - height(t->left) > 1)
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
    t->height = max(height(t->left), height(t->right)) + 1;

}



/**
* Make the tree logically empty.
*/
template <class Comparable>
void AvlTree<Comparable>::makeEmpty()
{
    makeEmpty(root);
}

/**
* Internal method to make subtree empty.
*/
template <class Comparable>
void AvlTree<Comparable>::
makeEmpty(AvlNode<Comparable>*& t) const
{
    if (t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

/**
* Print the tree contents in sorted order.
*/
template <class Comparable>
void AvlTree<Comparable>::printTree() const
{
    if (isEmpty())
        cout << "Empty tree" << endl;
    else
        printTree(root);
}

/**
* Internal method to print a subtree rooted at t in sorted order.
*/
template <class Comparable>
void AvlTree<Comparable>::
printTree(AvlNode<Comparable>* t) const
{
    if (t != NULL)
    {
        printTree(t->left);
        cout << t->element << endl;
        printTree(t->right);
    }
}


/**
* Destructor for the tree.
*/
template <class Comparable>
AvlTree<Comparable>::~AvlTree()
{
    makeEmpty();
}