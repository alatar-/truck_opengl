#ifndef TEST_RECTANGLE_H
#define TEST_RECTANGLE_H

#include "shared.h"
#include "../rectangle.h"

class TestRectangle : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TestRectangle);

    CPPUNIT_TEST(test_intersection_positives_only_1);
    CPPUNIT_TEST(test_intersection_positives_only_2);
    CPPUNIT_TEST(test_intersection_bent_1);

    CPPUNIT_TEST(test_full_inclusion_positives_only_1);
    CPPUNIT_TEST(test_full_inclusion_positives_only_2);

    CPPUNIT_TEST(test_point_inside_positives_only_1);
    CPPUNIT_TEST(test_point_inside_positives_only_2);
    CPPUNIT_TEST(test_point_inside_bent_1);
    CPPUNIT_TEST(test_point_inside_bent_2);

    CPPUNIT_TEST(test_intersection_negatives_only_1); 
    CPPUNIT_TEST(test_intersection_mixed_1);
    CPPUNIT_TEST(test_intersection_mixed_2);
    CPPUNIT_TEST(test_intersection_mixed_3);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_intersection_positives_only_1();
    void test_intersection_positives_only_2();
    void test_intersection_bent_1();
        
    void test_full_inclusion_positives_only_1();
    void test_full_inclusion_positives_only_2();

    void test_point_inside_positives_only_1();
    void test_point_inside_positives_only_2();
    void test_point_inside_bent_1();
    void test_point_inside_bent_2();

    void test_intersection_negatives_only_1(); 
    void test_intersection_mixed_1();
    void test_intersection_mixed_2();
    void test_intersection_mixed_3();
    void test_intersection_mixed_4();
    void test_intersection_mixed_5();


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
        ,   *r_bend_1;

    vertex_2d *point_1
        ,   *point_2
        ,   *point_3;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRectangle );

#endif
