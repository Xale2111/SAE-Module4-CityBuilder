//
// Created by alexk on 28.05.2026.
//

#include "camera/camera.h"
#include "DataUtils.h"

void Camera::SetupView(sf::Vector2f size, sf::Vector2f position) {
  view.setSize(size);
  view.setCenter(position);
}

sf::View& Camera::GetView() {
  return view;
}

void Camera::HandleMouse(const std::optional<sf::Event> &event, sf::RenderWindow &window) {
  if(const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>())
  {
    float delta = scroll->delta;

    if(delta > 0 && view.getSize().x > maxZoom * DataUtils::kScreenWidth)
    {
      view.zoom(zoomFactor);
    }
    else if(delta < 0 && view.getSize().x < maxDezoom * DataUtils::kScreenWidth)
    {
      view.zoom(dezoomFactor);
    }
    window.setView(view);

  }
  if (const auto* press = event->getIf<sf::Event::MouseButtonPressed>())
  {
    if (press->button == sf::Mouse::Button::Left)
    {
      isDragging_ = true;
      lastMousePos_ = sf::Mouse::getPosition(window);
    }
  }

  if (const auto* release = event->getIf<sf::Event::MouseButtonReleased>())
  {
    if (release->button == sf::Mouse::Button::Left)
      isDragging_ = false;
  }

  if (const auto* move = event->getIf<sf::Event::MouseMoved>())
  {
    if (isDragging_)
    {
      sf::Vector2i currentPos = sf::Mouse::getPosition(window);
      sf::Vector2i delta = currentPos - lastMousePos_;

      view.move(sf::Vector2f(-delta.x, -delta.y));
      lastMousePos_ = currentPos;

      window.setView(view);
    }
  }
}


