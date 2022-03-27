#include <gtest/gtest.h>

#include <Tree/Tree.hpp>

// TEST(BasicTree, SuccessAddingNode)
// {
//     Tree<int> t;
//     Tree<int>::node* myNode;
//     Tree<int>::node* node40;

//     t.addRoot(10, &myNode);
//     t.addChild(myNode, 20, nullptr);
//     t.addChild(myNode, 30, nullptr);
//     t.addChild(myNode, 40, &myNode);
//     node40 = myNode;

//     t.addChild(myNode, 50, nullptr);
//     t.addChild(myNode, 51, nullptr);
//     t.addChild(myNode, 52, &myNode);
//     t.addChild(myNode, 50, nullptr);

//     // t.traverse();
//     t.dfs_traverse(node40);
//     std::cout << "-------------\n";
// }

TEST(TreeIterator, SiblingIterator)
{
    Tree<int> t;
    Tree<int>::node* myNode;

    myNode = t.addRoot(10);
    t.addChild(myNode, 11);
    auto ch = t.addChild(myNode, 12);
    t.addChild(ch, 121);
    t.addChild(ch, 122);
    auto it = t.getIteratorToChild(ch);
    while(it.get()) {
        std::cout << "ch Child: " << it->data << "\n";
        it++;
    }

    ch = t.addChild(myNode, 13);

    it = t.getIteratorToChild(myNode);
    while(it.get()) {
        std::cout << "myNode Child: " << it->data << "\n";
        it++;
    }
    t.addChild(ch, 131);
    t.addChild(ch, 132);

    t.dump();
}
/**
 *          10
 *      20      30   40
 *            
 *            50
 */