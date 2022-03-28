#include <gtest/gtest.h>
#include <string>

#include <Tree/Tree.hpp>

TEST(BasicTree, SuccessAddingRoot)
{
    Tree<int> t;

    t.addRoot(10);
    ASSERT_NE(nullptr, t.findNode(10));
}

TEST(BasicTree, SuccessAddingChild)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    auto node = t.addChild(root, 12);

    ASSERT_EQ(node, t.findNode(12));
}

TEST(BasicTree, DataCheck)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    ASSERT_EQ(root, t.findNode(10));
    ASSERT_EQ(10, t.data(root));
}

TEST(BasicTree, DataCheckString)
{
    Tree<std::string> t;

    auto root = t.addRoot("DrinkMenu");
    ASSERT_EQ(root, t.findNode("DrinkMenu"));
    ASSERT_EQ("DrinkMenu", t.data(root));
}

TEST(BasicTree, RemoveChild)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    t.addChild(root, 12);
    auto sizeBeforeRemove = t.size();

    ASSERT_TRUE(t.removeChild(root, 12));
    auto sizeAfterRemove = t.size();

    ASSERT_EQ(sizeBeforeRemove - 1, sizeAfterRemove);
}

TEST(BasicTree, HaveChild)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    t.addChild(root, 12);
    t.addChild(root, 15);
    t.addChild(root, 20);

    ASSERT_TRUE(t.haveChild(root));
}

TEST(BasicTree, SizeCheck)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    t.addChild(root, 12);
    t.addChild(root, 15);
    auto node = t.addChild(root, 20);

    t.addChild(node, 21);
    t.addChild(node, 25);
    node = t.addChild(node, 27);

    t.addChild(node, 271);
    t.addChild(node, 277);

    ASSERT_EQ(9, t.size());
}

TEST(TreeIterator, SiblingIterator)
{
    Tree<int> t;

    auto root = t.addRoot(10);
    t.addChild(root, 11);
    t.addChild(root, 12);

    auto it = t.getIteratorToChild(root);
    ASSERT_EQ(11, t.data(it++.get()));
    ASSERT_EQ(12, t.data(it++.get()));
    ASSERT_EQ(nullptr, it.get());
}

TEST(TreeIterator, SiblingIteratorString)
{
    Tree<std::string> t;

    auto root = t.addRoot("DrinkMenu");
    t.addChild(root, "Wines");
    t.addChild(root, "Soft Drinks");
    t.addChild(root, "Fruit Juices");

    auto it = t.getIteratorToChild(root);
    ASSERT_EQ("Wines", t.data(it++.get()));
    ASSERT_EQ("Soft Drinks", t.data(it++.get()));
    ASSERT_EQ("Fruit Juices", t.data(it++.get()));
    ASSERT_EQ(nullptr, it.get());
}

TEST(Tree, removeNode)
{
    Tree<int> t;
    Tree<int>::node* myNode;

    myNode = t.addRoot(10);
    t.addChild(myNode, 12);
    t.addChild(myNode, 15);
    auto n = t.addChild(myNode, 17);
    t.addChild(n, 171);
    t.addChild(n, 172);
    t.dump();

    ASSERT_TRUE(t.removeChild(myNode, 17));
    t.dump();

    ASSERT_EQ(nullptr, t.findNode(17));
    ASSERT_EQ(nullptr, t.findNode(171));
    ASSERT_EQ(nullptr, t.findNode(172));
}

TEST(Tree, findNode)
{
    Tree<int> t;
    Tree<int>::node* myNode;

    myNode = t.addRoot(10);
    ASSERT_EQ(myNode, t.findNode(10));

    auto n = t.addChild(myNode, 12);
    ASSERT_EQ(n, t.findNode(12));
}
