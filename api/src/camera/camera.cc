//
// Created by alexk on 28.05.2026.
//

#include <print>
#include "camera/camera.h"
#include "data_utils.h"
namespace camera {

//********PRIVATE********//
void Camera::LimitCameraArea() {
  //Check current camera position
  //Check camera size
  //Based on screen size, camera size and position, replace camera if necessary
  //if center is smaller than mapWidth*viewsizeX/2 || bigger than mapWidth - mapWidth*viewsizeX/2
  //same logic with Y

  //if smaller or bigger, set to nearest acceptable value (mapWidth*viewsizeX/2 || bigger than mapWidth - mapWidth*viewsizeX/2)

  const sf::Vector2f center = view_.getCenter();
  const sf::Vector2f size = view_.getSize();

  const float kMinX = size.x / 2;
  const float kMaxX = DataUtils::kTilemapWidth - size.x / 2;
  const float kMinY = size.y / 2;
  const float kMaxY = DataUtils::kTilemapHeight - size.y / 2;

  if (center.x < kMinX) {
    view_.setCenter(sf::Vector2f(kMinX, center.y));
  } else if (center.x > kMaxX) {
    view_.setCenter(sf::Vector2f(kMaxX, center.y));
  }
  if (center.y < kMinY) {
    view_.setCenter(sf::Vector2f(center.x, kMinY));
  } else if (center.y > kMaxY) {
    view_.setCenter(sf::Vector2f(center.x, kMaxY));
  }
}


//********PUBLIC********//

void Camera::SetupView(sf::Vector2f size, sf::Vector2f position) {
  view_.setSize(size);
  view_.setCenter(position);
  LimitCameraArea();
}

sf::View &Camera::GetView() {
  return view_;
}

void Camera::HandleMouse(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
  if (const auto *scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
    float delta = scroll->delta;

    //zoom in
    if (delta > 0 && view_.getSize().x > max_zoom_ * DataUtils::kScreenWidth) {
      view_.zoom(zoom_factor_);
    }
      //zoom out
    else if (delta < 0 && view_.getSize().x < max_dezoom_ * DataUtils::kScreenWidth) {
      view_.zoom(dezoom_factor_);
      LimitCameraArea();
    }
    window.setView(view_);

  }
  if (const auto *press = event->getIf<sf::Event::MouseButtonPressed>()) {
    if (press->button == sf::Mouse::Button::Right) {
      is_dragging_ = true;
      last_mouse_pos_ = sf::Mouse::getPosition(window);
    }
  }

  if (const auto *release = event->getIf<sf::Event::MouseButtonReleased>()) {
    if (release->button == sf::Mouse::Button::Right)
      is_dragging_ = false;
    is_dragging_ = false;
  }

  if (const auto *move = event->getIf<sf::Event::MouseMoved>()) {
    if (is_dragging_) {
      sf::Vector2i currentPos = sf::Mouse::getPosition(window);
      sf::Vector2i delta = currentPos - last_mouse_pos_;

      view_.move(sf::Vector2f(-delta.x, -delta.y));
      last_mouse_pos_ = currentPos;
      LimitCameraArea();

      window.setView(view_);
    }
  }
}
}


