#include <gtest/gtest.h>

#include <PBDLib>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace LuHu;

//std::string modelName="deCube.obj";
std::string modelName="deCube.obj";
//std::string modelName="/home/datlucien/Documents/PP/PPproj/PBDLib/models/deCube.obj";


TEST(gtest, succes)
{
    ASSERT_EQ(0,0);
}


TEST(PBDobject, initialise_true_using_given_positions)
{
    LuHu::PBDobject Test;

    glm::vec3 originalPosition(0,1,0);

    std::vector<glm::vec3> pointPos{
                glm::vec3(0,0,0),
                glm::vec3(0,1,0),
                glm::vec3(1,0,0),
                glm::vec3(1,1,0),
                glm::vec3(0,2,0),
                glm::vec3(1,2,0)
    };

    std::vector<glm::vec3> pointPosPlusOriginalPos;

    for (auto p : pointPos) {
        pointPosPlusOriginalPos.push_back(p+originalPosition);
    }

    std::vector<glm::vec3> pointVel{
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0)
    };

    std::vector<float> pointMass{ 0,0,0,0,0,0 };

    bool testval=Test.Initialize(originalPosition, pointPos, pointVel,pointMass);

    ASSERT_TRUE(testval);
    ASSERT_EQ(originalPosition, Test.getOriginalPos());

}

TEST(PBDobject, initialise_false_using_given_positions)
{
    LuHu::PBDobject Test;

    glm::vec3 originalPosition(0,1,0);

    std::vector<glm::vec3> pointPos{
                glm::vec3(0,0,0),
                glm::vec3(0,1,0),
                glm::vec3(1,0,0),
                glm::vec3(1,1,0),
                glm::vec3(0,2,0),
                glm::vec3(1,2,0)
    };

    std::vector<glm::vec3> pointPosPlusOriginalPos;

    for (auto p : pointPos) {
        pointPosPlusOriginalPos.push_back(p+originalPosition);
    }

    std::vector<glm::vec3> pointVel{
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
    };

    std::vector<float> pointMass{ 0,0,0,0,0,0 };

    bool testval=Test.Initialize(originalPosition, pointPos, pointVel,pointMass);

    ASSERT_FALSE(testval);
}


TEST(Mesh, Constructor)
{
    LuHu::Mesh Test("deCube.obj", 0);

    ASSERT_EQ(Test.m_pointPositions.size(), 24);
    ASSERT_EQ(Test.m_modelName, "deCube.obj");
    ASSERT_EQ(Test.m_faceIndices.size(),12);
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

TEST(PBDobject, initialise_true_using_obj_mesh)
{
    LuHu::Mesh Test(modelName, 0);
    ASSERT_TRUE(Test.Model);

    auto uniqueIndex = new std::vector<uint>;
    auto uniquePoints = Test.getUniquePoints(Test.m_pointPositions , uniqueIndex);
    auto numFaceIndex = Test.m_faceIndices.size();

    auto uniqueFaces = Test.getFaceIndices(uniquePoints, *uniqueIndex);
    auto uniqueEdges = Test.getEdgeIndices(uniquePoints, *uniqueIndex);

    LuHu::PBDobject TestObj;
    glm::vec3 oiriginalPos(0,0,1);
    TestObj.Initialize(modelName, 0, oiriginalPos, 1.0f);

    ASSERT_EQ(TestObj.getName(),modelName);

    std::vector<glm::vec3> uniquePointsPlusPos;

    for(auto p : uniquePoints)
    {
        uniquePointsPlusPos.push_back(p + oiriginalPos);
    }

    std::vector<uint> objUniqueFaces;

    for(auto p : uniqueFaces)
    {
        objUniqueFaces.push_back(p.p1);
        objUniqueFaces.push_back(p.p2);
        objUniqueFaces.push_back(p.p3);
    }

    std::vector<uint> objUniqueEdges;

    for(auto p : uniqueEdges)
    {
        objUniqueEdges.push_back(p.p1);
        objUniqueEdges.push_back(p.p2);
    }

    ASSERT_EQ(TestObj.getPointPositions(),  uniquePointsPlusPos );
    ASSERT_EQ(TestObj.getFacesPoints(),     objUniqueFaces      );
    ASSERT_EQ(TestObj.getDistConstraints(), objUniqueEdges      );

}


TEST(PBDobject, InitializeWithManualDataInput)
{
    LuHu::PBDobject TestObj;
    glm::vec3 oiriginalPos(0,0,1);
    //bool PBDobject::Initialize(glm::vec3 _originalPos, std::vector<glm::vec3> _PointPos, std::vector<glm::vec3> _PointVel, std::vector<float> _PointMass)
    std::vector<glm::vec3> _PointPos={
        glm::vec3(0,0,0),
        glm::vec3(1,0,0),
        glm::vec3(2,0,0),
        glm::vec3(0,1,0),
        glm::vec3(1,1,0),
        glm::vec3(2,1,0)
    };

    std::vector<glm::vec3> _PointVel={
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(0,0,0)
    };
    std::vector<float> _PointMass={
        1,1,1,1,1,1
    };

    TestObj.Initialize(oiriginalPos, _PointPos, _PointVel, _PointMass);

    std::vector<uint> _DistanceCons={
        0,1 ,
        1,2 ,
        0,3 ,
        3,4 ,
        0,4 ,
        1,4 ,
        1,5 ,
        4,5 ,
        2,5
    };

    TestObj.addDistConstraints(_DistanceCons);

    ASSERT_EQ(_DistanceCons,TestObj.getDistConstraints());
    std::vector<std::vector<uint>> colourMap = TestObj.generateColourMap();

    for (uint it1=0; it1<colourMap.size(); it1++) {
        std::cout<<"Colour "<<it1<<" has these constraints:\n";
        for (uint it2=0; it2<colourMap[it1].size(); it2++) {
            auto con=colourMap[it1][it2]*2;
            auto c1=TestObj.getDistConstraints()[con];
            auto c2=TestObj.getDistConstraints()[con+1];
            std::cout<<"c "<<c1<<" "<<c2<<"\n";
        }
    }
}
