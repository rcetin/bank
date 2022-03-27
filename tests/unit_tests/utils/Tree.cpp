#include <gtest/gtest.h>

#include <Tree/Tree.hpp>

TEST(BasicTree, SuccessAddingNode)
{
    Tree<int> t;
    Tree<int>::node* myNode;
    Tree<int>::node* node40;

    t.addRoot(10, &myNode);
    t.addChild(myNode, 20, nullptr);
    t.addChild(myNode, 30, nullptr);
    t.addChild(myNode, 40, &myNode);
    node40 = myNode;

    t.addChild(myNode, 50, nullptr);
    t.addChild(myNode, 51, nullptr);
    t.addChild(myNode, 52, &myNode);
    t.addChild(myNode, 50, nullptr);

    // t.traverse();
    t.dfs_traverse(node40);
    std::cout << "-------------\n";
}

// TEST(TreeIterator, SiblingIterator)
// {
//     Tree<int> t;
//     Tree<int>::node* myNode;
//     Tree<int>::node* node40;

//     t.addRoot(10, &myNode);
//     t.addChild(myNode, 20, nullptr);
//     t.addChild(myNode, 30, nullptr);
//     t.addChild(myNode, 40, &myNode);
//     node40 = myNode;

//     auto it = t.childrenOf(node40);
//     while(it->nextSibling != nullptr) {
//         std::cout << "it->data: " << it->data << "\n";
//     }

//     t.addChild(myNode, 50, nullptr);
//     t.addChild(myNode, 51, nullptr);
//     t.addChild(myNode, 52, &myNode);
//     t.addChild(myNode, 50, nullptr);
// }
/**
 *          10
 *      20      30   40
 *            
 *            50
 */