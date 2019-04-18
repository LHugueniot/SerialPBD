#ifndef PBDOBJECT_H
#define PBDOBJECT_H

#include "kernel.h"

//glm maths library
#include <glm/glm.hpp>
#include <string>
#include <vector>


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
    bool Initialize(std::string _model, uint _meshIndex, glm::vec3 _originalPos);

    //=================================================================================================================================
    /// @brief initialiser, takes a set of points and sets the model position to _originalPos
    //=================================================================================================================================
    bool Initialize(glm::vec3 _originalPos, std::vector<glm::vec3> _PointPos, std::vector<glm::vec3> _PointVel, std::vector<float> _PointMass);

    //=================================================================================================================================
    /// @brief returns pointers to the points stored in PBDObject
    //=================================================================================================================================
    const glm::vec3 getPointPosition(uint _index) const;

    const glm::vec3 getPointVelocity(uint _index) const;

    const std::vector<glm::vec3> getPointPositions() const;

    const std::vector<glm::vec3> getPointVelocities() const;

    const float getPointMass(uint _index) const;

    const float getPointInvMass(uint _index) const;

    const std::vector<float> getPointMasses() const;

    const std::vector<float> getPointInvMasses() const;

    //=================================================================================================================================
    /// @brief returns pointers to the constraints stored in PBDObject
    //=================================================================================================================================
    const std::vector<uint> getConstraint() const;

    //=================================================================================================================================
    /// @brief adds a point to PBDObject
    //=================================================================================================================================
    bool addPoint(glm::vec3 _newPointPos, glm::vec3 _newPointVel, float _newPointMass, float _newPointInvMass);

    bool addPoints(std::vector<glm::vec3> _newPointPos,std::vector<glm::vec3> _newPointVel, std::vector<float> _newPointMass,  std::vector<float> _newPointInvMass);

    //=================================================================================================================================
    /// @brief adds a constraint to PBDObject
    //=================================================================================================================================
    bool addDistConstraint(uint _IndexPoint1, uint _IndexPoint2);

    bool addDistConstraints(std::vector<uint> _IndexPoint);

    bool addBendConstraint(uint _IndexPoint1, uint _IndexPoint2);

    bool addBendConstraints(std::vector<uint> _IndexPoint1, std::vector<uint> _IndexPoint2, std::vector<uint> _IndexPoint3);

    //=================================================================================================================================
    /// @brief returns faces based on points
    //=================================================================================================================================
    const std::vector<uint> getFacesPoints() const;

    //=================================================================================================================================
    /// @brief returns modelName
    //=================================================================================================================================
    std::string getName() const { return m_modelName; }

    //=================================================================================================================================
    /// @brief returns original model world space pos
    //=================================================================================================================================
    glm::vec3 getOriginalPos() const
    {
        return m_originalPosition;
    }

private:

    /// @param string storing model name

    std::string m_modelName;

    /// @param stores original position

    glm::vec3 m_originalPosition;

    std::vector<glm::vec3> m_pointPos;

    std::vector<glm::vec3> m_pointVel;

    std::vector<float> m_pointMass;

    std::vector<float> m_pointInvMass;

    std::vector<uint> m_distanceConstraint;

    std::vector<uint> m_bendingConstraint;

    std::vector<uint> m_facePoints;
};

}
#endif // PBDOBJECT_H
