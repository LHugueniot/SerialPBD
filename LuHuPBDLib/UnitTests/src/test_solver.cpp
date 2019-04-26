#include "gtest/gtest.h"
#include <PBDLib>

TEST(solver, constructor)
{
    LuHu::solver TestSolver(3.0f, glm::vec3(0,-0.98,0));
    ASSERT_EQ(TestSolver.getDamp(), 3.0f);
    ASSERT_EQ(TestSolver.getGrav(), glm::vec3(0,-0.98,0));

}

TEST(solver, GetterSetterMethods)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.98,0));

    TestSolver.setGrav(glm::vec3(2));
    TestSolver.setDamp(2.0f);

    ASSERT_EQ(TestSolver.getDamp(), 2.0f);
    ASSERT_EQ(TestSolver.getGrav(), glm::vec3(2));

}

TEST(solver, addPBDObject)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.98,0));

    std::shared_ptr<LuHu::PBDobject> TestObj(new LuHu::PBDobject);

    std::string modelName="deCube.obj";

    EXPECT_TRUE(TestObj->Initialize(modelName,0,glm::vec3(0),1.0f));

    EXPECT_TRUE(TestSolver.addPBDobject(TestObj));

    EXPECT_EQ(TestSolver.getObjects()[0], TestObj);
}

TEST(solver, rmPBDObject)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.98,0));

    std::shared_ptr<LuHu::PBDobject> TestObj(new LuHu::PBDobject);

    std::string modelName="deCube.obj";

    EXPECT_TRUE(TestObj->Initialize(modelName,0,glm::vec3(0),1.0f));

    EXPECT_TRUE(TestSolver.addPBDobject(TestObj));

    EXPECT_TRUE(TestSolver.rmPBDobject(TestObj));

    ASSERT_EQ(0,TestSolver.getObjects().size());



    std::shared_ptr<LuHu::PBDobject> TestObj1(new LuHu::PBDobject);

    std::string modelName2="deCube.obj";

    EXPECT_TRUE(TestObj1->Initialize(modelName,0,glm::vec3(0),1.0f));

    EXPECT_FALSE(TestSolver.rmPBDobject(TestObj1));



    EXPECT_TRUE(TestSolver.addPBDobject(TestObj));

    EXPECT_FALSE(TestSolver.rmPBDobject(3));

    EXPECT_TRUE(TestSolver.rmPBDobject(0));

    ASSERT_EQ(0,TestSolver.getObjects().size());
}

TEST(solver, setPBDobject)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.98,0));

    std::shared_ptr<LuHu::PBDobject> TestObj(new LuHu::PBDobject);

    std::string modelName="deCube.obj";

    EXPECT_TRUE(TestObj->Initialize(modelName,0,glm::vec3(0),1.0f));

    EXPECT_FALSE(TestSolver.setPBDobject(0,TestObj));

    EXPECT_TRUE(TestSolver.addPBDobject(TestObj));

    EXPECT_TRUE(TestSolver.setPBDobject(0,TestObj));
}

TEST(solver, RunSolver)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.08,0));

    std::shared_ptr<LuHu::PBDobject> TestObj(new LuHu::PBDobject);

    std::string modelName="deCube.obj";

    EXPECT_TRUE(TestObj->Initialize(modelName,0,glm::vec3(0),1.0f));
    TestObj->addBendingConstraint(0,1,2);
    TestObj->setPointInvMass(0,0);
    EXPECT_TRUE(TestSolver.addPBDobject(TestObj));


    for(uint Tstep=0;Tstep<20; Tstep++)
    {
        TestSolver.RunSolver(5,10);
    }
}

TEST(solver, custommesh)
{
    LuHu::solver TestSolver(1.0f, glm::vec3(0,-0.08,0));

    auto TestObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);

    std::vector<glm::vec3> positions{
        glm::vec3(0),//0
                glm::vec3(0,0,1), //1
                glm::vec3(0,0,2), //2
                glm::vec3(1,0,0), //3
                glm::vec3(1,0,1), //4
                glm::vec3(1,0,2), //5
                glm::vec3(2,0,0), //6
                glm::vec3(2,0,1), //7
                glm::vec3(2,0,2), //8
    };
    std::vector<glm::vec3> velocities{
        glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),

    };

    std::vector<float> masses{1,1,1,1,1,1,1,1,1};
    TestObj->Initialize(glm::vec3(0,1,0),positions, velocities, masses);

    std::vector<uint> distConsraints{0,1,
                                    1,2,
                                    0,3,
                                    0,4,
                                    1,4,
                                    1,5,
                                    2,5,
                                    3,4,
                                    4,5,
                                    3,6,
                                    3,7,
                                    4,7,
                                    4,8,
                                    6,7,
                                    7,8,
                                    5,8
    };

    std::vector<uint> bendConstraints{0,3,6,
                                     1,4,7,
                                     2,5,8,
                                     0,1,2,
                                     3,4,5,
                                     6,7,8};

    TestObj->addDistConstraints(distConsraints);
    TestObj->addBendingConstraints(bendConstraints);
    TestObj->setPointInvMass(0,0);
    TestObj->setPointInvMass(2,0);

    TestSolver.addPBDobject(TestObj);

    for(uint Tstep=0;Tstep<20; Tstep++)
    {
        TestSolver.RunSolver(5,10);
    }
}
