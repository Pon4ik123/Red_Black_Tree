#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// Root is always Black
// Black uncle - rotate
// Red uncle - flipping the colors
//
//   After rotation            After color flip
//      Black                        Red
//      /   \                       /   \
//    Red   Red                 Black    Black

template<typename T>
class Node {
public:
    Node<T>* parent, * left, * right;
    T value;
    bool red;

    Node(T v, Node<T>* p, Node<T>* l, Node<T>* r) {
        value = v;
        parent = p;
        left = l;
        right = r;
        red = true;
    }
};

template<typename T>
class RBT {
public:
    Node<T>* root_main;
    int size;

    RBT() : root_main(NULL) {
        root_main = nullptr;
    }

    ~RBT() {
        deleteRBT(root_main);
    }

    void deleteRBT(Node<T>* &node) {
        if (node == nullptr) {
            return;
        }

        //delete the left and right subtrees
        deleteRBT(node->left);
        deleteRBT(node->right);

        delete node;
        node = nullptr;
    }//

    void leftRotation(Node<T>* &root, Node<T>* x) {
        Node<T>* y = x->right; //temporary pointer y to the right child of x
        x->right = y->left;    //the right child of x to be the left child of y

        //If y has a left child, update its parent pointer to be x
        if (y->left != nullptr) {
            y->left->parent = x;
        }

        //the parent of y to be the same as the parent of x
        y->parent = x->parent;

        //If x is the root, update the root of the tree to be y
        if (x->parent == nullptr) {
            root = y;
        }
        else {

            // If x is a left child, update the left child of x's parent to be y,
            // otherwise update the right child
            if (x == x->parent->left) {
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }

        }

        // Complete the rotation by updating the left child of y to be x,
        // and updating the parent of x to be y
        y->left = x;
        x->parent = y;
    }//

    void rightRotation(Node<T>* &root, Node<T>* y) {
        Node<T>* x = y->left; //a temporary pointer x to the left child of y
        y->left = x->right;   //the left child of y to be the right child of x

        //If x has a right child, update its parent pointer to be y
        if (x->right != nullptr) {
            x->right->parent = y;
        }

        //the parent of x to be the same as the parent of y
        x->parent = y->parent;

        //If y is the root, update the root of the tree to be x
        if (y->parent == nullptr) {
            root = x;
        }
        else {

            // If y is a right child, update the right child of y's parent to be x,
            // otherwise update the left child
            if (y == y->parent->right) {
                y->parent->right = x;
            }
            else {
                y->parent->left = x;
            }

        }

        // Complete the rotation by updating the right child of x to be y,
        // and updating the parent of y to be x
        x->right = y;
        y->parent = x;
    }//

    void add(T value) {
        Node<T>* node = new Node<T>(value, nullptr, nullptr, nullptr);
        node->red = true;
        add(this->root_main, node);
    }//

    void add(Node<T>* &root, Node<T>* newNode) {
        Node<T>* x = root;
        Node<T>* temp = nullptr;
        while (x != nullptr) {
            temp = x;
            if (newNode->value > x->value) {
                x = x->right;
            }
            else {
                x = x->left;
            }
        }
        newNode->parent = temp;
        if (temp != nullptr) {
            if (newNode->value > temp->value) {
                temp->right = newNode;
            }
            else {
                temp->left = newNode;
            }
        }
        else {
            root = newNode;
        }
        newNode->red = true;
        addFix(root, newNode);
    }//

    void addFix(Node<T>* &root, Node<T>* newNode) {
        Node<T>* parent = newNode->parent;

        while (newNode != this->root_main && parent->red == true) {
            Node<T>* grandparent = parent->parent;

            if (grandparent->left == parent) {
                Node<T>* uncle = grandparent->right;

                if (uncle != nullptr && uncle->red == true) {

                    parent->red = false;
                    uncle->red = false;
                    grandparent->red = true;

                    newNode = grandparent;
                    parent = newNode->parent;

                }
                else {

                    if (parent->right == newNode) {
                        leftRotation(root, parent);
                        Node<T>* temp = parent;
                        parent = newNode;
                        newNode = temp;
                    }
                    rightRotation(root, grandparent);
                    grandparent->red = true;
                    parent->red = false;
                    break;

                }
            }

            else {
                Node<T>* uncle = grandparent->left;

                if (uncle != nullptr && uncle->red == true) {

                    parent->red = false;
                    uncle->red = false;
                    grandparent->red = true;

                    newNode = grandparent;
                    parent = newNode->parent;

                }
                else {

                    if (parent->left == newNode) {
                        rightRotation(root, parent);
                        Node<T>* temp = parent;
                        parent = newNode;
                        newNode = temp;
                    }
                    leftRotation(root, grandparent);
                    grandparent->red = true;
                    parent->red = false;
                    break;
                }
            }

        }
        root->red = false;
    }//

    Node<T>* serch(Node<T>* node, int key) {
        if (node == nullptr || node->value == key) {
            return node;
        }
        else {
            if (key > node->value) {
                return serch(node->right, key);
            }
            else {
                return serch(node->left, key);
            }
        }
    }//

    void preOrder(Node<T>* tree) {
        if (tree != nullptr) {
            cout << tree->value << " ";
            preOrder(tree->left);
            preOrder(tree->right);
        }
    }//

    void preOrder() {
        if (root_main == nullptr)
            cout << "empty RBtree\n";
        else
            preOrder(root_main);
    }//

    void inOrder(Node<T>* tree) {
        if (tree != nullptr) {
            inOrder(tree->left);
            cout << tree->value << " ";
            inOrder(tree->right);
        }
    }//

    void inOrder() {
        if (root_main == nullptr)
            cout << "empty RBtree\n";
        else
            inOrder(root_main);
    }//

    void showTree() {
        if (root_main == nullptr)
            cout << "empty RBtree\n";
        else
            showTree(root_main);
    }//

    void showTree(Node<T>* node) {
        if (node == nullptr) {
            return;
        }
        if (node->parent == nullptr) {
            if (node->red) {
                cout << node->value << "(red) is root" << endl;
            }
            else {
                cout << node->value << "(black) is root" << endl;
            }
        }
        else if (node->parent->left == node) {
            if (node->red) {
                cout << node->value << "(red) is " << node->parent->value << "'s " << "left child" << endl;
            }
            else {
                cout << node->value << "(black) is " << node->parent->value << "'s " << "left child" << endl;
            }
        }
        else {
            if (node->red) {
                cout << node->value << "(red) is " << node->parent->value << "'s " << "right child" << endl;
            }
            else {
                cout << node->value << "(black) is " << node->parent->value << "'s " << "right child" << endl;
            }
        }
        showTree(node->left);
        showTree(node->right);
    }//

    int HeightOfTree(Node<T> *node) {
        if(node == 0)
            return 0;
        int left, right;
        if (node->left != nullptr) {
            left = HeightOfTree(node->left);
        }else
            left = -1;
        if (node->right != nullptr) {
            right = HeightOfTree(node->right);
        }else
            right = -1;
        int max = left > right ? left : right;
        return max+1;
    }
};

int main() {
    RBT<int> tree;
    tree.add(10);
    tree.add(40);
    tree.add(30);
    tree.add(60);
    tree.add(90);
    tree.add(70);
    tree.add(20);
    tree.add(50);
    tree.add(80);
    tree.add(100);
    int height = tree.HeightOfTree(tree.root_main);
    cout << height << endl;
    tree.showTree();
    return 0;
}