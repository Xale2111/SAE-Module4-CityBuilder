//
// Created by alexk on 03.06.2026.
//

#ifndef CITYBUILDER_BT_SEQUENCE_H
#define CITYBUILDER_BT_SEQUENCE_H

#include <vector>
#include "bt_composite.h"

namespace core::ai::behaviour_tree {
class SequenceNode : public CompositeNode {
 public :
  ~SequenceNode() override = default;
  void Reset() override { currentChild_ = 0; };
  Status Tick() override {
    while (currentChild_ < static_cast<int>(children_.size())) {
      Status childStatus = children_[currentChild_]->Tick();

      if (childStatus == Status::kSuccess) {
        currentChild_++;
      } else if (childStatus == Status::kFailure) {
        Reset();
        return Status::kFailure;
      } else {
        return Status::kRunning;
      }
    }
    Reset();
    return Status::kSuccess;
  }

};

}
#endif //CITYBUILDER_BT_SEQUENCE_H
