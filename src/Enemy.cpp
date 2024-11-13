
#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
{
  if (!black_texture.loadFromFile("Data/Images/SpaceShooterRedux/PNG/Enemies/enemyBlack1.png") ||
      !blue_texture.loadFromFile("Data/Images/SpaceShooterRedux/PNG/Enemies/enemyBlue2.png") ||
      !green_texture.loadFromFile("Data/Images/SpaceShooterRedux/PNG/Enemies/enemyGreen3.png") ||
      !orange_texture.loadFromFile("Data/Images/SpaceShooterRedux/PNG/Enemies/enemyOrange4.png"))
  {
    std::cout <<"Enemy texture not loaded\n";
  }
  getSprite()->setScale(0.5, 0.5);
  setDirection(-1, 0);
}
Enemy::~Enemy()
{

}

/*============================================================================*/

bool Enemy::getVisibility() const
{
  return is_visible;
}
void Enemy::setVisibility(bool val)
{
  is_visible = val;
}

/*============================================================================*/

void Enemy::addTexture(Colours colour)
{
  if (colour == Colours::Black)
  {
    sprite.setTexture(black_texture);
  }
  else if (colour == Colours::Blue)
  {
    sprite.setTexture(blue_texture);
  }
  else if (colour == Colours::Green)
  {
    sprite.setTexture(green_texture);
  }
  else
  {
    sprite.setTexture(orange_texture);
  }
}
float Enemy::spriteHeight() const
{
  return sprite_height;
}
float Enemy::spriteWidth() const
{
  return sprite_width;
}
sf::Sprite* Enemy::getSprite()
{
  return &sprite;
}
int Enemy::returnGridSizeY() const
{
  return grid_size_y;
}
int Enemy::returnGridSizeX() const
{
  return grid_size_x;
}
void Enemy::moveLR(float dt)
{
  getSprite()->move(returnDirection().x * returnSpeed() * dt,
                    returnDirection().y * returnSpeed() * dt);
}
//void Enemy::moveLR(float dt)
//{
//  // aumentare y * num (1%)
//  getSprite()->move(returnDirection().x * returnSpeed() * dt,
//                    returnDirection().y * returnSpeed() * dt);
//}

//void Enemy::moveLR(float dt)
//{
  // get position of the x comp of the object
  // add to the current pos direction.x * speed * dt
  // calculate y1 and y2 y = ax^2 + b
  // (a and b are 2 arbitrary numbers) a minore di 1
  // subtract y2- y1
  // assign (y2 - y1) to the y comp
  // dir.y * speed *dt

  // ripetere con sin func from math library
  // y = a * sin(bx)

//  getSprite()->move(returnDirection().x * returnSpeed() * dt,
//                    returnDirection().y * returnSpeed() * dt);
//}

bool Enemy::moveD(float dt, sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x <= 0 || getSprite()->getPosition().x >= window.getSize().x - spriteWidth())
  {
    return true;
//    setDirection(returnDirection().x * -1, returnDirection().y);
//    getSprite()->setPosition(getSprite()->getPosition().x, getSprite()->getPosition().y + spriteHeight() + 20);
  }
  else
  {
    return false;
  }
}
void Enemy::inBound(sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x < 0)
  {
    getSprite()->setPosition(0, getSprite()->getPosition().y);
  }
  if (getSprite()->getPosition().x > window.getSize().x - spriteWidth())
  {
    getSprite()->setPosition(window.getSize().x - spriteWidth(), getSprite()->getPosition().y);
  }
}
float Enemy::returnSpeed()
{
  return speed;
}

void Enemy::grav(float dt, sf::RenderWindow& window)
{
  float mult_factor = 1.00002;
  static float speed = 0.3;
  speed *= mult_factor;
  getSprite()->move(returnDirection().x * returnSpeed() * dt, speed * returnSpeed() * dt);
}
void Enemy::quad(float dt, sf::RenderWindow& window)
{
  float a = -0.0001f;

  float future_x = getSprite()->getPosition().x + (returnDirection().x * returnSpeed() * dt);
  float present_y = a *  pow(getSprite()->getPosition().x , 2);
  float future_y = a *  pow(future_x, 2);
  float new_y = future_y - present_y;
  getSprite()->move(returnDirection().x * returnSpeed() * dt, new_y);
}
void Enemy::sinCurve(float dt, sf::RenderWindow& window)
{
  float a = 12.f;
  float b = -0.1;

  float future_x = getSprite()->getPosition().x + (returnDirection().x * returnSpeed() * dt);
  float present_y =  a * sin(b * getSprite()->getPosition().x);
  float future_y = a * sin(b * future_x);
  float new_y = future_y - present_y;
  getSprite()->move(returnDirection().x * returnSpeed() * dt, new_y);
}
void Enemy::setDirection(float dir_x, float dir_y)
{
  direction.x = dir_x;
  direction.y = dir_y;
  direction.normalise();
}
Vector2 Enemy::returnDirection()
{
  return direction;
}
