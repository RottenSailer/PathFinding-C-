#include "button.h"

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

   // Center the text on the button
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
