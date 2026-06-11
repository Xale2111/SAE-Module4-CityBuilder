//
// Created by alexk on 28.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_

#include "SFML/Graphics.hpp"
#include "data_utils.h"
namespace camera {

class Camera {
 public :
  void SetupView(sf::Vector2f size = {DataUtils::kScreenWidth, DataUtils::kScreenHeight},
                 sf::Vector2f position = {DataUtils::kScreenWidth / 2, DataUtils::kScreenHeight / 2});
  [[nodiscard]] sf::View &GetView();
  void HandleMouse(const std::optional<sf::Event> &event, sf::RenderWindow &window);

 private :
  sf::View view_;
  float zoom_factor_ = .9f;
  float dezoom_factor_ = 1.1f;

  const float max_zoom_ = .4f;
  const float max_dezoom_ = 5.f;

  bool is_dragging_ = false;
  sf::Vector2i last_mouse_pos_;

  void LimitCameraArea();

};
}

#endif //SAE_ALEXK_CITYBUILDER_API_SRC_CAMERA_CAMERA_H_
