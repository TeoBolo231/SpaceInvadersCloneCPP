
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));

  start_menu = true;
  mode_menu = false;
  pause_menu = false;
  end_screen = false;

  game_running = false;
  normal_mode = false;
  grav_mode = false;
  quad_mode = false;
  sin_mode = false;
}

Game::~Game()
{
  delete player;
  player = nullptr;
  delete enemy;
  enemy = nullptr;
  delete[] enemies;
  enemies = nullptr;
  delete cannon;
  cannon = nullptr;
}

/*============================================================================*/

bool Game::init()
{
  //Start Screen
  setStartScreen();

  //Mode Screen
  setModeScreen();

  // Pause Menu
  setPauseMenu();

  // End Screen
  setEndScreen();

  // Game Screen
  setGameScreen();

  return true;
}

/*============================================================================*/

void Game::update(float dt)
{
  if (game_running)
  {
    player->move(dt);
    player->inBound(window);
    cannon->move(*player, dt);
    cannon->inBound(*player, window);

    if (normal_mode)
    {
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].moveLR(dt);
        enemies[i].inBound(window);

      }
    }

    if (grav_mode)
    {
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].grav(dt, window);
        enemies[i].inBound(window);
      }
    }

    if (quad_mode)
    {
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].quad(dt, window);
        enemies[i].inBound(window);
      }
    }

    if (sin_mode)
    {
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].sinCurve(dt, window);
        enemies[i].inBound(window);
      }
    }
    for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
    {
      if (enemies[i].moveD(dt,window) && enemies[i].getVisibility())
      {
        for (int j = 0; j < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++j)
        {
          enemies[j].getSprite()->setPosition(enemies[j].getSprite()->getPosition().x,
                                              enemies[j].getSprite()->getPosition().y + enemies[j].spriteHeight());
          enemies[j].setDirection(enemies[j].returnDirection().x * -1, enemies[j].returnDirection().y);
        }
        break;
      }
    }

    // move bullets + hit
    for (int i = 0; i < cannon->returnClipSize(); ++i)
    {
      if (cannon->bullet[i].returnVisibility())
      {
        cannon->bullet[i].move(dt);
        if (cannon->bullet[i].getSprite()->getPosition().y < 0)
        {
          cannon->bullet[i].setVisibility(false);
        }
        for (int j = 0; j < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++j)
        {
          if (cannon->bullet[i].getSprite()->getGlobalBounds().intersects(enemies[j].getSprite()->getGlobalBounds()) && enemies[j].getVisibility())
          {
            cannon->bullet[i].setVisibility(false);
            enemies[j].setVisibility(false);
            player->incrementScore(10);
          }
        }
      }
    }
    cannon->setFireRate(1);
    printScore();
    endGameManager();
  }
}

/*============================================================================*/

void Game::render()
{
  // Start Menu
  if (start_menu)
  {
    window.draw(start_menu_bg_sprite);
    window.draw(start_menu_title_text);
    window.draw(rules_text);
    window.draw(start_menu_play_option);
    window.draw(start_menu_quit_option);
  }

  // Mode Menu
  else if (mode_menu)
  {
    window.draw(mode_menu_bg_sprite);
    window.draw(mode_menu_text);
    window.draw(normal_mode_text);
    window.draw(grav_mode_text);
    window.draw(quad_mode_text);
    window.draw(sin_mode_text);
  }

    // Pause Menu
  else if (pause_menu)
  {
    window.draw(background_pause_sprite);
    window.draw(pause_title_text);
    window.draw(restart_opt_pause_text);
    window.draw(quit_opt_pause_text);
  }

    // Win Screen
  else if (end_screen)
  {
    window.draw(end_bg_sprite);
    window.draw(end_title_text);
    window.draw(play_again_end_text);
    window.draw(quit_end_text);
  }

    // Game
  else if (game_running)
  {
    window.draw(game_bg_sprite);
    window.draw(score_text);
    window.draw(lives_text);
    window.draw(*player->getSprite());

    for (int i = 0; i < cannon->returnClipSize(); ++i)
    {
      if (cannon->bullet[i].returnVisibility())
      {
        window.draw(*cannon->bullet[i].getSprite());
      }
    }

    for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); i++)
    {
      if (enemies[i].getVisibility())
      {
        window.draw(*enemies[i].getSprite());
      }
    }
  }

    // Error
  else
  {
    std::cout<< "Rendering Error\n";
  }

}

/*============================================================================*/

// Inputs
void Game::keyPressed(sf::Event event)
{
  // Start Menu
  if (start_menu)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        start_menu_play_option.setString(">Play<");
        start_menu_play_option.setStyle(sf::Text::Italic | sf::Text::Bold);
        start_menu_quit_option.setString("Quit");
        start_menu_quit_option.setStyle(sf::Text::Regular);
      }
      else
      {
        start_menu_play_option.setString("Play");
        start_menu_play_option.setStyle(sf::Text::Regular);
        start_menu_quit_option.setString(">Quit<");
        start_menu_quit_option.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        start_menu = false;
        mode_menu = true;
        //game_running = true;
      }
      else
      {
        window.close();
      }
    }
  }

  // Pause Menu
  if (pause_menu)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;

      if(play_selected)
      {
        restart_opt_pause_text.setString(">Restart<");
        restart_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
        quit_opt_pause_text.setString("Quit");
        quit_opt_pause_text.setStyle(sf::Text::Regular);
      }
      else
      {
        restart_opt_pause_text.setString("Restart");
        restart_opt_pause_text.setStyle(sf::Text::Regular);
        quit_opt_pause_text.setString(">Quit<");
        quit_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        pause_menu = false;
        mode_menu = true;
        normal_mode = false;
        grav_mode = false;
        quad_mode = false;
        sin_mode = false;
      }
      else
      {
        window.close();
      }
    }
  }

  // End Screen
  if (end_screen)
  {
    if ((event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::D))
    {
      play_selected = !play_selected;
      if(play_selected)
      {
        play_again_end_text.setString(">Play Again<");
        play_again_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
        quit_end_text.setString("Quit");
        quit_end_text.setStyle(sf::Text::Regular);
      }
      else
      {
        play_again_end_text.setString("Play Again");
        play_again_end_text.setStyle(sf::Text::Regular);
        quit_end_text.setString(">Quit<");
        quit_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
      }
    }
    else if (event.key.code == sf::Keyboard::Enter)
    {
      if(play_selected)
      {
        end_screen = false;
        mode_menu = true;
        normal_mode = false;
        grav_mode = false;
        quad_mode = false;
        sin_mode = false;
      }
      else
      {
        window.close();
      }
    }
  }

  // In Game
  if (game_running)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      if (cannon->returnFireRate() <= 0)
      {
        cannon->shoot();
        std::cout<<"Pew Pew"<< std::endl;
      }
    }

    if (event.key.code == sf::Keyboard::A)
    {
      player->setDirection(-1);
    }

    if (event.key.code == sf::Keyboard::D)
    {
      player->setDirection(1);
    }

    //TEST END SCREEN
    if (event.key.code == sf::Keyboard::N)
    {
      game_running = false;
      end_screen = true;
    }
  }

  // Open/Close Pause Menu
  if (event.key.code == sf::Keyboard:: Escape)
  {
    if (game_running)
    {
      game_running = false;
      pause_menu = true;
    }
    else if (pause_menu)
    {
      pause_menu = false;
      game_running = true;
    }
  }

}
void Game::keyReleased(sf::Event event)
{
  // In Game
  if (game_running)
  {
    if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
    {
      player->setDirection(0);
    }
  }
}
void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);

  // Start Menu
  if (start_menu)
  {
    if (collisionCheckText(click, start_menu_play_option))
    {
      start_menu = false;
      mode_menu = true;
      //game_running = true;
    }
    if (collisionCheckText(click, start_menu_quit_option))
    {
      window.close();
    }
  }

  // Mode Menu
  if (mode_menu)
  {
    if (collisionCheckText(click, normal_mode_text))
    {
      mode_menu = false;
      game_running = true;
      normal_mode = true;
      player->initPos(window);
      player->setLives(player->returnStartLives());
      player->resetScore();
      cannon->initPos(window);
      resetEnemyPos();
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].setVisibility(true);
        enemies[i].setDirection(-1, 0);
      }
      for (int j = 0; j < cannon->returnClipSize(); ++j)
      {
        cannon->bullet[j].setVisibility(false);
      }
    }
    if (collisionCheckText(click, grav_mode_text))
    {
      mode_menu = false;
      game_running = true;
      grav_mode = true;
      player->initPos(window);
      player->setLives(player->returnStartLives());
      player->resetScore();
      cannon->initPos(window);
      resetEnemyPos();
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].setVisibility(true);
        enemies[i].setDirection(-1, 0);
      }
      for (int j = 0; j < cannon->returnClipSize(); ++j)
      {
        cannon->bullet[j].setVisibility(false);
      }
    }
    if (collisionCheckText(click, quad_mode_text))
    {
      mode_menu = false;
      game_running = true;
      quad_mode = true;
      player->initPos(window);
      player->setLives(player->returnStartLives());
      player->resetScore();
      cannon->initPos(window);
      resetEnemyPos();
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].setVisibility(true);
        enemies[i].setDirection(-1, 0);
      }
      for (int j = 0; j < cannon->returnClipSize(); ++j)
      {
        cannon->bullet[j].setVisibility(false);
      }
    }
    if (collisionCheckText(click, sin_mode_text))
    {
      mode_menu = false;
      game_running = true;
      sin_mode = true;
      player->initPos(window);
      player->setLives(player->returnStartLives());
      player->resetScore();
      cannon->initPos(window);
      resetEnemyPos();
      for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
      {
        enemies[i].setVisibility(true);
        enemies[i].setDirection(-1, 0);
      }
      for (int j = 0; j < cannon->returnClipSize(); ++j)
      {
        cannon->bullet[j].setVisibility(false);
      }
    }
  }

  // Pause Menu
  if (pause_menu)
  {
    if (collisionCheckText(click, restart_opt_pause_text))
    {
      pause_menu   = false;
      mode_menu = true;
      normal_mode = false;
      grav_mode = false;
      quad_mode = false;
      sin_mode = false;
    }
    if (collisionCheckText(click, quit_opt_pause_text))
    {
      window.close();
    }
  }

  // End Screen
  if (end_screen)
  {
    if (collisionCheckText(click, play_again_end_text))
    {
      end_screen = false;
      mode_menu = true;
      normal_mode = false;
      grav_mode = false;
      quad_mode = false;
      sin_mode = false;
    }
    if (collisionCheckText(click, quit_end_text))
    {
      window.close();
    }
  }
}
bool Game::collisionCheckText(sf::Vector2i click, sf::Text text)
{
  if (click.x > text.getPosition().x
      && click.y > text.getPosition().y
      && click.x < text.getPosition().x + text.getGlobalBounds().width
      && click.y < text.getPosition().y + text.getGlobalBounds().height)
  {
    return true;
  }

  else
  {
    return false;
  }
}

// Start Screen
void Game::setStartScreen()
{
  // Background
  startScreenBG();

  // Game Title
  startScreenGameTitle();

  // Rules
  startScreenRules();

  // Play Option
  startScreenPlay();

  // Quit Option
  startScreenQuit();
}
void Game::startScreenBG()
{
  if (!start_menu_bg_texture.loadFromFile("Data/Images/start_bg.png"))
  {
    std::cout << "Start menu background texture not loaded\n";
  }
  start_menu_bg_sprite.setTexture(start_menu_bg_texture);
  start_menu_bg_sprite.setScale
    (
      window.getSize().x / start_menu_bg_sprite.getGlobalBounds().width,
      window.getSize().y / start_menu_bg_sprite.getGlobalBounds().height
    );
  start_menu_bg_sprite.setPosition(0, 0);
}
void Game::startScreenGameTitle()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  start_menu_title_text.setString("PONG");
  start_menu_title_text.setFont(font_game);
  start_menu_title_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  start_menu_title_text.setCharacterSize(50);
  start_menu_title_text.setFillColor(sf::Color(0, 0, 0, 255));
  start_menu_title_text.setPosition
    (
      window.getSize().x/2 - start_menu_title_text.getGlobalBounds().width/2,
      0
    );
}
void Game::startScreenRules()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  rules_text.setString("\n\n\n\n                     Controls\n\n"
                       "A = Move spaceship left\n"
                       "D = Move spaceship right\n"
                       "Space = Fire!\n\n");

  rules_text.setFont(font_game);
  rules_text.setCharacterSize(30);
  rules_text.setFillColor(sf::Color(0, 0, 0, 255));
  rules_text.setPosition
    (
      window.getSize().x/4 + window.getSize().x/30,
      window.getSize().y/5 - rules_text.getGlobalBounds().height/2
    );
}
void Game::startScreenPlay()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  start_menu_play_option.setString(">Play<");
  start_menu_play_option.setStyle(sf::Text::Italic | sf::Text::Bold);
  start_menu_play_option.setFont(font_game);
  start_menu_play_option.setCharacterSize(30);
  start_menu_play_option.setFillColor(sf::Color(0, 0, 0, 255));
  start_menu_play_option.setPosition
    (
      window.getSize().x/3 - start_menu_play_option.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - start_menu_play_option.getGlobalBounds().height/2
    );
}
void Game::startScreenQuit()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  start_menu_quit_option.setString("Quit");
  start_menu_quit_option.setFont(font_game);
  start_menu_quit_option.setCharacterSize(30);
  start_menu_quit_option.setFillColor(sf::Color(0, 0, 0, 255));
  start_menu_quit_option.setPosition
    (
      window.getSize().x - window.getSize().x/3 - start_menu_quit_option.getGlobalBounds().width/2 - window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - start_menu_quit_option.getGlobalBounds().height/2
    );
}

// Mode Screen
void Game::setModeScreen()
{
  // Background
  modeScreenBG();

  // Text
  modeScreenText();

  // Normal Mode Option
  modeScreenNorm();

  // Gravity Mode Option
  modeScreenGrav();

  // Quadratic Mode Option
  modeScreenQuad();

  // Sin Mode Option
  modeScreenSin();

}
void Game::modeScreenBG()
{
  if (!mode_menu_bg_texture.loadFromFile("Data/Images/start_bg.png"))
  {
    std::cout << "Start menu background texture not loaded\n";
  }
  mode_menu_bg_sprite.setTexture(mode_menu_bg_texture);
  mode_menu_bg_sprite.setScale
    (
      window.getSize().x / mode_menu_bg_sprite.getGlobalBounds().width,
      window.getSize().y / mode_menu_bg_sprite.getGlobalBounds().height
    );
  mode_menu_bg_sprite.setPosition(0, 0);
}
void Game::modeScreenText()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  mode_menu_text.setString("Choose game mode (mouse only)");

  mode_menu_text.setFont(font_game);
  mode_menu_text.setCharacterSize(30);
  mode_menu_text.setFillColor(sf::Color(0, 0, 0, 255));
  mode_menu_text.setPosition
    (
      window.getSize().x/4 + window.getSize().x/30,
      window.getSize().y/5 - mode_menu_text.getGlobalBounds().height/2
    );
}
void Game::modeScreenNorm()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  normal_mode_text.setString("Normal\nMode");
  normal_mode_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  normal_mode_text.setFont(font_game);
  normal_mode_text.setCharacterSize(30);
  normal_mode_text.setFillColor(sf::Color(0, 0, 0, 255));
  normal_mode_text.setPosition
    (
      normal_mode_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - normal_mode_text.getGlobalBounds().height/2 - 80
    );
}
void Game::modeScreenGrav()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  grav_mode_text.setString("Gravity\nMode");
  grav_mode_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  grav_mode_text.setFont(font_game);
  grav_mode_text.setCharacterSize(30);
  grav_mode_text.setFillColor(sf::Color(0, 0, 0, 255));
  grav_mode_text.setPosition
    (
      window.getSize().x/3 - grav_mode_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - grav_mode_text.getGlobalBounds().height/2 - 80
    );
}
void Game::modeScreenQuad()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  quad_mode_text.setString("Quadratic\nMode");
  quad_mode_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  quad_mode_text.setFont(font_game);
  quad_mode_text.setCharacterSize(30);
  quad_mode_text.setFillColor(sf::Color(0, 0, 0, 255));
  quad_mode_text.setPosition
    (
      window.getSize().x - window.getSize().x/3 - quad_mode_text.getGlobalBounds().width/2 - window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - quad_mode_text.getGlobalBounds().height/2 - 80
    );
}
void Game::modeScreenSin()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  sin_mode_text.setString("Sin Curve\nMode");
  sin_mode_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  sin_mode_text.setFont(font_game);
  sin_mode_text.setCharacterSize(30);
  sin_mode_text.setFillColor(sf::Color(0, 0, 0, 255));
  sin_mode_text.setPosition
    (
      window.getSize().x - sin_mode_text.getGlobalBounds().width - window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - sin_mode_text.getGlobalBounds().height/2 - 80
    );
}

// Game Screen
void Game::setGameScreen()
{
  // BG GAME
  gameScreenBG();

  // Player
  gameScreenPlayer();
  gameScreenLives();
  gameScreenScore();

  // Level
  levelOne();
}
void Game::gameScreenBG()
{
  if (!game_bg_texture.loadFromFile("Data/Images/game_bg.jpg"))
  {
    std::cout << "Game background texture not loaded\n";
  }
  game_bg_sprite.setTexture(game_bg_texture);
  game_bg_sprite.setScale
    (
      window.getSize().x / game_bg_sprite.getGlobalBounds().width,
      window.getSize().y / game_bg_sprite.getGlobalBounds().height
    );
  game_bg_sprite.setPosition(0, 0);
}
void Game::gameScreenLives()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  lives_text.setString("Lives:" + std::to_string(player->returnLives()));
  lives_text.setFont(font_game);
  lives_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  lives_text.setCharacterSize(30);
  lives_text.setFillColor(sf::Color(255,255,255,255));
  lives_text.setPosition
    (
      window.getSize().x - lives_text.getGlobalBounds().width - window.getSize().x/20,
      0
    );
}
void Game::gameScreenScore()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  score_text.setString("Score:" + std::to_string(player->returnScore()));
  score_text.setFont(font_game);
  score_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  score_text.setCharacterSize(30);
  score_text.setFillColor(sf::Color(255,255,255,255));
  score_text.setPosition
    (
      window.getSize().x/20,
      0
    );
}
void Game::gameScreenPlayer()
{
  player = new Player("Data/Images/SpaceShooterRedux/PNG/playerShip3_red.png");
  player->initPos(window);
  cannon = new Cannon(window, *player);
}
void Game::levelOne()
{
  // Enemies
  enemy = new Enemy;
  enemies = new Enemy[enemy->returnGridSizeX() * enemy->returnGridSizeY()];
  resetEnemyPos();

}

// End Screen
void Game::setEndScreen()
{
  // BG
  endScreenBG();

  // Text
  endScreenText();

  // Play Again Option
  endScreenPlay();

  // Quit option
  endScreenQuit();
}
void Game::endScreenBG()
{
  if (!end_bg_texture.loadFromFile("Data/Images/end_bg.png"))
  {
    std::cout << "End screen background texture not loaded\n";
  }
  end_bg_sprite.setTexture(end_bg_texture);
  end_bg_sprite.setScale
    (
      window.getSize().x / end_bg_sprite.getGlobalBounds().width,
      window.getSize().y / end_bg_sprite.getGlobalBounds().height
    );
  end_bg_sprite.setPosition(0, 0);
}
void Game::endScreenText()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  end_title_text.setString("GAME OVER");
  end_title_text.setFont(font_game);
  end_title_text.setCharacterSize(80);
  end_title_text.setFillColor(sf::Color(0, 0, 0, 255));
  end_title_text.setPosition
    (
      window.getSize().x/2 - end_title_text.getGlobalBounds().width/2,
      window.getSize().y/3 - end_title_text.getGlobalBounds().height/2 - window.getSize().y/10
    );
}
void Game::endScreenPlay()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  play_again_end_text.setString(">Play again<");
  play_again_end_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  play_again_end_text.setFont(font_game);
  play_again_end_text.setCharacterSize(30);
  play_again_end_text.setFillColor(sf::Color(0, 0, 0, 255));
  play_again_end_text.setPosition
    (
      window.getSize().x/3 - play_again_end_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - play_again_end_text.getGlobalBounds().height/2
    );
}
void Game::endScreenQuit()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  quit_end_text.setString("Quit");
  quit_end_text.setFont(font_game);
  quit_end_text.setCharacterSize(30);
  quit_end_text.setFillColor(sf::Color(0, 0, 0, 255));
  quit_end_text.setPosition
    (
      window.getSize().x - window.getSize().x/3 - quit_end_text.getGlobalBounds().width/2,
      window.getSize().y - window.getSize().y/3 - quit_end_text.getGlobalBounds().height/2
    );
}

// Pause Screen
void Game::setPauseMenu()
{
  // Background
  pauseMenuBG();

  // Pause Text
  pauseMenuText();

  // Restart Option
  pauseMenuRestart();

  // Quit option
  pauseMenuQuit();
}
void Game::pauseMenuBG()
{
  if (!background_pause_texture.loadFromFile("Data/Images/pause_bg.png"))
  {
    std::cout << "Pause background texture not loaded\n";
  }
  background_pause_sprite.setTexture(background_pause_texture);
  background_pause_sprite.setScale
    (
      window.getSize().x / background_pause_sprite.getGlobalBounds().width,
      window.getSize().y / background_pause_sprite.getGlobalBounds().height
    );
  background_pause_sprite.setPosition(0, 0);
}
void Game::pauseMenuText()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  pause_title_text.setString("PAUSE");
  pause_title_text.setFont(font_game);
  pause_title_text.setCharacterSize(80);
  pause_title_text.setFillColor(sf::Color(0, 0, 0, 255));
  pause_title_text.setPosition
    (
      window.getSize().x/2 - pause_title_text.getGlobalBounds().width/2,
      window.getSize().y/3 - pause_title_text.getGlobalBounds().height/2 - window.getSize().y/10
    );
}
void Game::pauseMenuRestart()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  restart_opt_pause_text.setString(">Restart<");
  restart_opt_pause_text.setStyle(sf::Text::Italic | sf::Text::Bold);
  restart_opt_pause_text.setFont(font_game);
  restart_opt_pause_text.setCharacterSize(30);
  restart_opt_pause_text.setFillColor(sf::Color(0, 0, 0, 255));
  restart_opt_pause_text.setPosition
    (
      window.getSize().x/3 - restart_opt_pause_text.getGlobalBounds().width/2 + window.getSize().x/20,
      window.getSize().y - window.getSize().y/3 - restart_opt_pause_text.getGlobalBounds().height/2
    );
}
void Game::pauseMenuQuit()
{
  if (!font_game.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout<<"Font not loaded\n";
  }
  quit_opt_pause_text.setString("Quit");
  quit_opt_pause_text.setFont(font_game);
  quit_opt_pause_text.setCharacterSize(30);
  quit_opt_pause_text.setFillColor(sf::Color(0, 0, 0, 255));
  quit_opt_pause_text.setPosition
    (
      window.getSize().x - window.getSize().x/3 - quit_opt_pause_text.getGlobalBounds().width,
      window.getSize().y - window.getSize().y/3 - quit_opt_pause_text.getGlobalBounds().height/2
    );
}

// Other
void Game::resetEnemyPos()
{
  for (int i = 0; i < enemies->returnGridSizeX(); i++)
  {
    for (int j = 0; j < enemies->returnGridSizeY(); j++)
    {
      if (j == 0)
      {
        enemies[i * enemies->returnGridSizeY() + j].addTexture(Enemy::Colours::Black);
      }
      else if (j == 1)
      {
        enemies[i * enemies->returnGridSizeY() + j].addTexture(Enemy::Colours::Blue);
      }
      else if (j == 2)
      {
        enemies[i * enemies->returnGridSizeY() + j].addTexture(Enemy::Colours::Green);
      }
      else
      {
        enemies[i * enemies->returnGridSizeY() + j].addTexture(Enemy::Colours::Orange);
      }

      enemies[i * enemies->returnGridSizeY() + j].getSprite()->setPosition
          (
            window.getSize().x/2 - ((enemies->spriteWidth() + window.getSize().x/40) * (enemies->returnGridSizeX()/2) - window.getSize().x/80) +
            ((enemies->spriteWidth() + window.getSize().x/40) * i),
            (enemies->spriteHeight() + window.getSize().y/40) * j
          );

//      if (normal_mode)
//      {
//        enemies[i * enemies->returnGridSizeY() + j].getSprite()->setPosition
//          (
//            window.getSize().x/2 - ((enemies->spriteWidth() + window.getSize().x/40) * (enemies->returnGridSizeX()/2) - window.getSize().x/80) +
//            ((enemies->spriteWidth() + window.getSize().x/40) * i),
//            (enemies->spriteHeight() + window.getSize().y/40) * j
//          );
//      }
//      else if (grav_mode)
//      {
//        enemies[i * enemies->returnGridSizeY() + j].getSprite()->setPosition
//          (
//            window.getSize().x/2 - ((enemies->spriteWidth() + window.getSize().x/40) * (enemies->returnGridSizeX()/2) - window.getSize().x/80) +
//            ((enemies->spriteWidth() + window.getSize().x/40) * i),
//            (enemies->spriteHeight() + window.getSize().y/40) * j
//          );
//      }
//      else if (quad_mode)
//      {
//        enemies[i * enemies->returnGridSizeY() + j].getSprite()->setPosition
//          (
//            window.getSize().x/2 - ((enemies->spriteWidth() + window.getSize().x/40) * (enemies->returnGridSizeX()/2) - window.getSize().x/80) +
//            ((enemies->spriteWidth() + window.getSize().x/40) * i),
//            (enemies->spriteHeight() + window.getSize().y/40) * j
//          );
//      }
//      else if (sin_mode)
//      {
//        enemies[i * enemies->returnGridSizeY() + j].getSprite()->setPosition
//          (
//            window.getSize().x/2 - ((enemies->spriteWidth() + window.getSize().x/40) * (enemies->returnGridSizeX()/2) - window.getSize().x/80) +
//            ((enemies->spriteWidth() + window.getSize().x/40) * i),
//            (enemies->spriteHeight() + window.getSize().y/40) * j
//          );
//      }

    }
  }
}
void Game::printScore()
{
  score_text.setString("Score:" + std::to_string(player->returnScore()));
}
void Game::endGameManager()
{
  int vis = 0;
  if (player->returnLives() == 0)
  {
    game_running = false;
    end_screen = true;
  }

  for (int i = 0; i < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++i)
  {

    if (!enemies[i].getVisibility())
    {
      vis = vis + 1;
    }

  }
  if (vis == enemies->returnGridSizeX() * enemies->returnGridSizeY())
  {
    game_running = false;
    end_screen = true;
  }

  for (int j = 0; j < enemies->returnGridSizeX() * enemies->returnGridSizeY(); ++j)
  {
    if (enemies[j].getVisibility() && enemies[j].getSprite()->getPosition().y > window.getSize().y - 100)
    {
      game_running = false;
      end_screen = true;
    }
  }
}

