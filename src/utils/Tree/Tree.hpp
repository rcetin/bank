#ifndef TREE_HPP
#define TREE_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <Rand/Rand.hpp>

#include <string>

template <typename T>
class Tree
{
public:
    struct node
    {
        node(T data_, std::unique_ptr<node> first, std::unique_ptr<node> nextSib)
            : data(data_)
            , firstChild(std::move(first))
            , nextSibling(std::move(nextSib))
        {
            std::cout << "Node created with data=" << data << "\n";
        }
        ~node()
        {
            std::cout << "Node destroyed with data=" << data << "\n";
        }

        T data;
        std::unique_ptr<node> firstChild;
        std::unique_ptr<node> nextSibling;
    };

    struct iterator
    {
        iterator(node* node)
            : node_(node)
        { }

        node* operator->()
        {
            return node_;
        }

        node& operator*()
        {
            return *node_;
        }

        iterator& operator++(int)
        {
            node_ = node_->nextSibling.get();
            return *this;
        }

        node* get()
        {
            return node_;
        }

    private:
        node* node_;
    };

    void dump()
    {
        dfs_traverse(root.get());
    }

    node* addRoot(T data)
    {
        root = std::make_unique<node>(data, nullptr, nullptr);

        return root.get();
    }

    node* addChild(node* parentNode, T data)
    {
        if(!parentNode->firstChild) {
            parentNode->firstChild = std::make_unique<node>(data, nullptr, nullptr);
            return parentNode->firstChild.get();
        }

        return addSibling(parentNode->firstChild.get(), data);
    }

    iterator getIteratorToChild(node* parent)
    {
        return iterator(parent->firstChild.get());
    }

    bool haveChild(node* parent)
    {
        return parent->firstChild.get() ? true : false;
    }

private:
    node* addSibling(node* sibling, T data)
    {
        if(!sibling->nextSibling) {
            sibling->nextSibling = std::make_unique<node>(data, nullptr, nullptr);
            return sibling->nextSibling.get();
        }

        return addSibling(sibling->nextSibling.get(), data);
    }

    node* findNode(node* root, node* targetNode)
    {
        if(!root) {
            return nullptr;
        }

        if(root == targetNode) {
            std::cout << "node found\n";
            return root;
        }

        std::cout << root->data;
        if(root->firstChild) {
            std::cout << "\n";
        }
        auto found = findNode(root->firstChild.get(), targetNode);
        if(found) {
            return found;
        }
        if(root->nextSibling) {
            std::cout << "--";
        }

        return findNode(root->nextSibling.get(), targetNode);
    }

    void dfs_traverse(node* root)
    {
        if(!root) {
            return;
        }

        std::cout << root->data;
        if(root->firstChild.get()) {
            std::cout << "\n..";
        }
        dfs_traverse(root->firstChild.get());
        if(root->nextSibling.get()) {
            std::cout << "~";
        }

        dfs_traverse(root->nextSibling.get());
    }

    std::unique_ptr<node> root = nullptr;
};

#endif