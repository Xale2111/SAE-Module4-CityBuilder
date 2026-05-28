//
// Created by alexk on 28.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_

#include "SFML/Graphics.hpp"
#include "DataUtils.h"

class Camera {
 private :
  sf::View view;
  float zoomFactor = .9f;
  float dezoomFactor = 1.1f;

  const float maxZoom = .4f;
  const float maxDezoom = 2.f;

  bool isDragging_ = false;
  sf::Vector2i lastMousePos_;

 public :
  void SetupView(sf::Vector2f size = {DataUtils::kScreenWidth,DataUtils::kScreenHeight},sf::Vector2f position = {DataUtils::kScreenWidth/2,DataUtils::kScreenHeight/2} );
  sf::View& GetView();
  void HandleMouse(const std::optional<sf::Event>& event, sf::RenderWindow &window);
};

#endif //SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_
