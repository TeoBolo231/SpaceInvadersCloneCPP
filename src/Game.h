
#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include "Enemy.h"
#include "Player.h"
#include "Cannon.h"
#include "Vector2.h"
#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);

  // Objects
  Player* player = nullptr;
  Cannon* cannon = nullptr;
  Enemy* enemy = nullptr;
  Enemy* enemies = nullptr;

 private:
  void setStartScreen();
  void startScreenBG();
  void startScreenGameTitle();
  void startScreenRules();
  void startScreenPlay();
  void startScreenQuit();

  void setModeScreen();
  void modeScreenBG();
  void modeScreenText();
  void modeScreenNorm();
  void modeScreenGrav();
  void modeScreenQuad();
  void modeScreenSin();

  void setGameScreen();
  void gameScreenBG();
  void gameScreenPlayer();
  void gameScreenLives();
  void gameScreenScore();
  void levelOne();

  void setEndScreen();
  void endScreenBG();
  void endScreenText();
  void endScreenPlay();
  void endScreenQuit();

  void setPauseMenu();
  void pauseMenuBG();
  void pauseMenuText();
  void pauseMenuRestart();
  void pauseMenuQuit();

  bool collisionCheckText(sf::Vector2i click, sf::Text text);

  void resetEnemyPos();
  void printScore();
  void endGameManager();

  // General
  sf::Font font_game;
  bool play_selected = true;
  sf::RenderWindow& window;

  // Game States
  bool start_menu;
  bool mode_menu;
  bool pause_menu;
  bool end_screen;
  bool game_running;
  bool normal_mode;
  bool grav_mode;
  bool quad_mode;
  bool sin_mode;

  // Start Menu
  sf::Text start_menu_title_text;
  sf::Sprite start_menu_bg_sprite;
  sf::Texture start_menu_bg_texture;
  sf::Text start_menu_play_option;
  sf::Text start_menu_quit_option;
  sf::Text rules_text;

  // Mode Menu
  sf::Sprite mode_menu_bg_sprite;
  sf::Texture mode_menu_bg_texture;
  sf::Text mode_menu_text;
  sf::Text normal_mode_text;
  sf::Text grav_mode_text;
  sf::Text quad_mode_text;
  sf::Text sin_mode_text;

  // Pause Menu
  sf::Sprite background_pause_sprite;
  sf::Texture background_pause_texture;
  sf::Text pause_title_text;
  sf::Text restart_opt_pause_text;
  sf::Text quit_opt_pause_text;

  // End Screen
  sf::Sprite end_bg_sprite;
  sf::Texture end_bg_texture;
  sf::Text end_title_text;
  sf::Text play_again_end_text;
  sf::Text quit_end_text;

  // Game
  sf::Sprite game_bg_sprite;
  sf::Texture game_bg_texture;
  sf::Text score_text;
  sf::Text lives_text;

};

#endif // SPACEINVADERS_GAME_H
