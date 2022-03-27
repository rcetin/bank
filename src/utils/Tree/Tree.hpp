#ifndef TREE_HPP
#define TREE_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <Rand/Rand.hpp>

template <typename T>
class Tree
{
public:
    struct node;
    class SiblingIterator;
    // using nodePtr = node*;
    using treeId = int32_t;
    struct node
    {
        // treeId id;
        T data;
        node* firstChild;
        node* nextSibling;
    };

    SiblingIterator childrenOf(node* parent)
    {
        return SiblingIterator{parent->firstChild};
    }

    void traverse()
    {
        dfs_traverse(root);
    }

    void dfs_traverse(node* root)
    {
        if(!root) {
            return;
        }

        std::cout << root->data;
        if(root->firstChild) {
            std::cout << "\n";
        }
        dfs_traverse(root->firstChild);
        if(root->nextSibling) {
            std::cout << "--";
        }

        dfs_traverse(root->nextSibling);
    }

    void addRoot(T data, node** outNode)
    {
        root = new node{data, nullptr, nullptr};
        if(outNode) {
            *outNode = root;
        }
        return;
    }

    void addChild(node* parentNode, T data, node** outNode)
    {
        if(!parentNode->firstChild) {
            parentNode->firstChild = new node{data, nullptr, nullptr};
            if(outNode) {
                *outNode = parentNode->firstChild;
            }
            return;
        }

        addSibling(&parentNode->firstChild, data, outNode);
        return;
    }

    // class SiblingIterator
    // {
    // public:
    //     // Should point the first child
    //     SiblingIterator(node* n)
    //         : node_(n)
    //     { }

    //     SiblingIterator& operator++()
    //     {
    //         node_ = node_->nextSibling;
    //         return *this;
    //     }

    //     node* operator->()
    //     {
    //         return node_;
    //     }

    //     node& operator*()
    //     {
    //         return *node_;
    //     }

    // private:
    //     node* node_;
    // };

private:
    void addSibling(node** sibling, T data, node** outNode)
    {
        if(!(*sibling)) {
            *sibling = new node{data, nullptr, nullptr};
            if(outNode) {
                *outNode = *sibling;
            }
            return;
        }

        addSibling(&((*sibling)->nextSibling), data, outNode);
    }

    node* root = nullptr;
    Rand<0, INT32_MAX> rng;
};

#endif