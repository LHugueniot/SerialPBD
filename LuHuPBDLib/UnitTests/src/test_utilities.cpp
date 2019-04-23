#include "gtest/gtest.h"
#include "utilities.h"

TEST(utilities, getModel)
{
    auto testScene = LuHu::getModel("deCube.obj");

    const aiScene* scene= aiImportFile(
    "/home/s4906706/Documents/AP/Projects/CudaPBD/LuHuPBDLib/models/deCube.obj",
                                       aiProcess_CalcTangentSpace       |
                                       aiProcess_Triangulate            |
                                       aiProcess_JoinIdenticalVertices  |
                                       aiProcess_SortByPType);
    for(uint i=0;i<testScene->mMeshes[0]->mNumVertices; i++){
        EXPECT_EQ(testScene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].x);
        EXPECT_EQ(testScene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].y);
        EXPECT_EQ(testScene->mMeshes[0]->mVertices[i].z, scene->mMeshes[0]->mVertices[i].z);
    }

    auto testSceneFalse = LuHu::getModel("nonExistance.obj");

    EXPECT_EQ(testSceneFalse, nullptr);
}

TEST(utilities, aiToGLM)
{
    aiVector3D testAssimpVec3(10,15,20);
    glm::vec3 testGLMVec3(10,15,20);

    glm::vec3 testResult = LuHu::aiToGLM(testAssimpVec3);

    EXPECT_EQ(testResult, testGLMVec3);
}

TEST(utilities, storePoints)
{
    auto testScene = LuHu::getModel("deCube.obj");

    auto assimpMesh=testScene->mMeshes[0];

    auto meshPoints=LuHu::storePoints(assimpMesh);

    std::vector<glm::vec3> comparisonPoints{
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,-0.5)
    };

    EXPECT_EQ(meshPoints, comparisonPoints);
}

TEST(utilities, storeFaceIndices)
{
    auto testScene = LuHu::getModel("deCube.obj");

    auto assimpMesh=testScene->mMeshes[0];

    auto faceIndices= LuHu::storeFaceIndices(assimpMesh);

    for(uint i=0; i<assimpMesh->mNumFaces; i++ )
    {
        EXPECT_EQ(assimpMesh->mFaces[i].mIndices[0], faceIndices[i].p1);
        EXPECT_EQ(assimpMesh->mFaces[i].mIndices[1], faceIndices[i].p2);
        EXPECT_EQ(assimpMesh->mFaces[i].mIndices[2], faceIndices[i].p3);
    }
}
