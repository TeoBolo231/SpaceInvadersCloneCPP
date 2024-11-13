
#ifndef SPACEINVADERSSFML_PLAYER_H
#define SPACEINVADERSSFML_PLAYER_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Player
{
 public:
  Player(std::string filename);
  ~Player();

  void incrementScore(int val);
  void resetScore();
  int returnScore();

  void setLives(int val);
  int returnLives();
  int returnStartLives();

  void inBound(sf::RenderWindow& window);
  void initPos(sf::RenderWindow& window);

  void setDirection(float dir);
  Vector2 returnDirection();
  void move(float dt);
  sf::Sprite* getSprite();

  float returnSpeed();

  float returnSpriteW();
  float returnSpriteH();

 private:
  sf::Sprite sprite;

  Vector2 direction = {0, 0};

  sf::Texture texture;
  int score;
  int lives;
  int start_lives = 2;
  float speed = 500.f;
  float sprite_width = 48.;
  float sprite_height = 48.;
};

#endif // SPACEINVADERSSFML_PLAYER_H
