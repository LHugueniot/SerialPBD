#ifndef SOLVER_H
#define SOLVER_H

#include "pbdobject.h"

namespace LuHu {

enum constraints{
    DISTANCE,
    BENDING,
    DISTANCE_AND_BENDING
};

class solver /// main class for simulation
{
public:

    /// @brief constructor
    /// @param _dampening used for damping velocities
    /// @param _gravity, sets gravity direction and value during simulation

    solver(float _dampening, glm::vec3 _gravity);

    /// @brief get and setter functions

    glm::vec3 getGrav() const;
    float getDamp() const;
    void setGrav(glm::vec3 _grav);
    void setDamp(float _damp);

    /// @brief addPBDobject, adds a PBDobject to the simulation

    std::shared_ptr<LuHu::PBDobject> addPBDobject(std::shared_ptr<PBDobject> _PBDobjectPtr);
    std::shared_ptr<PBDobject> addPBDobject(PBDobject &_PBDobjectPtr);

    /// @brief rmPBDobject, removes a PBDobject to the simulation

    bool rmPBDobject(std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr);
    bool rmPBDobject(uint index);


    bool setPBDobject(uint index, std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr);
    /// @brief getObjects returns a vector of shared pointers to PBDobjects

    std::vector<std::shared_ptr<LuHu::PBDobject>> getObjects() const;

    /// @brief RunSolver, main solver loop that runs based on time step, dt

    void RunSolver(float dt, uint iterations, constraints control);


private:

    std::vector<std::shared_ptr<LuHu::PBDobject>> m_PBDObjects;
    float m_dampening;
    glm::vec3 m_gravity;
};

} //end of namespace
#endif // SOLVER_H
