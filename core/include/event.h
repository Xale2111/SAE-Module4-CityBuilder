//
// Created by alexk on 05.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
#define SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
#include <functional>
#include <vector>

class CustomEvent {
 public:
  void operator+=(std::function<void()> callback) {
    callbacks_.push_back(callback);
  }
  void Invoke() {
    for (auto& callback : callbacks_)
      callback();
  }
 private:
  std::vector<std::function<void()>> callbacks_;
};
#endif //SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
