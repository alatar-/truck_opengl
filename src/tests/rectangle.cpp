#include "rectangle.h"

void TestRectangle::test_intersection_positives_only_1() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_2) == false);
}

void TestRectangle::test_intersection_positives_only_2() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_3) == true);
}

void TestRectangle::test_intersection_bent_1() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_bend_1) == true);
}


void TestRectangle::test_full_inclusion_positives_only_1() {
    CPPUNIT_ASSERT(r_pos_1->full_inclusion(*r_pos_4) == true);
}

void TestRectangle::test_full_inclusion_positives_only_2() {
    CPPUNIT_ASSERT(r_pos_1->full_inclusion(*r_pos_2) == false);
}


void TestRectangle::test_point_inside_positives_only_1() {
    CPPUNIT_ASSERT(r_pos_1->has_point_inside(*point_1) == true);
}

void TestRectangle::test_point_inside_positives_only_2() {
    CPPUNIT_ASSERT(r_pos_2->has_point_inside(*point_1) == false);
}

void TestRectangle::test_point_inside_bent_1() {
    CPPUNIT_ASSERT(r_bend_1->has_point_inside(*point_2) == true);
}

void TestRectangle::test_point_inside_bent_2() {
    CPPUNIT_ASSERT(r_bend_1->has_point_inside(*point_3) == false);
}

void TestRectangle::setUp(void) {
    r_pos_1 = new Rectangle(
        vertex_2d(1, 6)
    ,   vertex_2d(5, 6)
    ,   vertex_2d(5, 2)
    ,   vertex_2d(1, 2) );

    r_pos_2 = new Rectangle(
        vertex_2d(6, 4)
    ,   vertex_2d(8, 4)
    ,   vertex_2d(8, 3)
    ,   vertex_2d(6, 3) );

    r_pos_3 = new Rectangle(
        vertex_2d(4, 4)
    ,   vertex_2d(8, 4)
    ,   vertex_2d(8, 3)
    ,   vertex_2d(4, 3) );

    r_pos_4 = new Rectangle(
        vertex_2d(2, 4)
    ,   vertex_2d(3, 4)
    ,   vertex_2d(3, 3)
    ,   vertex_2d(2, 3) );

    r_bend_1 = new Rectangle(
        vertex_2d(1, 2)
    ,   vertex_2d(2, 3)
    ,   vertex_2d(3, 2)
    ,   vertex_2d(2, 1) );

    point_1 = new vertex_2d(3, 3);
    point_2 = new vertex_2d(2, 2);
    point_3 = new vertex_2d(10, 12);
}

void TestRectangle::tearDown(void) {
    delete r_pos_1;
    delete r_pos_2;
    delete r_pos_3;
    delete r_pos_4;
    delete r_bend_1;

    delete point_1;
    delete point_2;
    delete point_3;
}
