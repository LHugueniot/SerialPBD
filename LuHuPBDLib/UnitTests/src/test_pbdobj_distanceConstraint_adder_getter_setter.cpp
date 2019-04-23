#include "gtest/gtest.h"
#include "pbdobject.h"

TEST(PBDobject, distanceConstraint_adder_getter_setter_methods)
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

    Test.addDistConstraint(0,5);

    ASSERT_EQ(Test.getDistanceConstraints()[0],0);
    ASSERT_EQ(Test.getDistanceConstraints()[1],5);

    float testRestLength=glm::length(glm::vec3(1,2,0) - glm::vec3(0,0,0));

    ASSERT_EQ(Test.getDistConRestLength()[0],testRestLength);

    std::vector<uint> distanceConstraints{2,3,4,5};
    std::vector<uint> distanceConstraintsFail1{2,3,4,5,3};
    std::vector<uint> distanceConstraintsFail2{2,3,4,20};

    EXPECT_FALSE(Test.addDistConstraints(distanceConstraintsFail1));
    EXPECT_FALSE(Test.addDistConstraints(distanceConstraintsFail2));

    EXPECT_TRUE(Test.addDistConstraints(distanceConstraints));

    ASSERT_EQ(Test.getDistanceConstraints()[2],2);
    ASSERT_EQ(Test.getDistanceConstraints()[3],3);
    ASSERT_EQ(Test.getDistanceConstraints()[4],4);
    ASSERT_EQ(Test.getDistanceConstraints()[5],5);
}
