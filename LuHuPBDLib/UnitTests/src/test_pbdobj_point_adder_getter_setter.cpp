#include "gtest/gtest.h"
#include "pbdobject.h"

TEST(PBDobject, point_adder_getter_setter_methods)
{
    LuHu::PBDobject Test;

    Test.addPoint(glm::vec3(4),glm::vec3(0),1.0,1.0);

    ASSERT_EQ(Test.getPointPosition(0) , glm::vec3(4));
    ASSERT_EQ(Test.getPointVelocity(0) , glm::vec3(0));
    ASSERT_EQ(Test.getPointMass(0) , 1.0f);
    ASSERT_EQ(Test.getPointInvMass(0) , 1.0f);

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

    std::vector<glm::vec3> TestPointPositions{glm::vec3(4)};

    TestPointPositions.insert(TestPointPositions.end(),
                              newPointPositions.begin(),
                              newPointPositions.end());

    ASSERT_EQ( Test.getPointPositions(), TestPointPositions);

    std::vector<glm::vec3> TestPointVelocities{glm::vec3(0)};

    TestPointVelocities.insert(TestPointVelocities.end(),
                               newPointVelocities.begin(),
                               newPointVelocities.end());

    ASSERT_EQ( Test.getPointVelocities() ,TestPointVelocities );

    std::vector<float> TestPointMasses{1.0f};

    TestPointMasses.insert(TestPointMasses.end(),
                               newPointMasses.begin(),
                               newPointMasses.end());

    ASSERT_EQ( Test.getPointMasses(), TestPointMasses);

    std::vector<float> TestPointInverseMasses{1.0f};

    TestPointInverseMasses.insert(TestPointInverseMasses.end(),
                               newPointInverseMasses.begin(),
                               newPointInverseMasses.end());

    ASSERT_EQ( Test.getPointInvMasses(), TestPointInverseMasses );

    ASSERT_FALSE(Test.setPointPosition(20,glm::vec3(5)));

    ASSERT_TRUE(Test.setPointPosition(3,glm::vec3(5)));
    ASSERT_EQ(Test.getPointPosition(3), glm::vec3(5));

    ASSERT_FALSE(Test.setPointVelocity(324,glm::vec3(10)));

    ASSERT_TRUE(Test.setPointVelocity(5,glm::vec3(10)));
    ASSERT_EQ(Test.getPointVelocity(5), glm::vec3(10));

    ASSERT_FALSE(Test.setPointMass(20,50));

    ASSERT_TRUE(Test.setPointMass(3,50));
    ASSERT_EQ(Test.getPointMass(3), 50);

    ASSERT_FALSE(Test.setPointMass(22130, 50));

    ASSERT_TRUE(Test.setPointInvMass(2, 2000));
    ASSERT_EQ(Test.getPointInvMass(2), 2000);

}
