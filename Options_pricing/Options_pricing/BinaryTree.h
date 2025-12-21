#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <vector>
#include <iostream>
#include <stdexcept>
#include <iomanip>   // for std::setw



template <typename T>
class BinaryTree {
private:
    int _depth;
    std::vector<std::vector<T>> _tree; // data: row n has n+1 values

public:

    // Constructor : empty tree of depth 0
    BinaryTree() : _depth(0), _tree(1, std::vector<T>(1, T{})) {}

    //tree with depth N
    explicit BinaryTree(int depth) { setDepth(depth); }

    // Resize the depth
    void setDepth(int depth) {
        if (depth < 0) throw std::invalid_argument("Depth must be >= 0");
        _depth = depth;
        _tree.clear();
        _tree.resize(_depth + 1);
        for (int n = 0; n <= _depth; ++n) {
            _tree[n].assign(n + 1, T{}); // default value for T
        }
    }

    // Return current depth
    int getDepth() const { return _depth; }

    // Set value of node (n, i)
    void setNode(int n, int i, const T& value) {
        if (n < 0 || n > _depth || i < 0 || i > n)
            throw std::out_of_range("setNode: indices out of range");
        _tree[n][i] = value;
    }

    // to get the value of the node (n, i)
    const T& getNode(int n, int i) const {
        if (n < 0 || n > _depth || i < 0 || i > n)
            throw std::out_of_range("getNode: indices out of range");
        return _tree[n][i];
    }

    void printNode(int n, int i, const std::string& prefix, bool isLast) const
    {
        std::cout << prefix;

        if (n != 0)   // root has no branch symbol
            std::cout << (isLast ? "|__ " : "|-- ");

        // print node value
        std::cout << _tree[n][i] << "\n";


        if (n < _depth)
        {
            std::string newPrefix = prefix + (isLast ? "    " : "|   ");

            printNode(n + 1, i, newPrefix, false);
            printNode(n + 1, i + 1, newPrefix, true);
        }
    }

    void display() const
    {
        if (_depth == 0)
        {
            std::cout << _tree[0][0] << "\n";
            return;
        }

		// Allign the root (just for ethetics)
        std::cout << "    ";
        printNode(0, 0, "", true);

        std::cout.flush();
    }



};
#endif
