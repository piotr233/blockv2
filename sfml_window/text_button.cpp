//
// Created by studio25 on 01.08.2021.
//

#include "text_button.h"
#include <SFML/Graphics/RectangleShape.hpp>

bool sfml_window::TextButton::DetectHover(const Coord &press_point) {
  hover_ = structure_.CheckWBoundaries(press_point);
  return hover_;
}
bool sfml_window::TextButton::DetectPress(const Coord &press_point,
                                          sf::Event &event) {
  hover_ = structure_.CheckWBoundaries(press_point);
  return structure_.CheckWBoundaries(press_point) &&
         (event.type == sf::Event::MouseButtonReleased &&
          event.mouseButton.button == sf::Mouse::Left);
}
void sfml_window::TextButton::DrawToWindow(sf::RenderWindow &window) {


  if(display_background_) window.draw(background_);
 // if(hover_) text_.setScale(1.5,1.5);
  window.draw(text_);
}
sfml_window::TextButton::TextButton(const Coord &position,const std::string& text)
  {
  background_.setPosition((float)position.x,
                          (float)position.y);

  background_.setFillColor(sf::Color(195, 195, 195));
  background_.setOutlineColor(sf::Color(255, 255, 255));
  background_.setOutlineThickness(2);


  if (!font_.loadFromFile("C:\\Users\\studio25\\Documents\\blockv2\\sfml_"
                         "window\\assets\\Georama-Medium.ttf"))
    throw "bad file";

  text_.setPosition((float)position.x,
                    (float)position.y);
  text_.setFont(font_);
  text_.setString(text);
  text_.setCharacterSize(24);

  structure_ = {position,(unsigned)(22*text.size()),(unsigned)(24*2)};

  background_.setSize({(float)structure_.width, (float)structure_.height});


}
void sfml_window::TextButton::SetButtonColor(const sf::Color &button_color) {
  button_color_ = button_color;
  text_.setFillColor(button_color);
  text_.setOutlineColor(button_color);
  background_.setFillColor(Light(button_color_));
  background_.setOutlineColor(button_color_);
}
