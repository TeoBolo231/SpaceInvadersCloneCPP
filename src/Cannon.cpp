
#include "Cannon.h"

Cannon::Cannon(sf::RenderWindow& window, Player player)
{
  getSprite()->setPosition(window.getSize().x/2, window.getSize().y - player.returnSpriteH());
  bullet = new Bullet[clip_size];
}
Cannon::~Cannon()
{
  delete[] bullet;
  bullet = nullptr;
}
void Cannon::move(Player player, float dt)
{
  getSprite()->move(player.returnDirection().x * player.returnSpeed() * dt,
                    player.returnDirection().y * player.returnSpeed() * dt);
}
void Cannon::inBound(Player player, sf::RenderWindow& window)
{
  if (getSprite()->getPosition().x <= player.returnSpriteW()/2)
  {
    getSprite()->setPosition(player.returnSpriteW()/2, getSprite()->getPosition().y);
  }
  if (getSprite()->getPosition().x >= window.getSize().x - player.returnSpriteW()/2)
  {
    getSprite()->setPosition(window.getSize().x - player.returnSpriteW()/2 ,
                             getSprite()->getPosition().y);
  }
}
void Cannon::shoot()
{
  for (int i = 0; i < clip_size; ++i)
  {
    if (!bullet[i].returnVisibility())
    {
      bullet[i].getSprite()->setPosition(sprite.getPosition().x, sprite.getPosition().y);
      bullet[i].setVisibility(true);
      break;
    }
  }
  current_fr = fire_rate;
}
sf::Sprite* Cannon::getSprite()
{
  return &sprite;
}
int Cannon::returnClipSize()
{
  return clip_size;
}
int Cannon::returnFireRate()
{
  return current_fr;
}
int Cannon::setFireRate(int val)
{
  current_fr -= val;
}
void Cannon::initPos(sf::RenderWindow& window)
{
  getSprite()->setPosition
    (
      window.getSize().x/2,
      window.getSize().y - window.getSize().y/50
    );
}
