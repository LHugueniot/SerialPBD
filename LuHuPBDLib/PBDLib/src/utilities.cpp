#include "utilities.h"

namespace LuHu {

const aiScene* getModel(std::string _model)
{
    std::string path = std::experimental::filesystem::current_path();
    std::string finalPath = path+ "/../../models/" +_model;
    //std::cout<<finalPath<<std::endl;
    const aiScene* scene= aiImportFile(finalPath.c_str(),  aiProcess_CalcTangentSpace       |
                                       aiProcess_Triangulate            |
                                       aiProcess_JoinIdenticalVertices  |
                                       aiProcess_SortByPType);
    if(!scene){
        std::cout<<"\n"<<aiGetErrorString()<<"\n";
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

std::vector<glm::vec3> storePoints(aiMesh* _assimpMesh)
{
    std::vector<glm::vec3> temp;
    for(uint j=0; j<_assimpMesh->mNumVertices; j++)
    {
        temp.push_back(aiToGLM(_assimpMesh->mVertices[j]));
        //auto a = aiToGLM(_assimpMesh->mVertices[j]);

        //std::cout<<"glm::vec3("<<a.x<<","<<a.y<<","<<a.z<<"),\n";
    }
    return temp;
}

std::vector<indexTrio> storeFaceIndices(aiMesh* _assimpMesh)
{
    std::vector<indexTrio> temp;
    auto faces=_assimpMesh->mFaces;
    auto numFaces =_assimpMesh->mNumFaces;
    for(uint j=0; j<numFaces; j++)
    {
        auto faceIndex = faces[j];
        if(faceIndex.mNumIndices==3)
        {

            temp.push_back(
                        indexTrio(faceIndex.mIndices[0],
                                  faceIndex.mIndices[1],
                                  faceIndex.mIndices[2])
                    );
        }
        else
        {
            std::cout<<"This mesh contains Quads, only triangle meshes are allowed\n";
            return std::vector<indexTrio>{indexTrio(0,0,0)};
        }

    }
    return temp;
}

std::vector<glm::vec3> Mesh::getUniquePoints(std::vector<glm::vec3> _points, std::vector<uint>* _filter)
{
    std::vector<glm::vec3> UniquePoints;

    UniquePoints.push_back(_points[0]);

    for (uint i=0; i<_points.size(); i++)
    {
        auto result1 = std::find(UniquePoints.begin(), UniquePoints.end(), _points[i]);
        int index = std::distance(UniquePoints.begin(), result1);

        if(result1!=UniquePoints.end()) {
            _filter->push_back(index);
        }
        else
        {
            UniquePoints.push_back(_points[i]);
            _filter->push_back(index);
        }
    }

    return UniquePoints;
}


std::vector<indexTrio> Mesh::getFaceIndices(std::vector<glm::vec3> _uniquePoints, std::vector<uint> &_filter)
{
    //std::cout<<_filter.size()<<"\n";
    std::vector<indexTrio> returnVector;
    uint numFaces =m_faceIndices.size();
    for (uint i=0; i< numFaces; i++) {
        returnVector.push_back( indexTrio(  _filter[m_faceIndices[i].p1],
                                            _filter[m_faceIndices[i].p2],
                                            _filter[m_faceIndices[i].p3] ));
    }
    return returnVector;
}

std::vector<indexDuo> Mesh::getEdgeIndices(std::vector<glm::vec3> _uniquePoints, std::vector<uint> &_filter)
{
    auto uniqueFaceIndices=getFaceIndices(_uniquePoints, _filter);
    uint uniqueFaceIndicesSize=uniqueFaceIndices.size();

    std::vector<indexDuo> allEdgeIndices;
    std::vector<indexDuo> uniqueEdgeIndices;

    for (uint i=0;i< uniqueFaceIndicesSize; i++) {
        allEdgeIndices.push_back(   indexDuo(uniqueFaceIndices[i].p1,uniqueFaceIndices[i].p2));
        allEdgeIndices.push_back(   indexDuo(uniqueFaceIndices[i].p2,uniqueFaceIndices[i].p3));
        allEdgeIndices.push_back(   indexDuo(uniqueFaceIndices[i].p3,uniqueFaceIndices[i].p1));
    }

    uniqueEdgeIndices.push_back(allEdgeIndices[0]);

    for (uint i=0; i<allEdgeIndices.size(); i++)
    {
        auto result1 = std::find(uniqueEdgeIndices.begin(), uniqueEdgeIndices.end(), allEdgeIndices[i]);
        int index = std::distance(uniqueEdgeIndices.begin(), result1);

        if(result1==uniqueEdgeIndices.end()) {
            uniqueEdgeIndices.push_back(allEdgeIndices[i]);
        }
    }
    return uniqueEdgeIndices;
}

std::vector<uint> generateBendConstraints(const std::vector<glm::vec3> pointPositions,
                                          const std::vector<uint> distanceConstraints)
{
    std::vector<uint> bendingConstraints;
    if(distanceConstraints.size()%2!=0)
    {
        std::cout<<"Incorect dist constraint array, size is not a multiple of 2\n";
        return std::vector<uint>{0};
    }
    std::vector<std::vector<uint>> nodeGraph(pointPositions.size());

    for(uint i=0; i<pointPositions.size(); i++)
    {
        for(uint j=0; j<distanceConstraints.size(); j+=2)
        {
            if((distanceConstraints[j]== i) || (distanceConstraints[j+1]== i ))
            {
                if(distanceConstraints[j]== i)
                    nodeGraph[i].push_back(distanceConstraints[j+1]);

                if(distanceConstraints[j+1]== i)
                    nodeGraph[i].push_back(distanceConstraints[j]);
            }
        }
    }

    for (uint v=0;v<nodeGraph.size(); v++)
    {
        for (auto vi :nodeGraph[v])
        {
            float bestAngle=0;
            uint bestVert=vi;
            for (auto vj :nodeGraph[v])
            {
                if(vi==vj)continue;

                glm::vec3 vit1=pointPositions[v];
                glm::vec3 vit2=pointPositions[vi];
                glm::vec3 vit3=pointPositions[vj];

                float potentialAngle= glm::dot((vit2-vit1), (vit3-vit1))/
                                      glm::length(vit2-vit1)*glm::length(vit3-vit1);
                if(potentialAngle<bestAngle)
                {
                    bestAngle=potentialAngle;
                    bestVert=vj;
                }
            }
            if(vi==bestVert)continue;
            bool found=false;
            for (uint it4=0;it4<bendingConstraints.size(); it4+=3)
            {

                if (((bendingConstraints[it4]==bestVert) && (bendingConstraints[it4+2]==vi) )||
                    ((bendingConstraints[it4+2]==bestVert) && (bendingConstraints[it4]==vi))){
                    found=true;
                    continue;
                }
            }
            if(!found)
            {
                bendingConstraints.push_back(vi);
                bendingConstraints.push_back(v);
                bendingConstraints.push_back(bestVert);
            }

        }
    }
    return bendingConstraints;
}

}
