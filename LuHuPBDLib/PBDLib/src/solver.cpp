#include "solver.h"
#include <algorithm>
#include <math.h>

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

std::shared_ptr<PBDobject> solver::addPBDobject(std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr)
{
        m_PBDObjects.push_back(_PBDobjectPtr);
        return _PBDobjectPtr;
}

std::shared_ptr<LuHu::PBDobject> solver::addPBDobject(PBDobject & _PBDobjectPtr)
{
        auto returnptr = std::make_shared<PBDobject>(_PBDobjectPtr);
        m_PBDObjects.push_back(returnptr);
        return returnptr;
}

bool solver::rmPBDobject(std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr)
{
    auto it = std::find(m_PBDObjects.begin(), m_PBDObjects.end(),_PBDobjectPtr);
    if(it!=m_PBDObjects.end())
    {
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

bool solver::setPBDobject(uint index, std::shared_ptr<LuHu::PBDobject> _PBDobjectPtr)
{
    if(index<m_PBDObjects.size())
    {
        m_PBDObjects.insert(m_PBDObjects.begin()+index, _PBDobjectPtr);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<LuHu::PBDobject>> solver::getObjects() const
{
    return m_PBDObjects;
}

void solver::RunSolver(float dt, uint iterations)
{
    float inv_dt = 1/dt;
    for(std::shared_ptr<PBDobject> & pObject :m_PBDObjects)
    {
        auto& objPPos= pObject->getPointPositions();
        auto& objPVel= pObject->getPointVelocities();
        auto& objPMasse= pObject->getPointMasses();
        auto& objPInvMass= pObject->getPointInvMasses();

        auto & objDistCons = pObject->getDistanceConstraints();
        auto & objDistConsRestLength = pObject->getDistConRestLength();
        auto & objBendingCons = pObject->getBendingConstraints();
        auto & objBendingConsRestLength = pObject->getBenConRestLength();

        uint numPoints = objPPos.size();

        std::vector<glm::vec3> projectedPosition(numPoints);

        for(uint i=0; i<numPoints; i++)
        {
            pObject->setPointVelocity(i, ( objPVel[i]+
                    (m_gravity*
                     m_dampening*
                     objPInvMass[i]*
                     inv_dt)));

            projectedPosition[i]=   (objPPos[i]+
                                    objPVel[i]*
                                    inv_dt);
        }

        for (uint k=0; k<iterations; k++)
        {
            for (uint j=0; j<objDistCons.size(); j+=2)
            {
                auto i1=objDistCons[j];
                auto i2=objDistCons[j+1];
                glm::vec3 dir = projectedPosition[i1] - projectedPosition[i2];

                float len = glm::length(dir);
                float W=objPInvMass[i1] + objPInvMass[i2];

                if(W<=0 || len<=0)continue;

                float itf=iterations;
                float kprime=(1- std::pow((1.0f-0.5f),(1/itf)) );

                projectedPosition[i1]+= (
                            (-(objPInvMass[i1]/W)*
                             (len - objDistConsRestLength[j/2])*
                             (dir/len)
                             )*kprime
                            );

                projectedPosition[i2]+= (
                            ((objPInvMass[i2]/W)*
                             (len - objDistConsRestLength[j/2])*
                             (dir/len)
                             )*kprime
                            );
            }
            for (uint j=0; j<objBendingCons.size(); j+=3)
            {

                auto i1=objBendingCons[j];
                auto i2=objBendingCons[j+1];
                auto i3=objBendingCons[j+2];

                auto p1=projectedPosition[i1];
                auto p2=projectedPosition[i2];
                auto p3=projectedPosition[i3];

                auto w1=objPInvMass[i1];
                auto w2=objPInvMass[i2];
                auto w3=objPInvMass[i3];

                float third=1/3;
                glm::vec3 centroid = third *(p1 + p2 +p3);
                glm::vec3 NC=  (p2 - centroid);

                float len=glm::length(NC);

                float W = w1+w2*2+w3;

                float magniDiff= ((0.5f + objBendingConsRestLength[j/3]) /
                            len);

                if(W<=0 || len <= 0)continue;

                projectedPosition[i1]+=((2*w1/W) *
                                        (NC) *
                                        (1- magniDiff));

                projectedPosition[i2]+=((-4*w2/W) *
                                        (NC) *
                                        (1- magniDiff));

                projectedPosition[i3]+=((2*w3/W) *
                                        (NC) *
                                        (1- magniDiff));

            }

        }

        for (uint i=0; i<numPoints; i++)
        {
            pObject->setPointVelocity( i, (projectedPosition[i] - objPPos[i])/ inv_dt );
            pObject->setPointPosition( i, projectedPosition[i]);
        }

    }
}

} // end of namespace

