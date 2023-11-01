#include <iostream>
using namespace std;

struct Trunk
{
    Trunk *prev;
    string str;

    Trunk(Trunk *prev, string str)
    {
        this->prev = prev;
        this->str = str;
    }
};

// Helper function to print branches of the binary tree
void showTrunks(Trunk *p)
{
    if (p == NULL)
        return;

    showTrunks(p->prev);

    cout << p->str;
}

class Node
{
public:
    int data;
    Node *left;
    Node *right;
    int height;
    Node(int data)
    {
        this->data = data;
        this->left = NULL;
        this->right = NULL;
        this->height = 0;
    }
};

class AVLTree
{
public:
    Node *root;

    AVLTree()
    {
        root = NULL;
    }

    int getHeight(Node *n)
    {
        if (n == NULL)
            return 0;
        return n->height;
    }

    int getBalanceFactor(Node *n)
    {
        if (n == NULL)
            return 0;

        return getHeight(n->left) - getHeight(n->right);
    }

    Node *rotateRight(Node *n)
    {

        Node *newRoot = n->left;
        Node *temp = newRoot->right;

        newRoot->right = n;
        n->left = temp;

        n->height = max(getHeight(n->left), getHeight(n->right)) + 1;
        newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

        return newRoot;
    }

    Node *rotateLeft(Node *n)
    {
        Node *newRoot = n->right;
        Node *temp = newRoot->left;

        newRoot->left = n;
        n->right = temp;

        n->height = max(getHeight(n->left), getHeight(n->right)) + 1;
        newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

        return newRoot;
    }

    Node *insertValue(Node *node, int value)
    {
        if (node == NULL)
        {
            Node *newNode = new Node(value);
            newNode->height = 1;
            return newNode;
        }

        if (value < node->data)
            node->left = insertValue(node->left, value);
        else if (value > node->data)
            node->right = insertValue(node->right, value);
        else
            return node;

        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor > 1)
        {
            if (value < node->left->data)
                return rotateRight(node);
            else
            {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else if (balanceFactor < -1)
        {
            if (value > node->right->data)
                return rotateLeft(node);
            else
            {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    Node *minValueNode(Node *n)
    {
        Node *curr = n;
        while (curr->left != NULL)
            curr = curr->left;

        return curr;
    }
    Node *maxValueNode(Node *n)
    {
        Node *curr = n;
        while (curr->right != NULL)
            curr = curr->right;

        return curr;
    }

    Node *deleteValue(Node *root, int value)
    {
        if (root == nullptr)
            return root;

        if (value < root->data)
            root->left = deleteValue(root->left, value);
        else if (value > root->data)
            root->right = deleteValue(root->right, value);
        else
        {
            if (root->left == nullptr || root->right == nullptr)
            {
                Node *temp = root->left ? root->left : root->right;

                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;

                delete temp;
            }
            else
            {
                Node *temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = deleteValue(root->right, temp->data);
            }
        }

        if (root == nullptr)
            return root;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
        int balanceFactor = getBalanceFactor(root);

        if (balanceFactor > 1)
        {
            if (getBalanceFactor(root->left) >= 0)
                return rotateRight(root);
            else
            {
                root->left = rotateLeft(root->left);
                return rotateRight(root);
            }
        }
        else if (balanceFactor < -1)
        {
            if (getBalanceFactor(root->right) <= 0)
                return rotateLeft(root);
            else
            {
                root->right = rotateRight(root->right);
                return rotateLeft(root);
            }
        }

        return root;
    }

    void inOrderTraversal(Node *node)
    {
        if (node != nullptr)
        {
            inOrderTraversal(node->left);
            cout << node->data << " ";
            inOrderTraversal(node->right);
        }
    }

    Node *Search(Node *root, int data)
    {
        if (root == nullptr || root->data == data)
            return root;

        if (root->data < data)
            return Search(root->right, data);

        return Search(root->left, data);
    }
};

void Menu()
{
    cout << endl
         << "------------------------------------------------------" << endl;
    cout << "Menu" << endl;
    cout << endl
         << "1. Insert" << endl;
    cout << "2. Delete" << endl;
    cout << "3. InOrder Traversal" << endl;
    cout << "4. Search" << endl;
    cout << "5. Max" << endl;
    cout << "6. Min" << endl;
    cout << "7. Exit" << endl;
    cout << "------------------------------------------------------" << endl
         << endl;
}

void printTree(Node *&root, Trunk *prev, bool isRight)
{
    if (root == NULL)
        return;

    string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    printTree(root->right, trunk, true);

    if (!prev)
        trunk->str = "---";
    else if (isRight)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else
    {
        trunk->str = "`---";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    cout << root->data << endl;

    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->left, trunk, false);
}

int main()
{
    AVLTree tree;
    int choice, value;

    while (true)
    {
        Menu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter the value to be inserted: ";
            cin >> value;
            tree.root = tree.insertValue(tree.root, value);
            break;
        case 2:
            cout << "Enter the value to be deleted: ";
            cin >> value;
            tree.root = tree.deleteValue(tree.root, value);
            break;
        case 3:
            cout << "In Order Traversal: ";
            tree.inOrderTraversal(tree.root);
            break;
        case 4:
            cout << "Enter the value to be searched: ";
            cin >> value;
            if (tree.Search(tree.root, value) != NULL)
                cout << "Value Found" << endl;
            else
                cout << "Value Not Found" << endl;
            break;
        case 5:
            cout << "Max value node is:" << tree.maxValueNode(tree.root)->data << endl;
            break;
        case 6:
            cout << "Min value node is:" << tree.minValueNode(tree.root)->data << endl;

            break;
        case 7:
            exit(0);
        default:
            cout << "Invalid Choice" << endl;
        }
    }

    return 0;
}
