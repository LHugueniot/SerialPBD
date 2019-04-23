#include "pbdobject.h"

namespace LuHu {

PBDobject::PBDobject()
{

}

bool PBDobject::Initialize(std::string _model, uint _meshIndex, glm::vec3 _originalPos,float _allPointsMass)
{
    m_modelName=_model;
    m_originalPosition=_originalPos;

    Mesh mesh(_model,_meshIndex);
    auto uniqueIndex = new std::vector<uint>;
    m_pointPos=mesh.getUniquePoints(mesh.m_pointPositions, uniqueIndex);

    for (uint i=0; i<m_pointPos.size(); i++) {
        m_pointMass.push_back(_allPointsMass);
        m_pointVel.push_back(glm::vec3(0));
    }


    auto uniqueFaces=mesh.getFaceIndices(m_pointPos, *uniqueIndex);

    auto uniqueEdges = mesh.getEdgeIndices(m_pointPos, *uniqueIndex);

    for (uint i=0; i<uniqueFaces.size();i++) {
        m_facePoints.push_back(uniqueFaces[i].p1);
        m_facePoints.push_back(uniqueFaces[i].p2);
        m_facePoints.push_back(uniqueFaces[i].p3);
    }
    for (uint i=0; i<uniqueEdges.size();i++) {
        m_distanceConstraint.push_back(uniqueEdges[i].p1);
        m_distanceConstraint.push_back(uniqueEdges[i].p2);
    }
    for (auto &p : m_pointPos)
    {
        p+=m_originalPosition;
    }

    return true;
}

bool PBDobject::Initialize(glm::vec3 _originalPos, std::vector<glm::vec3> _PointPos, std::vector<glm::vec3> _PointVel, std::vector<float> _PointMass)
{
    if(_PointPos.size() != 0 && _PointVel.size() != 0 && _PointMass.size() != 0 &&
            (_PointPos.size() == _PointVel.size() &&
             _PointPos.size() == _PointMass.size() &&
             _PointMass.size() == _PointVel.size() ))
    {
        m_originalPosition=_originalPos;
        m_pointPos=_PointPos;
        m_pointVel=_PointVel;
        m_pointMass=_PointMass;

        for(auto p : m_pointPos)
        {
            p+=m_originalPosition;

        }
        return true;
    }
    else
    {
        return false;
    }
}

const glm::vec3 PBDobject::getPointPosition(uint _index) const
{
    if(_index < m_pointPos.size()){
        return m_pointPos[_index];
    }
    else {
        std::cout<<"Index is out of range";
        return glm::vec3(0);
    }

}

const glm::vec3 PBDobject::getPointVelocity(uint _index) const
{
    if(_index < m_pointVel.size()){
        return m_pointVel[_index];
    }
    else {
        std::cout<<"Index is out of range";
        return glm::vec3(0);
    }
}

const float PBDobject::getPointMass(uint _index) const
{
    if(_index < m_pointMass.size()){
        return m_pointMass[_index];
    }
    else {
        std::cout<<"Index is out of range";
        return 0.0f;
    }
}

const float PBDobject::getPointInvMass(uint _index) const
{
    if(_index < m_pointInvMass.size()){
        return m_pointInvMass[_index];
    }
    else {
        std::cout<<"Index is out of range";
        return 0.0f;
    }
}

void PBDobject::setPointPosition(uint _index, glm::vec3 _newPos)
{
    m_pointPos[_index]=_newPos;
}

void PBDobject::setPointVelocity(uint _index, glm::vec3 _newVel)
{
    m_pointVel[_index]=_newVel;
}

void PBDobject::setPointMass(uint _index, float _newMass)
{
    m_pointMass[_index]=_newMass;
}

void PBDobject::setPointInvMass(uint _index, float _newInvMass)
{
    m_pointInvMass[_index]=_newInvMass;
}

const std::vector<glm::vec3> PBDobject::getPointPositions() const
{
    return m_pointPos;
}

const std::vector<glm::vec3> PBDobject::getPointVelocities() const
{
    return m_pointVel;
}

const std::vector<float> PBDobject::getPointMasses() const
{
    return m_pointMass;
}

const std::vector<float> PBDobject::getPointInvMasses() const
{
    return m_pointInvMass;
}


void PBDobject::addPoint(glm::vec3 _newPointPos,
                         glm::vec3 _newPointVel,
                         float _newPointMass,
                         float _newPointInvMass)
{
    m_pointPos.push_back(_newPointPos);
    m_pointVel.push_back(_newPointVel);
    m_pointMass.push_back(_newPointMass);
    m_pointInvMass.push_back(_newPointInvMass);
}

bool PBDobject::addPoints(std::vector<glm::vec3> _newPointPos,
                          std::vector<glm::vec3> _newPointVel,
                          std::vector<float> _newPointMass,
                          std::vector<float> _newPointInvMass)
{
    if( _newPointPos.size() == _newPointVel.size() &&
            _newPointPos.size() == _newPointMass.size() &&
            _newPointPos.size()== _newPointInvMass.size())
    {
        m_pointPos.insert(m_pointPos.end(), _newPointPos.begin(), _newPointPos.end());
        m_pointVel.insert(m_pointVel.end(), _newPointVel.begin(), _newPointVel.end());
        m_pointMass.insert(m_pointMass.end(), _newPointMass.begin(), _newPointMass.end());
        m_pointInvMass.insert(m_pointInvMass.end(), _newPointInvMass.begin(), _newPointInvMass.end());
        return true;
    }
    else
    {
        return false;
    }
}

void PBDobject::addDistConstraint(uint _IndexPoint1, uint _IndexPoint2)
{
    m_distanceConstraint.push_back(_IndexPoint1);
    m_distanceConstraint.push_back(_IndexPoint2);
}

bool PBDobject::addDistConstraints(std::vector<uint> _IndexPoint)
{

    if(_IndexPoint.size()%2==0)
    {
        m_distanceConstraint.insert(m_distanceConstraint.end(), _IndexPoint.begin(), _IndexPoint.end());

        for(uint i=0;i<_IndexPoint.size(); i+=2)
        {
            glm::vec3 _p1 = m_pointPos[_IndexPoint[i]];
            glm::vec3 _p2 = m_pointPos[_IndexPoint[i+1]];
            float restLen = glm::length(_p1 - _p2);
            m_distanceConRestLength.push_back(restLen);
        }
        return true;
    }
    else
    {
        return false;
    }
}

const std::vector<uint> PBDobject::getDistConstraints() const
{
    return m_distanceConstraint;
}

const std::vector<float> PBDobject::getDistConRestLength() const
{
    return m_distanceConRestLength;
}

void PBDobject::addBendConstraint(uint _IndexPoint1, uint _IndexPoint2, uint _IndexPoint3)
{
    m_bendingConstraint.push_back(_IndexPoint1);
    m_bendingConstraint.push_back(_IndexPoint2);
    m_bendingConstraint.push_back(_IndexPoint3);
}

bool PBDobject::addBendConstraints(std::vector<uint> _IndexPoint)
{
    if(_IndexPoint.size()%3==0)
    {
        m_bendingConstraint.insert(m_bendingConstraint.end(), _IndexPoint.begin(), _IndexPoint.end());
        return true;
    }
    else
    {
        std::cout<<"Bending constraint"<<_IndexPoint.size()<<"\n";
        return false;
    }
}

const std::vector<uint> PBDobject::getBendingConstraints() const
{
    return m_bendingConstraint;
}

const std::vector<uint> PBDobject::getFacesPoints() const
{
    return m_facePoints;
}

std::vector<std::vector<uint>> PBDobject::generateColourMap()
{
    //Create distance Constraint Node Graph
    std::vector<std::vector<uint>> nodeGraph(m_distanceConstraint.size()/2);

    for(uint i=0; i<m_distanceConstraint.size()/2; i++)
    {
        uint ii=i*2;

        for(uint j=0; j<m_distanceConstraint.size()/2; j++)
        {
            uint jj=j*2;

            if(     (m_distanceConstraint[jj]    == m_distanceConstraint[ii]    ||
                     m_distanceConstraint[jj+1]   == m_distanceConstraint[ii]    ||
                     m_distanceConstraint[jj]     == m_distanceConstraint[ii+1]  ||
                     m_distanceConstraint[jj+1]   == m_distanceConstraint[ii+1]  )   )
            {
                if(i!=j)
                {
                    nodeGraph[i].push_back(j);
                }
            }
        }
    }

    for(uint k =0; k< nodeGraph.size(); k++)
    {
        std::cout<<"Constraint "<<m_distanceConstraint[k*2]<<" "<<m_distanceConstraint[k*2+1]<<" is connected to: \n";
        for(uint f =0; f< nodeGraph[k].size(); f++)
        {
            std::cout<<m_distanceConstraint[nodeGraph[k][f]*2]<<" "<<m_distanceConstraint[nodeGraph[k][f]*2+1]<<"\n";
        }
    }


    uint nodegraphsize=0;
    for(auto b : nodeGraph)
    {
        nodegraphsize+=b.size();
    }

    std::cout<<"\nNodeGraph is of size:"<<nodegraphsize<<"\n\n";

    //Create Colour Graph based
    std::vector<int> colourIndex(nodeGraph.size());

    for(uint t=0 ;t< colourIndex.size();t++)
    {
        colourIndex[t]=-1;
    }
    colourIndex[0]=0;

    std::vector<std::vector<uint>> colourGraph(1);
    colourGraph[0].push_back(0);

    for(uint k =1; k< nodeGraph.size(); k++)
    {
        std::vector<bool> whitelist(colourGraph.size());

        for(uint t=0 ;t< whitelist.size();t++)
        {
            whitelist[t]=true;
        }

        for(uint l=0 ; l<nodeGraph[k].size(); l++)
        {
            if(colourIndex[nodeGraph[k][l]]>-1 )
            {
                whitelist[colourIndex[nodeGraph[k][l]]]=false;
            }
        }

        uint bestColour=UINT_MAX;

        for(uint t=0;t<whitelist.size(); t++)
        {
            if(whitelist[t]==true)
            {
                if(colourGraph[t].size()<bestColour)
                {
                    bestColour=t;
                }
            }
        }

        if(bestColour==UINT_MAX)
        {
            colourGraph.push_back(std::vector<uint>{k});
            colourIndex[k]=colourGraph.size()-1;
        }
        else
        {
            colourGraph[bestColour].push_back(k);
            colourIndex[k]=bestColour;
        }

    }
    return colourGraph;

}

} // end of namespace

