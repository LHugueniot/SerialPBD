#ifndef PBDOBJECT_H
#define PBDOBJECT_H

//glm maths library
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "utilities.h"


namespace LuHu {    //namespace (to be changed)

//using namespace glm;

class PBDobject
{

public:
    //=================================================================================================================================
    /// @brief constructor
    //=================================================================================================================================
    PBDobject();

    //=================================================================================================================================
    /// @brief destructor
    //=================================================================================================================================
    ~PBDobject(){}

    //=================================================================================================================================
    /// @brief initialiser, takes in the model path and name, the mesh index and generates points and constraints based on _originalPos
    //=================================================================================================================================
    bool Initialize(std::string _model, uint _meshIndex, glm::vec3 _originalPos, float _allPointsMass);

    //=================================================================================================================================
    /// @brief initialiser, takes a set of points and sets the model position to _originalPos
    //=================================================================================================================================
    bool Initialize(glm::vec3 _originalPos, std::vector<glm::vec3> _PointPos, std::vector<glm::vec3> _PointVel, std::vector<float> _PointMass);


    void addPoint(glm::vec3 _newPointPos, glm::vec3 _newPointVel, float _newPointMass, float _newPointInvMass);

    bool addPoints(std::vector<glm::vec3> _newPointPos,std::vector<glm::vec3> _newPointVel, std::vector<float> _newPointMass,  std::vector<float> _newPointInvMass);



    const glm::vec3 &getPointPosition(uint _index) const;

    const glm::vec3 &getPointVelocity(uint _index) const;

    const float &getPointMass(uint _index) const;

     const float &getPointInvMass(uint _index) const;

    const std::vector<glm::vec3> &getPointPositions() const;

    const std::vector<glm::vec3> &getPointVelocities() const;

    const std::vector<float> &getPointMasses() const;

    const std::vector<float> &getPointInvMasses() const;



    bool setPointPosition(uint _index, glm::vec3 _newPos);

    bool setPointVelocity(uint _index, glm::vec3 _newVel);

    bool setPointMass(uint _index, float _newMass);

    bool setPointInvMass(uint _index, float _newInvMass);



    //=================================================================================================================================
    /// @brief adds a constraint to PBDObject
    //=================================================================================================================================
    bool addDistConstraint(uint _IndexPoint1, uint _IndexPoint2);

    bool addDistConstraints(std::vector<uint> _IndexPoint);

    const std::vector<uint> &getDistanceConstraints()const;

    const std::vector<float> &getDistConRestLength() const;

    bool addBendingConstraint(uint _IndexPoint1, uint _IndexPoint2, uint _IndexPoint3);

    bool addBendingConstraints(std::vector<uint> _IndexPoint1);

    const std::vector<uint> &getBendingConstraints() const;

    const std::vector<float> &getBenConRestLength() const;

    //=================================================================================================================================
    /// @brief returns faces based on points
    //=================================================================================================================================
    const std::vector<uint> &getFacesPoints() const;

    std::vector<std::vector<uint>> generateColourMap();

    //=================================================================================================================================
    /// @brief returns modelName
    //=================================================================================================================================
    const std::string &getName() const { return m_modelName; }

    //=================================================================================================================================
    /// @brief returns original model world space pos
    //=================================================================================================================================
    const glm::vec3 &getOriginalPos() const { return m_originalPosition; }

private:

    /// @param string storing model name

    std::string m_modelName="";

    /// @param stores original position

    glm::vec3 m_originalPosition;

    std::vector<glm::vec3> m_pointPos;

    std::vector<glm::vec3> m_pointVel;

    std::vector<float> m_pointMass;

    std::vector<float> m_pointInvMass;

    std::vector<uint> m_distanceConstraint;

    std::vector<float> m_distanceConRestLength;

    std::vector<uint> m_bendingConstraint;

    std::vector<float> m_bendingConRestLength;

    std::vector<uint> m_facePoints;
};

}
#endif // PBDOBJECT_H
