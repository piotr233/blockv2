//
// Created by studio25 on 03.08.2021.
//

#ifndef BLOCK_V2_SFML_WINDOW_CONTEXT_LEVEL_PLAYER_H_
#define BLOCK_V2_SFML_WINDOW_CONTEXT_LEVEL_PLAYER_H_

#include "../../game.h"
#include "context.h"
#include "image_button.h"
#include "text_button.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <short_level_info.h>

namespace sfml_window {

enum class LevelPlayerButton { EXIT, RUN_SIMULATION, SIZE };

class LevelPlayer : public Context {
public:

  /// run provided simulation
  /// \param level_info full level info object
  /// \param window_height of the window
  /// \param window_width of the window
  LevelPlayer(unsigned int window_width, unsigned int window_height, const std::string &level_path);


  void LoadColors() override {};

  void DrawToWindow(sf::RenderWindow &window) override;

  sfml_window::ContextEvent
  HandleEvent(sf::Event &event, const sf::RenderWindow &window) override;



private:
  void LoadButtons();
  /// loads assets from file to memory
  void LoadAssets(const std::string &level_name);

  /// creates copy of a given cell, used to generate assets by rotating already
  /// existing ones
  /// \param copy id of created cell
  /// \param original original cell id
  /// \param flip the transformation witch will bu used to create new cell
  void CopyCell(Assets copy, Assets original, FlipDirection flip);

  /// loads cell from file to memory
  /// \param cell cell id
  /// \param asset_path path to the correct .png file
  void LoadCell(Assets cell, const std::string &asset_path);

  /// load background to memory and scale it to screen
  /// \param background_path path to wanted background
  void LoadBackground(const std::string &background_path);
  void GenGrid();


  void DrawGrid(sf::RenderWindow &window);
  void DrawCells(sf::RenderWindow &window);
  void DrawCell(sf::RenderWindow &window, sfml_window::Assets id,
                unsigned position);

  sf::Texture &Texture(Assets cell);
  sf::Sprite &Sprite(Assets cell);


  /// \format in pixels
  /// x axis domain = <0,window_width_>
  unsigned window_width_;

  /// \format in pixels
  /// y axis domain = <0,window_height_>
  unsigned window_height_;

  /// background texture
  sf::Texture background_texture_;
  /// background sprite always provided by user
  sf::Sprite background_sprite_;

  std::string level_directory_;

  Board level_;


  /// size of a square cell
  unsigned cell_size_;

  std::array<Button *, (unsigned)LevelPlayerButton::SIZE> buttons_;

  std::array<std::pair<sf::Texture, sf::Sprite>, (unsigned)Assets::SIZE> cells_;

  std::vector<sf::RectangleShape> grid_;
};
}
#endif // BLOCK_V2_SFML_WINDOW_CONTEXT_LEVEL_PLAYER_H_
