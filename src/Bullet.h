
#ifndef SPACEINVADERSSFML_BULLET_H
#define SPACEINVADERSSFML_BULLET_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Bullet
{
 public:
  Bullet();
  ~Bullet();

  void move(float dt);
  Vector2 returnDirection();
  sf::Sprite* getSprite();
  bool returnVisibility();
  void setVisibility(bool val);

  float returnSpeed();
  float spriteW();
  float spriteH();
 private:
  sf::Sprite sprite;

  Vector2 direction = {0, 0};

  sf::Texture texture;

  bool is_visible = false;
  float speed = 600;
  float sprite_width = 48.;
  float sprite_height = 48.;
};

#endif // SPACEINVADERSSFML_BULLET_H
