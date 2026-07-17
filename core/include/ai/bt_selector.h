//
// Created by alexk on 03.06.2026.
//

#ifndef CITYBUILDER_BT_SELECTOR_H
#define CITYBUILDER_BT_SELECTOR_H
#include <vector>
#include "bt_composite.h"

namespace core::ai::behaviour_tree {
class SelectorNode : public CompositeNode {
 public :
  ~SelectorNode() override = default;
  void Reset() override { currentChild_ = 0; };
  Status Tick() override {
    while (currentChild_ < static_cast<int>(children_.size())) {
      Status childStatus = children_[currentChild_]->Tick();

      if (childStatus == Status::kFailure) {
        currentChild_++;
      } else if (childStatus == Status::kSuccess) {
        Reset();
        return Status::kSuccess;
      } else {
        return Status::kRunning;
      }
    }
    Reset();
    return Status::kFailure;
  }

};

}
#endif //CITYBUILDER_BT_SELECTOR_H
