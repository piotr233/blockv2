//
// Created by studio25 on 03.08.2021.
//

#include "run_simulation.h"
#include <fstream>

/// replace with exists[(int)Assets::x]
#define EXIST(x) exists[(int)Assets::x]
sfml_window::RunSimulation::~RunSimulation() {
  for (auto &button : buttons_) {
    delete button;
  }
}
sfml_window::RunSimulation::RunSimulation(unsigned int window_width,
                                          unsigned int window_height,
                                          LevelInfo &level_info)
    : local_board_(level_info), window_width_(window_width),
      window_height_(window_height) {
  LoadColors();
  LoadButtons();
  LoadAssets(level_info.GetName());
  GenGrid();
}
void sfml_window::RunSimulation::DrawToWindow(sf::RenderWindow &window) {
  // window.clear(color_palette_[(unsigned)GuiColor::MENU_BACKGROUND_COLOR]);
  window.draw(background_sprite_);
  if (display_grid_)
    DrawGrid(window);

  for (const auto &button : buttons_)
    button->DrawToWindow(window);

  DrawCells(window);
}
sfml_window::ContextEvent
sfml_window::RunSimulation::HandleEvent(sf::Event &event,
                                        const sf::RenderWindow &window) {

  int mouse_x = sf::Mouse::getPosition(window).x;
  int mouse_y = sf::Mouse::getPosition(window).y;

  mouse_x = mouse_x < 0 ? 0 : mouse_x;
  mouse_y = mouse_y < 0 ? 0 : mouse_y;

  mouse_x = mouse_x <= window_width_ ? mouse_x : window_width_ - 1;
  mouse_y = mouse_y <= window_height_ ? mouse_y : window_height_ - 1;

  for (unsigned id = 0; id < buttons_.size(); id++)

    if (buttons_[id]->DetectInteraction({(unsigned)mouse_x, (unsigned)mouse_y},
                                        event))

      switch ((RunSimulationButton)id) {

      case RunSimulationButton::EXIT:
        return ContextEvent::EXIT;
      case RunSimulationButton::STOP_SIMULATION:
        break;
      case RunSimulationButton::END_SIMULATION:
        break;
      case RunSimulationButton::SIZE:
        break;
      }
}
void sfml_window::RunSimulation::LoadColors() {
  color_palette_[(unsigned)GuiColor::MENU_PRIMARY_COLOR] = sf::Color(0x0035d6);
  color_palette_[(unsigned)GuiColor::MENU_SECONDARY_COLOR] =
      sf::Color(0xa000d6);
  color_palette_[(unsigned)GuiColor::MENU_TERTIARY_COLOR] = sf::Color(0xd6a000);
  color_palette_[(unsigned)GuiColor::MENU_BACKGROUND_COLOR] =
      sf::Color(27, 27, 27);
  color_palette_[(unsigned)GuiColor::DANGER_COLOR] = sf::Color(255, 0, 0);
  color_palette_[(unsigned)GuiColor::WARNING_COLOR] = sf::Color(255, 255, 0);
  color_palette_[(unsigned)GuiColor::INFORMATIVE_COLOR] = sf::Color(0, 0, 255);
  color_palette_[(unsigned)GuiColor::SAFE_COLOR] = sf::Color(0, 255, 0);
}
void sfml_window::RunSimulation::LoadButtons() {

  std::string directory = "../sfml_window/assets/";

  buttons_[(unsigned)RunSimulationButton::EXIT] = new ImageButton(
      Rect({window_width_ - 36, 4}, 32, 32), directory + "cancel-button.png",
      color_palette_[(unsigned)GuiColor::DANGER_COLOR]);
  buttons_[(unsigned)RunSimulationButton::STOP_SIMULATION] = new ImageButton(
      Rect({window_width_ - 74, 4}, 32, 32), directory + "pause-button.png",
      color_palette_[(unsigned)GuiColor::WARNING_COLOR]);
  buttons_[(unsigned)RunSimulationButton::END_SIMULATION] = new ImageButton(
      Rect({window_width_ - 112, 4}, 32, 32), directory + "next.png",
      color_palette_[(unsigned)GuiColor::DANGER_COLOR]);
}

void sfml_window::RunSimulation::DrawGrid(sf::RenderWindow &window) {

  for (const auto &box : grid_)
    window.draw(box);
}
void sfml_window::RunSimulation::GenGrid() {

  // all cells are squares so the width = height
  // between all cells and surrounding them is 3px wide border
  double vertical_cell_size =
      ((window_width_ - 3) - 3 * local_board_.GetWidth()) /
      (double)(local_board_.GetWidth());
  //right border       /\        left border on every cell    /\

  double horizontal_cell_size =
      ((window_height_ - 3) - 3 * local_board_.GetHeight()) /
      (double)(local_board_.GetHeight());
  //right border       /\        left border on every cell    /\


  // the actual cell size must be smaller of the two above
  cell_size_ = vertical_cell_size < horizontal_cell_size
                   ? (unsigned)vertical_cell_size
                   : (unsigned)horizontal_cell_size;

  unsigned pixel_shift = cell_size_ + 3;
  // generate grid
  for (int y = 0; y < local_board_.GetHeight(); ++y)
    for (int x = 0; x < local_board_.GetWidth(); ++x) {
      grid_.push_back(sf::RectangleShape());
      grid_.back().setPosition(3 + x * pixel_shift, 3 + y * pixel_shift);
      grid_.back().setSize({(float)cell_size_, (float)cell_size_});
      grid_.back().setFillColor(sf::Color::Transparent);
      grid_.back().setOutlineColor(sf::Color::White);
      grid_.back().setOutlineThickness(1);
    }

  // center grid
  unsigned right_shift =
      (window_width_ - (3 + local_board_.GetWidth() * pixel_shift)) / 2;
  unsigned down_shift =
      (window_height_ - (3 + local_board_.GetHeight() * pixel_shift)) / 2;

  for (auto &square : grid_) {
    square.move(right_shift, down_shift);
  }
}
void sfml_window::RunSimulation::LoadBackground(const std::string &level_path) {

  if (!background_texture_.loadFromFile(level_path))
    throw "error";

  background_sprite_.setTexture(background_texture_);
  background_sprite_.setScale(
      (float)window_width_ / (float)background_texture_.getSize().x,
      (float)window_height_ / (float)background_texture_.getSize().y);
}
void sfml_window::RunSimulation::LoadAssets(const std::string &level_name) {

  bool exists[(unsigned)Assets::SIZE];
  static std::string file_names[(unsigned)Assets::SIZE];
  {
    file_names[(unsigned)Assets::BASIC] = "basic.png";
    file_names[(unsigned)Assets::BEDROCK] = "bedrock.png";
    file_names[(unsigned)Assets::TURN_C] = "turn_clockwise.png";
    file_names[(unsigned)Assets::TURN_CC] = "turn_counterclockwise.png";
    file_names[(unsigned)Assets::GOAL] = "goal.png";
    file_names[(unsigned)Assets::ENEMY] = "enemy.png";
    file_names[(unsigned)Assets::ENGINE_U] = "engine_up.png";
    file_names[(unsigned)Assets::ENGINE_D] = "engine_down.png";
    file_names[(unsigned)Assets::ENGINE_L] = "engine_left.png";
    file_names[(unsigned)Assets::ENGINE_R] = "engine_right.png";
    file_names[(unsigned)Assets::FACTORY_U] = "factory_up.png";
    file_names[(unsigned)Assets::FACTORY_D] = "factory_down.png";
    file_names[(unsigned)Assets::FACTORY_L] = "factory_left.png";
    file_names[(unsigned)Assets::FACTORY_R] = "factory_right.png";
    file_names[(unsigned)Assets::TP] = "tp.png";
    file_names[(unsigned)Assets::BACKGROUND] = "background.png";
  } // set filenames

  const std::string kDir = "../levels/" + level_name + "/";
  const std::string kDefaultDir = "../levels/default/";

  // detect custom assets
  std::fstream file;
  for (int i = 0; i < (unsigned)Assets::SIZE; i++) {
    std::string full_path = kDir + file_names[i];
    file.open(full_path.c_str());
    exists[i] = file.good();
    file.close();
  }

  {
    // loading images is hard
    // because user is dudu,
    // and I need to apologise for it
    //
    // ok nessery images:
    // basic, bedrock, goal, enemy
    //
    // turn has 2 versions
    // engine 4
    // factory 4
    // and tp infinite but it's hard to have infinite

    // ok, basic rules :
    // 1. the names of the files are strict,
    // 2. if directory has image for block use this one
    // if not use default one
    // 3.  only one of the variant blocks must be present, rest will be created
    // by flipping
    //  a) if the up is present down will be created first
    //   than the check will be run for left and right ones
    //  b) if the left and right are present that's it
    //   if only one is present the second will be created by vertical flip
  }

  // for background
  if (exists[(unsigned)Assets::BACKGROUND])
    LoadBackground(kDir + file_names[(unsigned)Assets::BACKGROUND]);
  else
    LoadBackground(kDefaultDir + file_names[(unsigned)Assets::BACKGROUND]);

  // load custom assets (if one exist)
  // all loaded this way assets are simple meaning they have defined counterpart
  // in default directory
  for (int i = 0; i < (unsigned)Assets::TURN_C; i++)
    if (exists[i]) {
      std::string full_path = kDir + file_names[i];
      LoadCell((Assets)i, full_path);
    } else {
      std::string full_path = kDefaultDir + file_names[i];
      LoadCell((Assets)i, full_path);
    }

  if (EXIST(TURN_C) and not EXIST(TURN_CC)) {
    CopyCell(Assets::TURN_CC, Assets::TURN_C, 180);
  } else if (not EXIST(TURN_C) and EXIST(TURN_CC)) {
    CopyCell(Assets::TURN_C, Assets::TURN_CC, 180);
  } else if (not EXIST(TURN_C) and not EXIST(TURN_CC)) {
    LoadCell(Assets::TURN_CC, kDefaultDir + file_names[(int)Assets::TURN_CC]);
    CopyCell(Assets::TURN_C, Assets::TURN_CC, 180);
  } else
    throw "error";
  // todo these  need to be done better
  if (not EXIST(ENGINE_U) and not EXIST(ENGINE_D) and not EXIST(ENGINE_L) and
      not EXIST(ENGINE_R)) {
    LoadCell(Assets::ENGINE_U, kDefaultDir + file_names[(int)Assets::ENGINE_U]);
    CopyCell(Assets::ENGINE_D, Assets::ENGINE_U, 180);
    CopyCell(Assets::ENGINE_R, Assets::ENGINE_U, 90);
    CopyCell(Assets::ENGINE_L, Assets::ENGINE_R, 180);
  }
  if (not EXIST(FACTORY_U) and not EXIST(FACTORY_D) and not EXIST(FACTORY_L) and
      not EXIST(FACTORY_R)) {
    LoadCell(Assets::FACTORY_R,
             kDefaultDir + file_names[(int)Assets::FACTORY_R]);
    CopyCell(Assets::FACTORY_L, Assets::FACTORY_R, 180);
    CopyCell(Assets::FACTORY_D, Assets::FACTORY_R, 90);
    CopyCell(Assets::FACTORY_U, Assets::FACTORY_D, 180);
  }


  // to finish off
//  for (auto &cell :cells_)
//    cell.first.setSmooth(true);

}

sf::Texture &sfml_window::RunSimulation::Texture(sfml_window::Assets cell) {
  return cells_[(unsigned)cell].first;
}
sf::Sprite &sfml_window::RunSimulation::Sprite(sfml_window::Assets cell) {
  return cells_[(unsigned)cell].second;
}

void sfml_window::RunSimulation::CopyCell(Assets copy, Assets original,
                                          double angle) {
  Texture(copy).create(Texture(original).getSize().x,
                       Texture(original).getSize().y);
  Texture(copy).update(Texture(original), 0, 0);
  Sprite(copy).setTexture(Texture(copy));
  Sprite(copy).rotate(angle);
}

void sfml_window::RunSimulation::LoadCell(Assets cell,
                                          const std::string &asset_path) {

  if (!Texture(cell).loadFromFile(asset_path))
    throw "error";

  Sprite(cell).setTexture(Texture(cell));

}
void sfml_window::RunSimulation::DrawCells(sf::RenderWindow &window) {
  // todo 1. proper display (this function)

  // todo 2. finish load cells function

  // todo 3. test this shit

  // todo 4. better assets

  // todo 5. let ti move

  for (unsigned y = 0; y < local_board_.GetHeight() * local_board_.GetWidth();
       ++y)
    switch (local_board_.GetCell(y)->GetType()) {
    case BotType::BASIC:
    case BotType::TURN:
    case BotType::BEDROCK:
    case BotType::GOAL:
      Sprite(Assets::GOAL).setPosition(grid_[y].getPosition());
      Sprite(Assets::GOAL)
          .setScale(
              (float)cell_size_ / (float)Texture(Assets::GOAL).getSize().x,
              (float)cell_size_ / (float)Texture(Assets::GOAL).getSize().y);
      window.draw(Sprite(Assets::GOAL));
      break;
    case BotType::ENEMY:
    case BotType::ENGINE:
    case BotType::FACTORY:
    case BotType::TP:
    case BotType::EMPTY:
      break;
    default:

      throw "error";
    }
}
