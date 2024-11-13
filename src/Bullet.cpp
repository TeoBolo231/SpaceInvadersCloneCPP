
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Bullet::Bullet()
{
  if (!texture.loadFromFile("Data/Images/SpaceShooterRedux/PNG/Lasers/laserBlue02.png"))
  {
    std::cout <<"Player texture not loaded\n";
  }
  sprite.setTexture(texture);
  sprite.setScale
    (
      sprite_width/ sprite.getGlobalBounds().width,
      sprite_height/ sprite.getGlobalBounds().height
    );

  sprite.setScale(0.5, 0.5);
  direction.x = 0;
  direction.y = -1;
}
Bullet::~Bullet()
{

}

float Bullet::returnSpeed()
{
  return speed;
}
float Bullet::spriteW()
{
  return sprite_width;
}
float Bullet::spriteH()
{
  return sprite_height;
}
void Bullet::move(float dt)
{
  getSprite()->move(returnDirection().x * returnSpeed() * dt,
                    returnDirection().y * returnSpeed() * dt);
}
Vector2 Bullet::returnDirection()
{
  return direction;
}
sf::Sprite* Bullet::getSprite()
{
  return &sprite;
}
bool Bullet::returnVisibility()
{
  return is_visible;
}
void Bullet::setVisibility(bool val)
{
  is_visible = val;
}
