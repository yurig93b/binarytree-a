//
// Created by Yuri Grigorian on 05/05/2021.
//


#include <cmath>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "sources/BinaryTree.hpp"
#include "sources/TraverseType.hpp"
#include "doctest.h"

using namespace ariel;

// a func to get a random positive int
u_int getrandom(u_int min, u_int max) {
    // just to be sure
    if (min > max) {
        u_int temp = min;
        min = max;
        max = temp;
    }

    u_int i = rand() % 10;
    i %= 5;
    u_int k = pow(10, i);
    u_int r = (u_int) rand() % k;
    return r;
}

// a func to get a random string
std::string randString() {
    u_int len = (u_int)(rand() % 20);
    std::string s = "";
    for (size_t i = 0; i < len; i++) {
        s += (char) (65 + (rand() % 57));
    }
    return s;

}

TEST_CASE ("Test addition") {
    BinaryTree<int> tree;

    tree.add_root(0);
    std::vector<int> v = {0};
    CHECK(std::equal(tree.begin_preorder(), tree.end_preorder(), v.begin()));

    // add children
    tree.add_left(0 ,1);
    tree.add_right(0 ,2);

    v = {0,1,2};
    CHECK(std::equal(tree.begin_preorder(), tree.end_preorder(), v.begin()));

    // replace vals
    tree.add_left(0 ,5);
    tree.add_right(0 ,6);
    tree.add_right(5 ,7);

    v = {0,5,7,6};
    CHECK(std::equal(tree.begin_preorder(), tree.end_preorder(), v.begin()));

    // add no exist
    CHECK_THROWS(tree.add_left(9999, 9));
    CHECK_THROWS(tree.add_right(9999, 9));
}

struct student{
    std::string name;
    student(std::string _name): name(_name){};

    bool operator==(const student &other){
        return name == other.name;
    }
};

TEST_CASE ("Test iterators") {
//         0
//    1         2
// 3     4

    BinaryTree<int> tree;
    tree.add_root(0);
    // add children
    tree.add_left(0, 1);
    tree.add_right(0, 2);
    tree.add_left(1, 3);
    tree.add_right(1, 4);

    CHECK(std::is_permutation(tree.begin_preorder(), tree.end_preorder(), tree.begin_inorder()));
    CHECK(std::is_permutation(tree.begin_preorder(), tree.end_preorder(), tree.begin_postorder()));
    CHECK(std::is_permutation(tree.begin_inorder(), tree.end_inorder(), tree.begin_postorder()));

    std::vector<int> v_pre = {0, 1, 3, 4, 2};
    std::vector<int> v_post = {3, 4, 1, 2, 0};
    std::vector<int> v_in = {3, 1, 4, 0, 2};


    CHECK(std::equal(tree.begin_preorder(), tree.end_preorder(), v_pre.begin()));
    CHECK(std::equal(tree.begin_postorder(), tree.end_postorder(), v_post.begin()));
    CHECK(std::equal(tree.begin_inorder(), tree.end_inorder(), v_in.begin()));


    // Check iterator stops on bad ++ pre incr
    auto it_pre = tree.begin_preorder();
    for(; it_pre != tree.end_preorder(); ++it_pre){}
    CHECK_THROWS(++it_pre);

    auto it_post = tree.begin_postorder();
    for(; it_post != tree.end_postorder(); ++it_post){}
    CHECK_THROWS(++it_post);

    auto it_in = tree.begin_postorder();
    for(; it_in != tree.end_postorder(); ++it_in){}
    CHECK_THROWS(++it_in);

    // Check iterator stops on bad ++ post incr
    it_pre = tree.begin_preorder();
    for(; it_pre != tree.end_preorder(); it_pre++){}
    CHECK_THROWS(it_pre++);

    it_post = tree.begin_postorder();
    for(; it_post != tree.end_postorder(); it_post++){}
    CHECK_THROWS(it_post++);

    it_in = tree.begin_postorder();
    for(; it_in != tree.end_postorder(); it_in++){}
    CHECK_THROWS(it_in++);
}

TEST_CASE ("Test multiple types") {
    // Strings
    BinaryTree<std::string> tree_of_strings;
    tree_of_strings.add_root("Hello");
    tree_of_strings.add_left("Hello", "World");
    tree_of_strings.add_right("Hello", "!");

    std::vector<std::string> v_strings = {"Hello", "World", "!"};
    CHECK(std::equal(tree_of_strings.begin_preorder(), tree_of_strings.end_preorder(), v_strings.begin()));
    tree_of_strings.add_root("Goodbye");
    v_strings = {"Goodbye", "World", "!"};
    CHECK(std::equal(tree_of_strings.begin_preorder(), tree_of_strings.end_preorder(), v_strings.begin()));


    // My struct
    BinaryTree<student> tree_of_student;
    tree_of_student.add_root(student("yuri"));
    tree_of_student.add_left(student("yuri"), student("student2"));
    std::vector<student> v_students = {student("yuri"), student("student2")};
    CHECK(std::equal(tree_of_student.begin_preorder(), tree_of_student.end_preorder(), v_students.begin()));
}

TEST_CASE ("Test constructor") {
    BinaryTree<int> tree;
    tree.add_root(0);
    tree.add_left(0, 1);
    tree.add_right(0 ,2);

    // Copy constructor
    BinaryTree<int> tree2(tree);
    CHECK(std::equal(tree.begin(), tree.end(), tree2.begin()));
    tree2.add_left(0, 10000);
    CHECK_FALSE(std::equal(tree.begin(), tree.end(), tree2.begin()));

    // Copy assigment
    tree2 = tree;
    CHECK(std::equal(tree.begin(), tree.end(), tree2.begin()));
    tree2.add_left(0, 1234);
    CHECK_FALSE(std::equal(tree.begin(), tree.end(), tree2.begin()));

    // Move constructor
    BinaryTree<int> tree3(std::move(tree));
    std::vector<int> v = {0, 1, 2};
    CHECK(tree.begin() == tree.end());
    CHECK(std::equal(tree3.begin_preorder(),tree3.end_preorder(), v.begin()));

    // Move assigment
    BinaryTree<int> tree4;
    tree4 = std::move(tree3);
    CHECK(tree3.begin() == tree3.end());
    CHECK(std::equal(tree4.begin_preorder(),tree4.end_preorder(), v.begin()));
}

TEST_CASE ("Test has some kind of output") {
    BinaryTree<int> tree;

    tree.add_root(0);
    std::stringstream ss;
    ss << tree;
    CHECK(ss.str() != "");
}