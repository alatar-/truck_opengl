#include "rectangle.h"

void TestRectangle::test_intersection_positives_only_1_f() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_2) == false);
}
void TestRectangle::test_intersection_positives_only_1_r() {
    CPPUNIT_ASSERT(r_pos_2->intersection(*r_pos_1) == false);
}

void TestRectangle::test_intersection_positives_only_2_f() {
    CPPUNIT_ASSERT(r_pos_1->intersection(*r_pos_3) == true);
}
void TestRectangle::test_intersection_positives_only_2_r() {
    CPPUNIT_ASSERT(r_pos_3->intersection(*r_pos_1) == true);
}

void TestRectangle::test_intersection_negatives_only_1_f() {
    CPPUNIT_ASSERT(r_pos_5->intersection(*r_pos_7) == true);
}
void TestRectangle::test_intersection_negatives_only_1_r() {
    CPPUNIT_ASSERT(r_pos_7->intersection(*r_pos_5) == true);
}

void TestRectangle::test_intersection_mixed_1_f() {
    CPPUNIT_ASSERT(r_pos_5->intersection(*r_pos_6) == true);
}
void TestRectangle::test_intersection_mixed_1_r() {
    CPPUNIT_ASSERT(r_pos_6->intersection(*r_pos_5) == true);
}

void TestRectangle::test_intersection_mixed_2_f() {
    CPPUNIT_ASSERT(r_pos_6->intersection(*r_pos_7) == false);
}
void TestRectangle::test_intersection_mixed_2_r() {
    CPPUNIT_ASSERT(r_pos_7->intersection(*r_pos_6) == false);
}

void TestRectangle::test_intersection_mixed_3_f() {
    CPPUNIT_ASSERT(r_pos_2->intersection(*r_pos_7) == false);
}
void TestRectangle::test_intersection_mixed_3_r() {
    CPPUNIT_ASSERT(r_pos_7->intersection(*r_pos_2) == false);
}

void TestRectangle::test_intersection_mixed_4_f() {
    CPPUNIT_ASSERT(r_pos_8->intersection(*r_pos_9) == false);
}
void TestRectangle::test_intersection_mixed_4_r() {
    CPPUNIT_ASSERT(r_pos_9->intersection(*r_pos_8) == false);
}

void TestRectangle::test_intersection_mixed_5_f() {
    CPPUNIT_ASSERT(r_pos_12->intersection(*r_pos_13) == false);
}
void TestRectangle::test_intersection_mixed_5_r() {
    CPPUNIT_ASSERT(r_pos_13->intersection(*r_pos_12) == false);
}

void TestRectangle::test_intersection_mixed_6_f() {
    CPPUNIT_ASSERT(r_pos_14->intersection(*r_pos_15) == false);
}
void TestRectangle::test_intersection_mixed_6_r() {
    CPPUNIT_ASSERT(r_pos_15->intersection(*r_pos_14) == false);
}

void TestRectangle::test_intersection_initial_f() {
    CPPUNIT_ASSERT(r_pos_10->intersection(*r_pos_11) == false);
}
void TestRectangle::test_intersection_initial_r() {
    CPPUNIT_ASSERT(r_pos_11->intersection(*r_pos_10) == false);
}

void TestRectangle::test_intersection_bent_1_f() {
    CPPUNIT_ASSERT(r_bend_1->intersection(*r_pos_1) == true);
}
void TestRectangle::test_intersection_bent_1_r() {
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
        vertex_2d<float>(1, 6)
    ,   vertex_2d<float>(5, 6)
    ,   vertex_2d<float>(5, 2)
    ,   vertex_2d<float>(1, 2) );

    r_pos_2 = new Rectangle(
        vertex_2d<float>(6, 4)
    ,   vertex_2d<float>(8, 4)
    ,   vertex_2d<float>(8, 3)
    ,   vertex_2d<float>(6, 3) );

    r_pos_3 = new Rectangle(
        vertex_2d<float>(4, 4)
    ,   vertex_2d<float>(8, 4)
    ,   vertex_2d<float>(8, 3)
    ,   vertex_2d<float>(4, 3) );

    r_pos_4 = new Rectangle(
        vertex_2d<float>(2, 4)
    ,   vertex_2d<float>(3, 4)
    ,   vertex_2d<float>(3, 3)
    ,   vertex_2d<float>(2, 3) );

    r_pos_5 = new Rectangle(
        vertex_2d<float>(-4, -1)
    ,   vertex_2d<float>(-1, -1)
    ,   vertex_2d<float>(-1, -5)
    ,   vertex_2d<float>(-4, -5) );

    r_pos_6 = new Rectangle(
        vertex_2d<float>(-2, -3)
    ,   vertex_2d<float>(0, -1)
    ,   vertex_2d<float>(2, -3)
    ,   vertex_2d<float>(0, -5) );

    r_pos_7 = new Rectangle(
        vertex_2d<float>(-5, -5)
    ,   vertex_2d<float>(-4, -4)
    ,   vertex_2d<float>(-3, -5)
    ,   vertex_2d<float>(-4, -6) );

    r_pos_8 = new Rectangle(
        vertex_2d<float>( 81.311844 , -110.697075 ) 
    ,   vertex_2d<float>( 81.311844 , 18.841148 ) 
    ,   vertex_2d<float>( 137.664673 , -110.697075 )
    ,   vertex_2d<float>( 137.664673 , 18.841148 ) 
    );

    r_pos_9 = new Rectangle(
        vertex_2d<float>( 166.144043 , -60.926590 ) 
    ,   vertex_2d<float>( 162.747879 , -63.291042 )  
    ,   vertex_2d<float>( 162.934326 , -56.316334 )
    ,   vertex_2d<float>( 159.538162 , -58.680786 )  
    );

    r_pos_10 = new Rectangle(
        vertex_2d<float>( -25.845991 , -111.946411 ) 
    ,   vertex_2d<float>( -25.845991 , 73.699738 ) 
    ,   vertex_2d<float>( -24.449694 , -111.946411 )
    ,   vertex_2d<float>( -24.449694 , 73.699738 ) 
    );

    r_pos_11 = new Rectangle(
        vertex_2d<float>( -3.107260 , -9.625175 ) 
    ,   vertex_2d<float>( -2.534225 , 9.438456 ) 
    ,   vertex_2d<float>( 2.507745 , -9.793956 )
    ,   vertex_2d<float>( 3.080779 , 9.269674 ) 
    );

    r_pos_12 = new Rectangle(
        vertex_2d<float>( -25.845991 , -111.946411 ) 
    ,   vertex_2d<float>( -25.845991 , 73.699738 ) 
    ,   vertex_2d<float>( -24.449694 , -111.946411 ) 
    ,   vertex_2d<float>( -24.449694 , 73.699738 ) 
    );

    r_pos_13 = new Rectangle(
        vertex_2d<float>( -4.156987 , 8.896313 ) 
    ,   vertex_2d<float>( -11.341509 , 31.071060 ) 
    ,   vertex_2d<float>( -6.460926 , 32.652348 ) 
    ,   vertex_2d<float>( 0.723597 , 10.477601 ) 
    );

    r_pos_14 = new Rectangle(
        vertex_2d<float>( -25.845991 , -111.946411 ) 
    ,   vertex_2d<float>( -25.845991 , 73.699738 ) 
    ,   vertex_2d<float>( -24.449694 , -111.946411 ) 
    ,   vertex_2d<float>( -24.449694 , 73.699738 ) 
    );

    r_pos_15 = new Rectangle(
        vertex_2d<float>( -9.439823 , -37.969597 ) 
    ,   vertex_2d<float>( -24.506420 , -20.190165 ) 
    ,   vertex_2d<float>( -5.602348 , -34.717651 ) 
    ,   vertex_2d<float>( -20.668945 , -16.938221 ) 
    );

    r_bend_1 = new Rectangle(
        vertex_2d<float>(1, 2)
    ,   vertex_2d<float>(2, 3)
    ,   vertex_2d<float>(3, 2)
    ,   vertex_2d<float>(2, 1) );

    point_1 = new vertex_2d<float>(3, 3);
    point_2 = new vertex_2d<float>(2, 2);
    point_3 = new vertex_2d<float>(10, 12);
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
    delete r_pos_10;
    delete r_pos_11;
    delete r_pos_12;
    delete r_pos_13;
    delete r_pos_14;
    delete r_pos_15;
    delete r_bend_1;

    delete point_1;
    delete point_2;
    delete point_3;
}
