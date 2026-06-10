//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_STAR_NODE_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_STAR_NODE_H_
namespace api::ai{

class AStarNode{
 public :
  float f(){return g_+h_;};
 private:
  float g_;
  float h_;

};

}


#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_A_STAR_NODE_H_
