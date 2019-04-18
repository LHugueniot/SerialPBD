#include <gtest/gtest.h>

#include <PBDLib>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace LuHu;

//std::string modelName="deCube.obj";
std::string modelName="/home/s4906706/Documents/PP/PPproj/LuHuPBDLib/PBDLib/models/deCube.obj";
//std::string modelName="/home/datlucien/Documents/PP/PPproj/PBDLib/models/deCube.obj";


TEST(gtest, succes)
{
    ASSERT_EQ(0,0);
}


TEST(PBDobject, initialisefalse)
{
    LuHu::PBDobject TestPBD;

    bool testval=TestPBD.Initialize("model",0 ,glm::vec3(0,0,0));

    ASSERT_EQ(TestPBD.getName(), "model");
    ASSERT_EQ(TestPBD.getOriginalPos(), glm::vec3(0,0,0));
    ASSERT_EQ(testval, false);

}

TEST(PBDobject, initialisetrue)
{
    LuHu::PBDobject TestPBD;

    bool testval=TestPBD.Initialize(modelName,0,glm::vec3(0,0,0));

    ASSERT_EQ(TestPBD.getName(), "model");
    ASSERT_EQ(TestPBD.getOriginalPos(), glm::vec3(0,0,0));
    ASSERT_EQ(testval, true);
}

TEST(point, constructorAndgetAndSetMethods)
{
}

TEST(kernel, getModel)
{    
    auto test=LuHu::getModel(modelName);
    ASSERT_TRUE(test);
}

TEST(kernel, storePoints)
{
    const aiScene* test=LuHu::getModel(modelName);
    LuHu::posVector a=LuHu::storePoints(test,0);
    for(uint i=0; i<test->mNumMeshes; i++ )
    {
        for(uint j=0; j<test->mMeshes[i]->mNumVertices; j++ )
        {
            ASSERT_EQ(test->mMeshes[i]->mVertices[j].x, a[j].x);
            ASSERT_EQ(test->mMeshes[i]->mVertices[j].y, a[j].y);
            ASSERT_EQ(test->mMeshes[i]->mVertices[j].z, a[j].z);
        }
    }
    for(uint j=0; j<test->mMeshes[0]->mNumVertices; j++ )
    {
        LuHu::printVec3(a[j]);
    }
}




