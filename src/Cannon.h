
#ifndef SPACEINVADERS_GAMEOBJECT_H
#define SPACEINVADERS_GAMEOBJECT_H

#include "Player.h"
#include "Bullet.h"

class Cannon
{
 public:
  Cannon(sf::RenderWindow& window, Player player);
  ~Cannon();

  void move(Player player, float dt);
  void inBound(Player player, sf::RenderWindow& window);
  void shoot();
  int returnClipSize();
  int returnFireRate();
  int setFireRate(int val);
  void initPos(sf::RenderWindow& window);

  Bullet* bullet = nullptr;

  sf::Sprite* getSprite();
 private:
  int current_fr = 0;
  int fire_rate = 15;
  int clip_size = 20;
  sf::Sprite sprite;
};





#endif // SPACEINVADERS_GAMEOBJECT_H
