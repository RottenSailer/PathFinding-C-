#include "ui/button.h"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor, const sf::Color& textColor,
               const std::string& text, sf::Font& font)
{
   buttonShape.setSize(size);
   buttonShape.setPosition(position);
   buttonShape.setFillColor(backgroundColor);
   originalColor = backgroundColor;

   buttonText.setFont(font);
   buttonText.setString(text);
   buttonText.setFillColor(textColor);
   buttonText.setCharacterSize(18);

   sf::FloatRect textRect = buttonText.getLocalBounds();
   buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
   buttonText.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::draw(sf::RenderWindow& window)
{
   window.draw(buttonShape);
   window.draw(buttonText);
}

bool Button::isMouseOver(sf::RenderWindow& window)
{
   sf::Vector2i  mousePos   = sf::Mouse::getPosition(window);
   sf::FloatRect buttonRect = buttonShape.getGlobalBounds();
   return buttonRect.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

void Button::update(sf::RenderWindow& window)
{
   if (isMouseOver(window))
   {
      buttonShape.setFillColor(sf::Color(150, 150, 150));
   }
   else
   {
      buttonShape.setFillColor(originalColor);
   }
}

void Button::setBackgroundColor(const sf::Color& color)
{
   buttonShape.setFillColor(color);
   originalColor = color;
}

void Button::setTextColor(const sf::Color& color)
{
   buttonText.setFillColor(color);
}

ButtonFactory::ButtonFactory(const sf::Color& buttonColor, const sf::Color& textColor, sf::Font& font)
  : buttonColor(buttonColor), textColor(textColor), font(font), size({100.f, 40.f})
{}

ButtonFactory::ButtonFactory(const sf::Color& buttonColor, const sf::Color& textColor, sf::Font& font, sf::Vector2f& size)
  : buttonColor(buttonColor), textColor(textColor), font(font), size(size)
{}

Button* ButtonFactory::createButton(const sf::Vector2f& position, const std::string& text)
{
   buttons.emplace_back(std::make_unique<Button>(size, position, buttonColor, textColor, text, font));
   return buttons.back().get();
}

Button* ButtonFactory::createButton(const sf::Vector2f& newSize, const sf::Vector2f& position, const std::string& text)
{
   buttons.emplace_back(std::make_unique<Button>(newSize, position, buttonColor, textColor, text, font));
   return buttons.back().get();
}

void ButtonFactory::updateAll(sf::RenderWindow& window)
{
   for (auto& button : buttons)
   {
      button->update(window);
   }
}

void ButtonFactory::drawAll(sf::RenderWindow& window)
{
   for (auto& button : buttons)
   {
      button->draw(window);
   }
}

// DialogBox::DialogBox(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor, const sf::Color& textColor,
//                      const std::string& text, const std::vector<std::string>& options, sf::Font& font)
//   : Button(size, position, backgroundColor, textColor, text, font)
// {
//    float buttonWidth      = 100.f;
//    float buttonHeight     = 40.f;
//    float totalButtonWidth = options.size() * buttonWidth + (options.size() - 1) * 20.f;
//    float startX           = this->buttonShape.getPosition().x + (this->buttonShape.getSize().x - totalButtonWidth) / 2.f;
//    float yPosition        = this->buttonShape.getPosition().y + this->buttonShape.getSize().y - buttonHeight - 20.f;
//
//    for (size_t i = 0; i < options.size(); ++i)
//    {
//       sf::Vector2f buttonPosition(startX + i * (buttonWidth + 20.f), yPosition);
//       Button button(sf::Vector2f(buttonWidth, buttonHeight), buttonPosition, sf::Color(100, 100, 250), sf::Color::White, options[i], font);
//       optionButtons.push_back(button);
//    }
// }

// void DialogBox::draw(sf::RenderWindow& window)
// {
//    window.draw(buttonShape);
//    window.draw(buttonText);
//
//    for (auto& button : optionButtons)
//    {
//       button.draw(window);
//    }
// }
//
// int DialogBox::handleEvents(sf::RenderWindow& window)
// {
//    sf::Event event;
//    while (window.pollEvent(event))
//    {
//       if (event.type == sf::Event::Closed)
//       {
//          window.close();
//          return -1;
//       }
//
//       else if (event.type == sf::Event::MouseButtonPressed)
//       {
//          if (event.mouseButton.button == sf::Mouse::Left)
//          {
//             for (size_t i = 0; i < optionButtons.size(); ++i)
//             {
//                if (optionButtons[i].isMouseOver(window))
//                {
//                   return static_cast<int>(i);
//                }
//             }
//          }
//       }
//    }
//    return -2;
// }
//
// int DialogBox::show(sf::RenderWindow& window)
//
// {
//     while (window.isOpen())
//     {
//         int selection = handleEvents(window);
//         if (selection >= 0)
//         {
//             return selection; // User made a selection
//         }
//         else if (selection == -1)
//         {
//             return -1; // Window closed
//         }
//
//         // Update buttons
//         mainButton.update(window);
//         if (isExpanded)
//         {
//             for (auto& button : optionButtons)
//             {
//                 button->update(window);
//             }
//         }
//
//         // Draw drop-down dialog box
//         window.clear();
//         draw(window);
//         window.display();
//     }
//     return -1; // Window closed
// }
