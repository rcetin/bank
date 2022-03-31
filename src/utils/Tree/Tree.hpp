#ifndef TREE_HPP
#define TREE_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

template <typename T>
class Tree
{
public:
    struct node
    {
        friend class Tree;
        node(T data_, std::unique_ptr<node> first, std::unique_ptr<node> nextSib)
            : data_(data_)
            , firstChild(std::move(first))
            , nextSibling(std::move(nextSib))
        { }

        T data(void)
        {
            return data_;
        }

    private:
        T data_;
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

        // postfix
        iterator operator++(int)
        {
            auto it = *this;
            node_ = node_->nextSibling.get();
            return it;
        }

        // prefix
        iterator& operator++()
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
        std::cout << "\n";
    }

    node* addRoot(T data)
    {
        root = std::make_unique<node>(data, nullptr, nullptr);
        size_++;
        return root.get();
    }

    node* addChild(node* parentNode, T data)
    {
        size_++;
        if(!parentNode->firstChild) {
            parentNode->firstChild = std::make_unique<node>(data, nullptr, nullptr);
            return parentNode->firstChild.get();
        }

        return addSibling(parentNode->firstChild.get(), data);
    }

    bool removeChild(node* parentNode, T data)
    {
        if(!parentNode->firstChild) {
            return false;
        }

        if((parentNode->firstChild)->data_ == data) {
            parentNode->firstChild = std::move((parentNode->firstChild)->nextSibling);
            size_--;
            return true;
        }

        auto prevSibling = parentNode->firstChild.get();
        auto currentSibling = prevSibling->nextSibling.get();
        while(currentSibling) {
            if(currentSibling->data_ == data) {
                prevSibling->nextSibling = std::move(currentSibling->nextSibling);
                currentSibling = nullptr;
                size_--;
                return true;
            }
            currentSibling = currentSibling->nextSibling.get();
            prevSibling = prevSibling->nextSibling.get();
        }

        return false;
    }

    node* findNode(T data)
    {
        return findNodeInternal(root.get(), data);
    }

    node* getParent(node* child)
    {
        return getParentInternal(root.get(), child);
    }

    node* getRoot(void)
    {
        return root.get();
    }

    iterator getIteratorToChild(node* parent)
    {
        return iterator(parent->firstChild.get());
    }

    bool haveChild(node* parent)
    {
        return parent->firstChild.get() ? true : false;
    }

    T data(node* node)
    {
        return node->data_;
    }

    std::size_t size()
    {
        return size_;
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

    node* findNodeInternal(node* root, T data)
    {
        if(!root) {
            return nullptr;
        }

        if(root->data_ == data) {
            return root;
        }

        auto found = findNodeInternal(root->firstChild.get(), data);
        if(found) {
            return found;
        }

        return findNodeInternal(root->nextSibling.get(), data);
    }

    node* getParentInternal(node* root, node* child)
    {
        if(!haveChild(root)) {
            return nullptr;
        }

        auto it = getIteratorToChild(root);
        while(it.get()) {
            if(it.get() == child) {
                return root;
            }
            auto node = getParentInternal(it.get(), child);
            if(node) {
                return node;
            }
            it++;
        }

        return nullptr;
    }

    void dfs_traverse(node* root)
    {
        if(!root) {
            return;
        }
        auto parent = getParent(root);
        std::cout << "node=" << root << ", data=" << root->data_ << ", parent=" << parent << ", "
                  << (parent ? parent->data_ : T{}) << "\n";
        dfs_traverse(root->firstChild.get());
        dfs_traverse(root->nextSibling.get());
    }

    std::unique_ptr<node> root = nullptr;
    std::size_t size_ = 0;
};

#endif