#ifndef TEST_RECTANGLE_H
#define TEST_RECTANGLE_H

#include "shared.h"
#include "../rectangle.h"

class TestRectangle : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TestRectangle);

    CPPUNIT_TEST(test_intersection_positives_only_1_f);
    CPPUNIT_TEST(test_intersection_positives_only_1_r);
    CPPUNIT_TEST(test_intersection_positives_only_2_f);
    CPPUNIT_TEST(test_intersection_positives_only_2_r);

    CPPUNIT_TEST(test_intersection_negatives_only_1_f);
    CPPUNIT_TEST(test_intersection_negatives_only_1_r);
    CPPUNIT_TEST(test_intersection_mixed_1_f);
    CPPUNIT_TEST(test_intersection_mixed_1_r);
    CPPUNIT_TEST(test_intersection_mixed_2_f);
    CPPUNIT_TEST(test_intersection_mixed_2_r);
    CPPUNIT_TEST(test_intersection_mixed_3_f);
    CPPUNIT_TEST(test_intersection_mixed_3_r);
    CPPUNIT_TEST(test_intersection_mixed_4_f);
    CPPUNIT_TEST(test_intersection_mixed_4_r);
    CPPUNIT_TEST(test_intersection_mixed_5_f);
    CPPUNIT_TEST(test_intersection_mixed_5_r);
    CPPUNIT_TEST(test_intersection_mixed_6_f);
    CPPUNIT_TEST(test_intersection_mixed_6_r);

    CPPUNIT_TEST(test_intersection_initial_f);
    CPPUNIT_TEST(test_intersection_initial_r);

    CPPUNIT_TEST(test_intersection_bent_1_f);
    CPPUNIT_TEST(test_intersection_bent_1_r);


    CPPUNIT_TEST(test_full_inclusion_positives_only_1);
    CPPUNIT_TEST(test_full_inclusion_positives_only_2);

    CPPUNIT_TEST(test_point_inside_positives_only_1);
    CPPUNIT_TEST(test_point_inside_positives_only_2);
    CPPUNIT_TEST(test_point_inside_bent_1);
    CPPUNIT_TEST(test_point_inside_bent_2);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_intersection_positives_only_1_f();
    void test_intersection_positives_only_1_r();
    void test_intersection_positives_only_2_f();
    void test_intersection_positives_only_2_r();

    void test_intersection_negatives_only_1_f();
    void test_intersection_negatives_only_1_r();
    void test_intersection_mixed_1_f();
    void test_intersection_mixed_1_r();
    void test_intersection_mixed_2_f();
    void test_intersection_mixed_2_r();
    void test_intersection_mixed_3_f();
    void test_intersection_mixed_3_r();
    void test_intersection_mixed_4_f();
    void test_intersection_mixed_4_r();
    
    void test_intersection_mixed_5_f();
    void test_intersection_mixed_5_r();
    void test_intersection_mixed_6_f();
    void test_intersection_mixed_6_r();

    void test_intersection_initial_f();
    void test_intersection_initial_r();

    void test_intersection_bent_1_f();
    void test_intersection_bent_1_r();
        

    void test_full_inclusion_positives_only_1();
    void test_full_inclusion_positives_only_2();

    void test_point_inside_positives_only_1();
    void test_point_inside_positives_only_2();
    void test_point_inside_bent_1();
    void test_point_inside_bent_2();

private:

    Rectangle *r_pos_1
        ,   *r_pos_2
        ,   *r_pos_3
        ,   *r_pos_4
        ,   *r_pos_5
        ,   *r_pos_6
        ,   *r_pos_7
        ,   *r_pos_8
        ,   *r_pos_9
        ,   *r_pos_10
        ,   *r_pos_11
        ,   *r_pos_12
        ,   *r_pos_13
        ,   *r_pos_14
        ,   *r_pos_15
        ,   *r_bend_1;

    Vertex2D<float>*point_1
        ,   *point_2
        ,   *point_3;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRectangle );

#endif
