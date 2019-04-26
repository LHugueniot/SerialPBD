#include "gtest/gtest.h"
#include "utilities.h"

TEST(utilities, getModel)
{
    auto testScene = LuHu::getModel("deCube.obj");

    const aiScene* scene= aiImportFile(
    //"/home/s4906706/Documents/AP/Projects/CudaPBD/LuHuPBDLib/models/deCube.obj",
                "/home/datlucien/Documents/AP/CudaPBD/LuHuPBDLib/models/deCube.obj",
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

TEST(utilities ,generateBendingConstraints)
{

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
    std::vector<uint> expectedBendingConstraints{
        0, 1, 5,
        2, 1, 0,
        0, 3, 7,
        6, 3, 0,
        0, 4, 8,
        1, 4, 8,
        3, 4, 8,
        2, 5, 8,
        6, 7, 8
    };

    auto bendingConstraints = LuHu::generateBendConstraints(positions, distConsraints);

    EXPECT_EQ(bendingConstraints, expectedBendingConstraints);
}
