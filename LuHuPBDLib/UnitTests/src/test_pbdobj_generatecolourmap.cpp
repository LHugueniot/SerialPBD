#include "gtest/gtest.h"
#include "pbdobject.h"

TEST(PBDobject, generateColourMap)
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

    ASSERT_EQ(_DistanceCons,TestObj.getDistanceConstraints());
    std::vector<std::vector<uint>> colourMap = TestObj.generateColourMap();

    //    for (uint it1=0; it1<colourMap.size(); it1++) {
    //        std::cout<<"Colour "<<it1<<" has these constraints:\n";
    //        for (uint it2=0; it2<colourMap[it1].size(); it2++) {
    //            auto con=colourMap[it1][it2]*2;
    //            auto c1=TestObj.getDistanceConstraints()[con];
    //            auto c2=TestObj.getDistanceConstraints()[con+1];
    //            std::cout<<"c "<<c1<<" "<<c2<<"\n";
    //        }
    //    }

    std::vector<std::vector<uint>> testColourMap{
        {0,3,8},
        {1,2,7},
        {4,6},
        {5}
    };
    ASSERT_EQ(colourMap, testColourMap);
}
