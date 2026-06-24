//
// Created by alexk on 05.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
#define SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
#include <functional>
#include <vector>

/// <summary>
/// Custom event class
/// In case a button (or anything that is clickable) needs to call a function without returning a ActionCode
/// ex : Build cards in the build menu or build menu open/close button
/// </summary>
class CustomEvent {
 public:
  void Append(std::function<void()> callback) {
    callbacks_.push_back(std::move(callback));
  }
  void Clear()
  {
    callbacks_.clear();
  }
  void Invoke() {
    for (auto& callback : callbacks_)
      callback();
  }
 private:
  std::vector<std::function<void()>> callbacks_;
};
#endif //SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_EVENT_H_
