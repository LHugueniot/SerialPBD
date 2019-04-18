#include "pbdobject.h"

#include "indexDuo.h"
#include <experimental/filesystem>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace LuHu {

const aiScene* getModel(std::string _model);
std::vector<glm::vec3> storePoints(aiMesh _assimpMesh);
std::vector<indexTrio> storeFaceIndices(aiMesh _assimpMesh);

struct Mesh{

    aiScene*  Model;
    aiMesh m_assimpMesh;

    Mesh(std::string _model, uint _index){ Model=getModel(_model);
                                            m_assimpMesh=Model->mMeshes[_index];
                                            m_pointPositions=storePoints(m_assimpMesh);
                                            m_faceIndices=storeFaceIndices(m_assimpMesh);
                                         }
    std::vector<glm::vec3> m_pointPositions;
    std::vector<indexTrio> m_faceIndices;


};

const aiScene* getModel(std::string _model)
{
    std::string path = std::experimental::filesystem::current_path();
    std::string finalPath = path+ "/../PBDLib/models/" +_model;
    std::cout<<finalPath<<std::endl;
    const aiScene* scene= aiImportFile(_model.c_str(),  aiProcess_CalcTangentSpace       |
                                       aiProcess_Triangulate            |
                                       aiProcess_JoinIdenticalVertices  |
                                       aiProcess_SortByPType);
    if(!scene){
        std::cout<<aiGetErrorString()<<"\n";
        return NULL;
    }
    return scene;
}

void printVec3(glm::vec3 _vec)
{
    std::cout<<"x="<<_vec.x<<" y="<<_vec.y<<" z="<<_vec.z<<"\n";
}

glm::vec3 aiToGLM(aiVector3D mVertices)
{
    return glm::vec3(mVertices.x,mVertices.y,mVertices.z);
}

std::vector<glm::vec3> storePoints(aiMesh _assimpMesh)
{
    std::vector<glm::vec3> temp;
    for(uint j=0; j<_assimpMesh->mNumVertices; j++)
    {
        temp.push_back(aiToGLM(_assimpMesh->mVertices[j]));
    }
    return temp;
}

std::vector<indexTrio> storeFaceIndices(aiMesh _assimpMesh)
{
    std::vector<indexTrio> temp;
    auto faces=_assimpMesh.mFaces;
    for(uint j=0; j<faces->mNumIndices; j++)
    {
        temp.push_back(faces->mIndices[j]);
    }
    return temp;
}

bool compare(glm::vec3 temp, std::vector<glm::vec3> bigVec)
{
    for(auto p: bigVec)
    {
        if(temp==p)
        {
            return true;
        }
    }
    return false;
}

bool compare( posVector bigVec1,  std::vector<glm::vec3> bigVec2)
{
    if(bigVec1.size()%2==0 && bigVec2.size()%2==0)
    {
        for(uint i=0; i<bigVec1.size(); i+=2)
        {
            for(uint j=0; j<bigVec2.size(); j+=2)
            {
                if((bigVec1[i]==bigVec2[j] && bigVec1[i+1]==bigVec2[j+1] )||
                        (bigVec1[i]==bigVec2[j+1] && bigVec1[i+1]==bigVec2[j]))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<glm::vec3> removeDuplicates(posVector _pVec)
{
    posVector tempVec=_pVec;

    posVector newVec;

    newVec.push_back(tempVec[0]);

    bool sorted=false;
    while(!sorted)
    {
        uint count=newVec.size()-1;
        for (auto p : tempVec)
        {

            if(compare(p, newVec)==true)
            {

                continue;
            }
            else
            {
                newVec.push_back(p);
                count--;
            }
        }
        if(count==newVec.size()-1)
        {
            sorted=true;
        }
    }


    return newVec;
}

std::vector<uint> getFaces(const aiScene* scene, uint meshIndex)
{
    std::vector<indexTrio> _temp;
    if(scene && scene->HasMeshes())
    {
        auto mesh=scene->mMeshes[meshIndex];
        auto& faces=mesh->mFaces;
        auto numFace=mesh->mNumFaces;

        for(uint i =0; i<numFace; i++)
        {
            auto faceIndices=faces[i].mIndices;
            if(faces[i].mNumIndices==3 )//&& i!=numFace-1)
            {
                //std::cout<<"tri\n";
                _temp.push_back(indexTrio(filter[faceIndices[0]], filter[faceIndices[1]] , filter[faceIndices[2]]));
            }

        }

        //   auto mesh=scene->mMeshes[meshIndex];
        //   auto& faces=mesh->mFaces;
        //   auto numFace=mesh->mNumFaces;
        //
        //   //     auto defaultIndexing=storePoints(scene,0);
        //   //     posVector uniqueVertices=removeDuplicates(defaultIndexing);
        //   std::vector<uint> filter;
        //
        //   for(uint i=0; i<defaultIndexing.size(); i++)
        //   {
        //       for(uint j=0; j<uniqueVertices.size(); j++)
        //       {
        //           if(defaultIndexing[i] == uniqueVertices[j])
        //           {
        //               filter.push_back(j);
        //           }
        //       }
        //   }
        //
        //   for(uint i=0;i<(numFace); i++)
        //   {
        //       auto faceIndices=faces[i].mIndices;
        //       if(faces[i].mNumIndices==3 )//&& i!=numFace-1)
        //       {
        //           //std::cout<<"tri\n";
        //           _temp.push_back(indexTrio(filter[faceIndices[0]], filter[faceIndices[1]] , filter[faceIndices[2]]));
        //       }
        //
        //   }
        //
        //   for(auto  p: _temp)
        //   {
        //       faceVector.push_back(pointVector[p.p1]);
        //       faceVector.push_back(pointVector[p.p2]);
        //       faceVector.push_back(pointVector[p.p3]);
        //   }
    }//
    //return faceVector;
}



PBDobject::PBDobject()
{

}

bool PBDobject::Initialize(std::string _model,uint _meshIndex, glm::vec3 _originalPos)
{
    m_modelName=_model;
    m_originalPosition=_originalPos;

    const aiScene* scene=getModel(m_modelName);
    if(scene==NULL)
    {
        return false;
    }

    auto allpoints = storePoints(scene, _meshIndex);
    auto edges = getEdges(scene, _meshIndex, allpoints);
    auto faceIndices=removeDuplicates(allpoints);

    //m_Constraints =createDistanceConstraints(edges,faceIndices, m_Points);
    //m_facePoints=getFaces(scene, _meshIndex , m_Points, allpoints);
    //
    //for(auto p : m_Points)
    //{
    //    auto pos = p.get()->getP();
    //    p.get()->setP(pos+m_originalPosition );
    //}
    return true;
}

bool PBDobject::Initialize(glm::vec3 _originalPos, std::vector<glm::vec3> _PointPos, std::vector<glm::vec3> _PointVel, std::vector<float> _PointMass)
{
    if(_PointPos.size() != 0 && _PointVel.size() != 0 && _PointMass.size() != 0
            && _PointPos.size() == 0 && _PointVel.size() == 0 && _PointMass.size())
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
    return m_pointPos[_index];
}

const glm::vec3 PBDobject::getPointVelocity(uint _index) const
{
    return m_pointVel[_index];
}

const std::vector<glm::vec3> PBDobject::getPointPositions() const
{
    return m_pointPos;
}

const std::vector<glm::vec3> PBDobject::getPointVelocities() const
{
    return m_pointVel;
}

const float PBDobject::getPointMass(uint _index) const
{}

const float PBDobject::getPointInvMass(uint _index) const
{}

const std::vector<uint> PBDobject::getConstraint() const
{}

void PBDobject::addPoint(glm::vec3 _newPointPos, glm::vec3 _newPointVel, float _newPointMass, float _newPointInvMass)
{
    m_pointPos.push_back(_newPointPos);
    m_pointVel.push_back(_newPointVel);
    m_pointMass.push_back(_newPointMass);
    m_pointInvMass.push_back(_newPointInvMass);
}

void PBDobject::addPoints(std::vector<glm::vec3> _newPointPos,std::vector<glm::vec3> _newPointVel, std::vector<float> _newPointMass, std::vector<float> _newPointInvMass)
{
    if(_newPointPos.size() == _newPointVel.size() == _newPointMass.size() == _newPointInvMass.size())
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

bool PBDobject::addDistConstraint(uint _IndexPoint1, uint _IndexPoint2)
{
    m_distanceConstraint.push_back(_IndexPoint1);
    m_distanceConstraint.push_back(_IndexPoint2);
}

bool PBDobject::addDistConstraints(std::vector<uint> _IndexPoint)
{
    if(_IndexPoint1.size()%2==0)
    {
        m_distanceConstraint.insert(m_distanceConstraint.end(), _IndexPoint.begin(), _IndexPoint.end());
        return true;
    }
    else
    {
        return false;
    }
}

bool PBDobject::addBendConstraint(uint _IndexPoint1,uint _IndexPoint2)
{

}

bool PBDobject::addBendConstraints(std::vector<uint> _IndexPoint1)
{
    if(_IndexPoint1.size()%3==0)
    {
        m_bendingConstraint.insert(m_bendingConstraint.end(), _IndexPoint.begin(), _IndexPoint.end());
        return true;
    }
    else
    {
        return false;
    }}

const std::vector<uint> PBDobject::getFacesPoints() const
{
    return m_facePoints;
}

} // end of namespace

