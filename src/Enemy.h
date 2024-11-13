
#ifndef SPACEINVADERSSFML_ENEMY_H
#define SPACEINVADERSSFML_ENEMY_H

#include "Vector2.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Enemy
{
 public:
  Enemy();
  ~Enemy();

  enum Colours {Black, Blue, Green, Orange};
  void addTexture(Colours colour);

  bool getVisibility() const;
  void setVisibility(bool val);

  float spriteWidth() const;
  float spriteHeight() const;
  int returnGridSizeX() const;
  int returnGridSizeY() const;

  void moveLR(float dt);
  bool moveD(float dt, sf::RenderWindow& window);
  void inBound(sf::RenderWindow& window);
  void setDirection(float dir_x, float dir_y);
  Vector2 returnDirection();
  float returnSpeed();
  void grav(float dt, sf::RenderWindow& window);
  void quad(float dt, sf::RenderWindow& window);
  void sinCurve(float dt, sf::RenderWindow& window);

  sf::Sprite* getSprite();

 private:
  bool is_visible = true;

  float sprite_width = 48.;
  float sprite_height = 48.;

  int grid_size_x = 8;
  int grid_size_y = 4;

  float speed = 130;

  Vector2 direction = {0, 0};

  sf::Sprite sprite;

  sf::Texture black_texture;
  sf::Texture blue_texture;
  sf::Texture green_texture;
  sf::Texture orange_texture;
};

#endif // SPACEINVADERSSFML_ENEMY_H
