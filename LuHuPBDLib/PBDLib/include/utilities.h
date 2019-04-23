#ifndef UTILITIES_H
#define UTILITIES_H

#include <experimental/filesystem>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

namespace LuHu{

//-------------------------------------------------Index Duo-----------------------------------------------

struct indexDuo
{

    /// @brief Constructor
    /// @param p pair of uints used as indices, when index duo is constructed they are sorted by value

    indexDuo(const int p1, const int p2) :
        p1(std::min(p1, p2)),
        p2(std::max(p1, p2))
    {}

    /// @param p pair of uints
    uint p1
    ,p2;

    /// @brief operator== overload operate for std::unique to use

    friend bool operator==(const indexDuo &_p1, const indexDuo &_p2)
    {
        return _p1.p1 == _p2.p1 && _p1.p2 == _p2.p2 ;
    }
};

//-------------------------------------------------Index Trio----------------------------------------------

struct indexTrio
{

    /// @brief Constructor
    /// @param p1 index to p1 of face
    /// @param p2 index to p1 of face
    /// @param p3 index to p1 of face

    indexTrio(const uint _a, const uint _b, const uint _c) :
        p1(_a),
        p2(_b),
        p3(_c)
    {}

    uint p1
    ,p2
    ,p3;

    friend bool operator==(const indexTrio &_p1, const indexTrio &_p2)
    {
        return _p1 == _p2;
    }
};

const aiScene* getModel(std::string _model);
std::vector<glm::vec3> storePoints(aiMesh *_assimpMesh);
std::vector<indexTrio> storeFaceIndices(aiMesh *_assimpMesh);
void printVec3(glm::vec3 _vec);
glm::vec3 aiToGLM(aiVector3D mVertices);
std::vector<glm::vec3> storePoints(aiMesh* _assimpMesh);
std::vector<indexTrio> storeFaceIndices(aiMesh* _assimpMesh);
bool compare(glm::vec3 temp, std::vector<glm::vec3> bigVec);
bool compare( std::vector<glm::vec3> bigVec1,  std::vector<glm::vec3> bigVec2);
std::vector<glm::vec3> removeDuplicates(std::vector<glm::vec3> _pVec);


//-------------------------------------------------Mesh--------------------------------------------------

struct Mesh{

    Mesh(std::string _model, uint _index){  m_modelName=_model;
                                            Model=getModel(_model);
                                            if(Model!=NULL)
                                            {
                                                m_assimpMesh=Model->mMeshes[_index];
                                                m_pointPositions=storePoints(m_assimpMesh);
                                                m_faceIndices=storeFaceIndices(m_assimpMesh);
                                            }
                                         }

    std::vector<glm::vec3> getUniquePoints(std::vector<glm::vec3> _points,
                                           std::vector<uint>* _filter);


    std::vector<indexTrio> getFaceIndices(std::vector<glm::vec3> _uniquePoints,
                                          std::vector<uint> &_filter);

    std::vector<indexDuo> getEdgeIndices(std::vector<glm::vec3> _uniquePoints,
                                         std::vector<uint> &_filter);

    std::string m_modelName;
    const aiScene*  Model;
    aiMesh* m_assimpMesh;
    std::vector<glm::vec3> m_pointPositions;
    std::vector<indexTrio> m_faceIndices;

};

}

#endif // UTILITIES_H
