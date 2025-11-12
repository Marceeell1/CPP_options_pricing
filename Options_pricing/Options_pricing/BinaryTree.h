#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <vector>
#include <iostream>
#include <stdexcept>
#include <iomanip>   // for std::setw



template <typename T>
class BinaryTree {
private:
    int _depth;                               // = N
    std::vector<std::vector<T>> _tree;        // data: row n has n+1 values

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



    void display() const {


        for (int n = 0; n <= _depth; ++n) {

            //Indent the current row to make it look centered
            int indent = (_depth - n) * 3; // change 3 to adjust width
            for (int s = 0; s < indent; ++s)
                std::cout << ' ';

            //  Print all values on this row
            for (int i = 0; i <= n; ++i) {
                std::cout << _tree[n][i];
                if (i != n)
                    std::cout << "  ";
            }
            std::cout << std::endl;

            // Print one line of connectors "/   \"
            if (n < _depth) {
                int indent2 = indent > 0 ? indent - 1 : 0;
                for (int s = 0; s < indent2; ++s)
                    std::cout << ' ';

                for (int i = 0; i <= n; ++i) {
                    std::cout << '/';
                    std::cout << "   ";
                    std::cout << '\\';
                    if (i != n)
                        std::cout << ' ';
                }
                std::cout << std::endl;
            }
        }

        std::cout.flush();
    }


};
#endif
