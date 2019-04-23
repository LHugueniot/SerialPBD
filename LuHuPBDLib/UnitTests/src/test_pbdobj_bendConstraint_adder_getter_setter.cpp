#include "gtest/gtest.h"
#include "pbdobject.h"

TEST(PBDobject, bendingConstraint_adder_getter_setter_methods)
{
    LuHu::PBDobject Test;

    std::vector<glm::vec3> newPointPositions{
        glm::vec3(0,0,0),
                glm::vec3(0,1,0),
                glm::vec3(1,0,0),
                glm::vec3(1,1,0),
                glm::vec3(0,2,0),
                glm::vec3(1,2,0)
    };
    std::vector<glm::vec3> newPointVelocities{
        glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0)
    };

    std::vector<float> newPointMasses
    {
        1.0f,
        2.0f,
        4.0f,
        3.5f,
        10.f,
        7.f
    };

    std::vector<float> newPointInverseMasses
    {
        1.f/1.0f,
        1.f/2.0f,
        1.f/4.0f,
        1.f/3.5f,
        1.f/10.f,
        1.f/7.f
    };

    bool addPointsTest =Test.addPoints(newPointPositions, newPointVelocities, newPointMasses, newPointInverseMasses);

    ASSERT_TRUE(addPointsTest);

    ASSERT_TRUE(Test.addBendingConstraint(0,3,2));
    ASSERT_FALSE(Test.addBendingConstraint(0,3,110));

    EXPECT_EQ(Test.getBendingConstraints()[0], 0);
    EXPECT_EQ(Test.getBendingConstraints()[1], 3);
    EXPECT_EQ(Test.getBendingConstraints()[2], 2);

    std::vector<uint> bendingConstraints{2,3,4,5,2,0};
    std::vector<uint> bendingConstraintsFail1{2,3,4,5,2,0,1};
    std::vector<uint> bendingConstraintsFail2{2,3,4,5,2,20};

    ASSERT_TRUE(Test.addBendingConstraints(bendingConstraints));
    ASSERT_FALSE(Test.addBendingConstraints(bendingConstraintsFail1));
    ASSERT_FALSE(Test.addBendingConstraints(bendingConstraintsFail2));

    std::vector<uint> bendingConstraintTest;

    for(uint i=3; i<Test.getBendingConstraints().size();i++)
    {
        bendingConstraintTest.push_back(Test.getBendingConstraints()[i]);
    }


    EXPECT_EQ(bendingConstraintTest,bendingConstraints);
}
