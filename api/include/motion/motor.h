#ifndef API_MOTION_MOTOR_H
#define API_MOTION_MOTOR_H

#include <SFML/System/Vector2.hpp>
#include <print>
#include <string>

namespace api::motion {

// Tiny linear mover: each Update() steps position_ toward destination_ at a
// fixed speed, snapping onto the destination once it is within reach.
// Provided to students as-is for the rough NPC (no pathfinding).
class Motor {
 public:
  void Update(float dt);  // call once per frame
  [[nodiscard]] float RemainingDistance() const { return remainingDistance_; }

  void SetSpeed(float speed) { speed_ = speed; }
  void SetPosition(sf::Vector2f position) {
    position_ = position;
    remainingDistance_ = (destination_ - position_).length();
  }
  void SetDestination(sf::Vector2f destination) {
    destination_ = destination;
    remainingDistance_ = (destination_ - position_).length();
  }

  [[nodiscard]] const sf::Vector2f& GetPosition() const { return position_; }

 private:
  sf::Vector2f position_;
  sf::Vector2f destination_;
  float speed_ = 0.f;
  float remainingDistance_ = 0.f;
};

inline void Motor::Update(const float dt) {
  const sf::Vector2f distance = destination_ - position_;
  remainingDistance_ = distance.length();

  if (remainingDistance_ < speed_ * dt) {
    position_ = destination_;

    remainingDistance_ = 0.f;
    return;
  }
  position_ += distance.normalized() * speed_ * dt;
}

}  // namespace api::motion

#endif  // API_MOTION_MOTOR_H
