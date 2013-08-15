#include "rectangle.h"

void TestRectangle::test_intersection_positives_only_1() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_2) == false);
}

void TestRectangle::test_intersection_positives_only_2() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_3) == true);
}

void TestRectangle::test_intersection_negatives_only_1() {
    CPPUNIT_ASSERT(r_pos_5->intersection(*r_pos_7) == true);
}

void TestRectangle::test_intersection_mixed_1() {
    CPPUNIT_ASSERT(r_pos_5->intersection(*r_pos_6) == true);
}

void TestRectangle::test_intersection_mixed_2() {
    CPPUNIT_ASSERT(r_pos_6->intersection(*r_pos_7) == false);
}

void TestRectangle::test_intersection_mixed_3() {
    CPPUNIT_ASSERT(r_pos_2->intersection(*r_pos_7) == false);
}

void TestRectangle::test_intersection_mixed_4() {
    CPPUNIT_ASSERT(r_pos_8->intersection(*r_pos_9) == false);
}


void TestRectangle::test_intersection_mixed_5() {
    CPPUNIT_ASSERT(r_pos_9->intersection(*r_pos_8) == false);
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

    r_pos_5 = new Rectangle(
        vertex_2d(-4, -1)
    ,   vertex_2d(-1, -1)
    ,   vertex_2d(-1, -5)
    ,   vertex_2d(-4, -5) );

    r_pos_6 = new Rectangle(
        vertex_2d(-2, -3)
    ,   vertex_2d(0, -1)
    ,   vertex_2d(2, -3)
    ,   vertex_2d(0, -5) );

    r_pos_7 = new Rectangle(
        vertex_2d(-5, -5)
    ,   vertex_2d(-4, -4)
    ,   vertex_2d(-3, -5)
    ,   vertex_2d(-4, -6) );

    r_pos_8 = new Rectangle(
        vertex_2d( 81.311844 , -110.697075 ) 
    ,   vertex_2d( 81.311844 , 18.841148 ) 
    ,   vertex_2d( 137.664673 , 18.841148 ) 
    ,   vertex_2d( 137.664673 , -110.697075 ) );

    r_pos_8 = new Rectangle(
        vertex_2d( 166.144043 , -60.926590 ) 
    ,   vertex_2d( 162.747879 , -63.291042 )  
    ,   vertex_2d( 159.538162 , -58.680786 )  
    ,   vertex_2d( 162.934326 , -56.316334 ) );

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
    delete r_pos_5;
    delete r_pos_6;
    delete r_pos_7;
    delete r_pos_8;
    delete r_pos_9;
    delete r_bend_1;

    delete point_1;
    delete point_2;
    delete point_3;
}
