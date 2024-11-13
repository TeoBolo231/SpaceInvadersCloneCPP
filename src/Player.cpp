
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

Player::Player(std::string filename)
{
  if (!texture.loadFromFile(filename))
  {
    std::cout <<"Player texture not loaded\n";
  }
  sprite.setTexture(texture);
  sprite.setScale
    (
      sprite_width/ sprite.getGlobalBounds().width,
      sprite_height/ sprite.getGlobalBounds().height
    );

  lives = start_lives;
  score = 0;
}
Player::~Player()
{

}

/*============================================================================*/

void Player::incrementScore(int val)
{
  score += val;
}
void Player::resetScore()
{
  score = 0;
}
int Player::returnScore()
{
  return score;
}

/*============================================================================*/

sf::Sprite* Player::getSprite()
{
  return &sprite;
}
float Player::returnSpriteW()
{
  return sprite_width;
}
float Player::returnSpriteH()
{
  return sprite_height;
}
void Player::initPos(sf::RenderWindow& window)
{
  getSprite()->setPosition
    (
      window.getSize().x/2 - getSprite()->getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/50 - getSprite()->getGlobalBounds().height
    );
}
float Player::returnSpeed()
{
  return speed;
}
void Player::setDirection(float dir)
{
  direction.x = dir;
  direction.y = 0;
  direction.normalise();
}
Vector2 Player::returnDirection()
{
  return direction;
}
void Player::move(float dt)
{
  getSprite()->move(returnDirection().x * returnSpeed() * dt,
                    returnDirection().y * returnSpeed() * dt);
}
void Player::inBound(sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x < 0)
  {
    getSprite()->setPosition(0,getSprite()->getPosition().y);
  }
  if (getSprite()->getPosition().x > window.getSize().x - returnSpriteW())
  {
    getSprite()->setPosition(window.getSize().x - returnSpriteW() ,
                             getSprite()->getPosition().y);
  }
}

/*============================================================================*/

void Player::setLives(int val)
{
  lives = lives + val;
}
int Player::returnLives()
{
  return lives;
}
int Player::returnStartLives()
{
  return start_lives;
}

/*============================================================================*/

/*============================================================================*/