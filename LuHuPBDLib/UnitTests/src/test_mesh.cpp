#include "gtest/gtest.h"
#include "utilities.h"

TEST(Mesh, Constructor)
{
    LuHu::Mesh Test("deCube.obj", 0);

    ASSERT_EQ(Test.m_pointPositions.size(), 24);
    ASSERT_EQ(Test.m_modelName, "deCube.obj");
    ASSERT_EQ(Test.m_faceIndices.size(),12);
}


TEST(Mesh, getEdgeIndices)
{
    LuHu::Mesh Test("deCube.obj", 0);
    ASSERT_TRUE(Test.Model);

    auto uniqueIndex = new std::vector<uint>;
    auto uniquePoints = Test.getUniquePoints(Test.m_pointPositions , uniqueIndex);
    auto numFaceIndex = Test.m_faceIndices.size();

    auto uniqueFaces = Test.getFaceIndices(uniquePoints, *uniqueIndex);
    auto uniqueEdges = Test.getEdgeIndices(uniquePoints, *uniqueIndex);

    ASSERT_EQ(uniqueEdges.size(), 18);
}

TEST(Mesh, getFaceIndices)
{
    LuHu::Mesh Test("deCube.obj", 0);
    ASSERT_TRUE(Test.Model);

    auto uniqueIndex = new std::vector<uint>;
    auto uniquePoints = Test.getUniquePoints(Test.m_pointPositions , uniqueIndex);
    auto numFaceIndex = Test.m_faceIndices.size();

    auto uniqueFaces = Test.getFaceIndices(uniquePoints, *uniqueIndex);

    auto numIndex = uniqueIndex->size();

    auto NumFaces= uniqueFaces.size();
    ASSERT_EQ(NumFaces, 12 );

    std::vector<glm::vec3> assimpFaceIndexTest;
    std::vector<glm::vec3> uniqueFaceIndexTest;

    for (uint i= 0; i<numFaceIndex; i++) {

        assimpFaceIndexTest.push_back(Test.m_pointPositions[Test.m_faceIndices[i].p1]);
        assimpFaceIndexTest.push_back(Test.m_pointPositions[Test.m_faceIndices[i].p2]);
        assimpFaceIndexTest.push_back(Test.m_pointPositions[Test.m_faceIndices[i].p3]);
    }

    for (uint i= 0; i<NumFaces; i++) {

        uniqueFaceIndexTest.push_back(uniquePoints[uniqueFaces[i].p1]);
        uniqueFaceIndexTest.push_back(uniquePoints[uniqueFaces[i].p2]);
        uniqueFaceIndexTest.push_back(uniquePoints[uniqueFaces[i].p3]);
    }
    ASSERT_EQ(uniqueFaceIndexTest, assimpFaceIndexTest );
}

TEST(Mesh, getUniquePoints)
{
    LuHu::Mesh Test("deCube.obj", 0);
    ASSERT_TRUE(Test.Model);

    auto uniqueIndex = new std::vector<uint>;
    auto uniquePoints = Test.getUniquePoints(Test.m_pointPositions , uniqueIndex);

    ASSERT_EQ(uniquePoints.size(), 8);
    ASSERT_EQ(uniqueIndex->size(), Test.m_pointPositions.size());
    for (uint i =0; i< uniquePoints.size(); i++) {
        //printVec3(uniquePoints[i]);
    }

    std::vector<glm::vec3> PosIndexTest;

    for (uint j =0; j< uniqueIndex->size(); j++) {
        //std::cout<<uniqueIndex->data()[j]<<"\n";
        PosIndexTest.push_back(uniquePoints[uniqueIndex->data()[j]]);
    }

    ASSERT_EQ(Test.m_pointPositions, PosIndexTest);
    for (uint k=0; k<PosIndexTest.size(); k++) {

        //LuHu::printVec3(PosIndexTest[k]);
        //LuHu::printVec3(Test.m_pointPositions[k]);
        //std::cout<<"\n";
    }

}
