#include "gtest/gtest.h"
#include "pbdobject.h"

std::string modelName="deCube.obj";

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

TEST(PBDobject, initialise_true_using_obj_mesh)
{
    LuHu::Mesh Test(modelName, 0);
    ASSERT_TRUE(Test.Model);

    auto uniqueIndex = new std::vector<uint>;
    auto uniquePoints = Test.getUniquePoints(Test.m_pointPositions , uniqueIndex);

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
    std::vector<float> objEdgesLengths;

    for(uint i=0; i<uniqueEdges.size(); i++)
    {
        objUniqueEdges.push_back(uniqueEdges[i].p1);
        objUniqueEdges.push_back(uniqueEdges[i].p2);
        glm::vec3 _p1 = uniquePoints[uniqueEdges[i].p1];
        glm::vec3 _p2 = uniquePoints[uniqueEdges[i].p2];
        float restLen = glm::length(_p1 - _p2);
        objEdgesLengths.push_back(restLen);
    }

    ASSERT_EQ(TestObj.getPointPositions(),  uniquePointsPlusPos );
    ASSERT_EQ(TestObj.getFacesPoints(),     objUniqueFaces      );
    ASSERT_EQ(TestObj.getDistanceConstraints(), objUniqueEdges      );
    ASSERT_EQ(TestObj.getDistConRestLength(), objEdgesLengths);
}
