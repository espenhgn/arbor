#include <iostream>
#include <fstream>
#include <numeric>

#include "gtest/gtest.h"

#include "cell_tree.hpp"
#include "json/src/json.hpp"

using json = nlohmann::json;
using range = memory::Range;

TEST(cell_tree, from_parent_index) {
    // tree with single branch corresponding to the root node
    // this is equivalent to a single compartment model
    //      CASE 1 : single root node in parent_index
    {
        std::vector<int> parent_index = {0};
        cell_tree tree(parent_index);
        EXPECT_EQ(tree.num_segments(), 1);
        EXPECT_EQ(tree.num_children(0), 0);
    }
    //      CASE 2 : empty parent_index
    {
        std::vector<int> parent_index;
        cell_tree tree(parent_index);
        EXPECT_EQ(tree.num_segments(), 1);
        EXPECT_EQ(tree.num_children(0), 0);
    }
    // tree with two segments off the root node
    {
        std::vector<int> parent_index =
            {0, 0, 1, 2, 0, 4};
        cell_tree tree(parent_index);
        EXPECT_EQ(tree.num_segments(), 3);
        // the root has 2 children
        EXPECT_EQ(tree.num_children(0), 2);
        // the children are leaves
        EXPECT_EQ(tree.num_children(1), 0);
        EXPECT_EQ(tree.num_children(2), 0);
    }
    {
        // tree with three segments off the root node
        std::vector<int> parent_index =
            {0, 0, 1, 2, 0, 4, 0, 6, 7, 8};
        cell_tree tree(parent_index);
        EXPECT_EQ(tree.num_segments(), 4);
        // the root has 3 children
        EXPECT_EQ(tree.num_children(0), 3);
        // the children are leaves
        EXPECT_EQ(tree.num_children(1), 0);
        EXPECT_EQ(tree.num_children(2), 0);
        EXPECT_EQ(tree.num_children(3), 0);
    }
    {
        // tree with three segments off the root node, and another 2 segments off of the third branch from the root node
        std::vector<int> parent_index =
            {0, 0, 1, 2, 0, 4, 0, 6, 7, 8, 9, 8, 11, 12};
        cell_tree tree(parent_index);
        EXPECT_EQ(tree.num_segments(), 6);
        // the root has 3 children
        EXPECT_EQ(tree.num_children(0), 3);
        // one of the chilren has 2 children ...
        EXPECT_EQ(tree.num_children(3), 2);
        // the rest are leaves
        EXPECT_EQ(tree.num_children(1), 0);
        EXPECT_EQ(tree.num_children(2), 0);
        EXPECT_EQ(tree.num_children(4), 0);
        EXPECT_EQ(tree.num_children(5), 0);
    }
    {
        //
        //              0
        //             /
        //            1
        //           / \
        //          2   3
        std::vector<int> parent_index = {0,0,1,1};
        cell_tree tree(parent_index);

        EXPECT_EQ(tree.num_segments(), 4);

        EXPECT_EQ(tree.num_children(0), 1);
        EXPECT_EQ(tree.num_children(1), 2);
        EXPECT_EQ(tree.num_children(2), 0);
        EXPECT_EQ(tree.num_children(3), 0);
    }
    {
        //
        //              0
        //             / \
        //            1   2
        //           / \
        //          3   4
        std::vector<int> parent_index = {0,0,0,1,1};
        cell_tree tree(parent_index);

        EXPECT_EQ(tree.num_segments(), 5);

        EXPECT_EQ(tree.num_children(0), 2);
        EXPECT_EQ(tree.num_children(1), 2);
        EXPECT_EQ(tree.num_children(2), 0);
        EXPECT_EQ(tree.num_children(3), 0);
        EXPECT_EQ(tree.num_children(4), 0);
    }
    {
        //              0
        //             / \
        //            1   2
        //           / \
        //          3   4
        //             / \
        //            5   6
        std::vector<int> parent_index = {0,0,0,1,1,4,4};
        cell_tree tree(parent_index);

        EXPECT_EQ(tree.num_segments(), 7);

        EXPECT_EQ(tree.num_children(0), 2);
        EXPECT_EQ(tree.num_children(1), 2);
        EXPECT_EQ(tree.num_children(2), 0);
        EXPECT_EQ(tree.num_children(3), 0);
        EXPECT_EQ(tree.num_children(4), 2);
        EXPECT_EQ(tree.num_children(5), 0);
        EXPECT_EQ(tree.num_children(6), 0);
    }
}

TEST(tree, change_root) {
    {
        // a cell with the following structure
        // make 1 the new root
        //              0       0
        //             / \      |
        //            1   2 ->  1
        //                      |
        //                      2
        std::vector<int> parent_index = {0,0,0};
        tree t;
        t.init_from_parent_index(parent_index);
        t.change_root(1);

        EXPECT_EQ(t.num_nodes(), 3);

        EXPECT_EQ(t.num_children(0), 1);
        EXPECT_EQ(t.num_children(1), 1);
        EXPECT_EQ(t.num_children(2), 0);
    }
    {
        // a cell with the following structure
        // make 1 the new root
        //              0          0
        //             / \        /|\
        //            1   2  ->  1 2 3
        //           / \             |
        //          3   4            4
        std::vector<int> parent_index = {0,0,0,1,1};
        tree t;
        t.init_from_parent_index(parent_index);
        t.change_root(1);

        EXPECT_EQ(t.num_nodes(), 5);

        EXPECT_EQ(t.num_children(0), 3);
        EXPECT_EQ(t.num_children(1), 0);
        EXPECT_EQ(t.num_children(2), 0);
        EXPECT_EQ(t.num_children(3), 1);
        EXPECT_EQ(t.num_children(4), 0);
    }
    {
        // a cell with the following structure
        // make 1 the new root
        // unlike earlier tests, this decreases the depth
        // of the tree
        //              0         0
        //             / \       /|\
        //            1   2 ->  1 2 5
        //           / \         / \ \
        //          3   4       3   4 6
        //             / \
        //            5   6
        std::vector<int> parent_index = {0,0,0,1,1,4,4};
        tree t;
        t.init_from_parent_index(parent_index);

        t.change_root(1);

        EXPECT_EQ(t.num_nodes(), 7);

        EXPECT_EQ(t.num_children(0), 3);
        EXPECT_EQ(t.num_children(1), 0);
        EXPECT_EQ(t.num_children(2), 2);
        EXPECT_EQ(t.num_children(3), 0);
        EXPECT_EQ(t.num_children(4), 0);
        EXPECT_EQ(t.num_children(5), 1);
        EXPECT_EQ(t.num_children(6), 0);
    }
}

TEST(cell_tree, balance) {
    {
        // a cell with the following structure
        // will balance around 1
        //              0         0
        //             / \       /|\
        //            1   2 ->  1 2 5
        //           / \         / \ \
        //          3   4       3   4 6
        //             / \
        //            5   6
        std::vector<int> parent_index = {0,0,0,1,1,4,4};
        cell_tree t(parent_index);

        t.balance();

        // the soma (original root) has moved to 5 in the new tree
        EXPECT_EQ(t.soma(), 5);

        EXPECT_EQ(t.num_segments(), 7);
        EXPECT_EQ(t.num_children(0),3);
        EXPECT_EQ(t.num_children(1),0);
        EXPECT_EQ(t.num_children(2),2);
        EXPECT_EQ(t.num_children(3),0);
        EXPECT_EQ(t.num_children(4),0);
        EXPECT_EQ(t.num_children(5),1);
        EXPECT_EQ(t.num_children(6),0);
        EXPECT_EQ(t.parent(0),-1);
        EXPECT_EQ(t.parent(1), 0);
        EXPECT_EQ(t.parent(2), 0);
        EXPECT_EQ(t.parent(3), 0);
        EXPECT_EQ(t.parent(4), 2);
        EXPECT_EQ(t.parent(5), 2);
        EXPECT_EQ(t.parent(6), 5);

        t.to_graphviz("cell.dot");
    }
}

// this test doesn't test anything yet... it just loads each cell in turn
// from a json file and creates a .dot file for it
TEST(cell_tree, json_load) {
    json  cell_data;
    std::ifstream("cells_small.json") >> cell_data;

    for(auto c : range(0,cell_data.size())) {
        std::vector<int> parent_index = cell_data[c]["parent_index"];
        cell_tree tree(parent_index);
        //tree.to_graphviz("cell_" + std::to_string(c) + ".dot");
        tree.to_graphviz("cell" + std::to_string(c) + ".dot");
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
