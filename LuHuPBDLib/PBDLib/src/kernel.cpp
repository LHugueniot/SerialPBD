#include "kernel.h"


namespace LuHu {











//std::vector<indexDuo> getEdges(const aiScene* scene, uint meshIndex,
//                               posVector defaultIndexing)
//{
//    std::vector<indexDuo> _temp;
//    std::vector<indexDuo> f_temp;
//    if(scene && scene->HasMeshes())
//    {
//        auto mesh=scene->mMeshes[meshIndex];
//        auto& faces=mesh->mFaces;
//        auto numFace=mesh->mNumFaces;
//
//        std::cout<<numFace;
//
////        auto defaultIndexing=storePoints(scene,0);
//        posVector uniqueVertices=removeDuplicates(defaultIndexing);
//        std::vector<uint> filter;
//
//        for(uint i=0; i<defaultIndexing.size(); i++)
//        {
//            for(uint j=0; j<uniqueVertices.size(); j++)
//            {
//                if(defaultIndexing[i] == uniqueVertices[j])
//                {
//                    filter.push_back(j);
//                }
//            }
//        }
//
//        for(uint i=0;i<(numFace); i++)
//        {
//            auto faceIndices=faces[i].mIndices;
//            if(faces[i].mNumIndices==3)
//            {
//                //std::cout<<"tri\n";
//                _temp.push_back(indexDuo(filter[faceIndices[0]], filter[faceIndices[1]]));
//                _temp.push_back(indexDuo(filter[faceIndices[1]], filter[faceIndices[2]]));
//                _temp.push_back(indexDuo(filter[faceIndices[2]], filter[faceIndices[0]]));
//
//            }
//        }
//
//        std::vector<std::vector<int>> test;
//        for(auto t : _temp)
//        {
//            std::vector<int> temp2;
//            temp2.push_back(t.p.first);
//            temp2.push_back(t.p.second);
//            test.push_back(temp2);
//        }
//
//        std::sort(test.begin(), test.end());
//        test.erase(std::unique(test.begin(), test.end()), test.end());
//
//        for(auto p : test)
//        {
//            f_temp.push_back(indexDuo(p[0], p[1]));
//        }
//        return f_temp;
//    }
//}
//
//std::vector<std::shared_ptr<constraint>> createDistanceConstraints(std::vector<indexDuo> edges,
//                                                                   std::vector<glm::vec3> vertices,
//                                                                   std::vector<std::shared_ptr<point>> &pointVector)
//{
//
//    posVector uniqueVertices=removeDuplicates(vertices);
//    std::vector<std::shared_ptr<point>> temp;           //to be a duplicate free vertex of point pointers
//
//    std::vector<std::shared_ptr<constraint>> conIndex;
//
//    for(auto p : uniqueVertices)
//    {
//        std::shared_ptr<point> a(new point(p,glm::vec3(0), 1.0f));
//        temp.push_back(a);
//    }
//
//    pointVector=temp;
//
//    for(uint j=0; j<edges.size(); j++)
//    {
//        auto ip1 =edges[j].p.first;
//        auto ip2 =edges[j].p.second;
//
//        auto p1= pointVector[ip1];
//        auto p2= pointVector[ip2];
//        std::shared_ptr<constraint> b(new distanceConstraint(p1, p2));
//        conIndex.push_back(b);
//    }
//
//    return conIndex;
//}

}
