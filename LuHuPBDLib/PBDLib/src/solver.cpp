#include "solver.h"
#include <algorithm>


namespace LuHu {

solver::solver(float _damp, glm::vec3 _grav)
{
    m_gravity=_grav;
    m_dampening=_damp;
}

glm::vec3 solver::getGrav() const
{
    return m_gravity;
}

float solver::getDamp() const
{
    return m_dampening;
}

void solver::setGrav(glm::vec3 _grav)
{
    m_gravity=_grav;
}

void solver::setDamp(float _damp)
{
    m_dampening=_damp;
}

bool solver::addPBDobject(std::shared_ptr<LuHu::PBDobject> &_PBDobjectPtr)
{
    if(&_PBDobjectPtr)
    {
        m_PBDObjects.push_back(_PBDobjectPtr);
        return true;
    }
    else{
        return false;
    }
}

bool solver::addPBDobject(PBDobject & _PBDobjectPtr)
{
    if(&_PBDobjectPtr)
    {
        m_PBDObjects.push_back(std::make_shared<PBDobject>(_PBDobjectPtr));
        return true;
    }
    else{
        return false;
    }
}

bool solver::rmPBDobject(std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr)
{
    if(_PBDobjectPtr)
    {
        auto it = std::find(m_PBDObjects.begin(), m_PBDObjects.end(),_PBDobjectPtr);
        m_PBDObjects.erase(it);
        return true;
    }
    else{
        return false;
    }
}
bool solver::rmPBDobject(uint index)
{
    if(index<=m_PBDObjects.size())
    {
        m_PBDObjects.erase(m_PBDObjects.begin()+index);
        return true;
    }
    else
    {
        return false;
    }

}

std::vector<std::shared_ptr<LuHu::PBDobject>> solver::getObjects() const
{
    return m_PBDObjects;
}

void solver::RunSolver(float dt)
{
    float inv_dt = 1/dt;
    for(std::shared_ptr<PBDobject> & pObject :m_PBDObjects)
    {
        auto objPPos= pObject->getPointPositions();
        auto objPVel= pObject->getPointVelocities();
        auto objPMasse= pObject->getPointMasses();
        auto objPInvMass= pObject->getPointInvMasses();

        auto objDistCons = pObject->getDistanceConstraints();
        auto objDistConsRestLength = pObject->getDistConRestLength();
        auto objBendingCons = pObject->getBendingConstraints();

        uint numPoints = objPPos.size();

        std::vector<glm::vec3> projectedPosition(numPoints);

        for(uint i=0; i<numPoints; i++)
        {

            pObject->setPointVelocity(i, ( pObject->getPointVelocity(i)+
                    (m_gravity*
                     m_dampening*
                     pObject->getPointInvMass(i)*
                     inv_dt)));

            projectedPosition[i]=   pObject->getPointPosition(i)+
                                    pObject->getPointVelocity(i)*
                                    inv_dt;
        }

        for (uint k=0; k<5; k++)
        {
            for (uint j=0; j<objDistCons.size(); j+=2)
            {
                auto m_p1=objDistCons[j];
                auto m_p2=objDistCons[j+1];
                glm::vec3 dir = projectedPosition[m_p1] - projectedPosition[m_p2];

                float len = glm::length(dir);
                float inv_mass=objPInvMass[m_p1] + objPInvMass[m_p2];

                projectedPosition[m_p1]= (
                            (projectedPosition[m_p1] -
                             (objPInvMass[m_p1]/inv_mass)*
                             (len - objDistConsRestLength[j/2])*
                             (dir/len)
                             )
                            );


                projectedPosition[m_p2]= (
                            (projectedPosition[m_p2] +
                             (objPInvMass[m_p2]/inv_mass)*
                             (len - objDistConsRestLength[j/2])*
                             (dir/len)
                             )
                            );
            }
        }

        for (uint i=0; i<numPoints; i++)
        {
            pObject->setPointVelocity( i, (projectedPosition[i] - objPPos[i])/ inv_dt );
            pObject->setPointPosition( i, projectedPosition[i] + objPPos[i] );
        }

    }
}

} // end of namespace

