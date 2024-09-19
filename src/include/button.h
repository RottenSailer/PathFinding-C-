#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position,
           const sf::Color& backgroundColor, const sf::Color& textColor,
           const std::string& text, sf::Font& font);

    void draw(sf::RenderWindow& window);
    bool isMouseOver(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);

    void setBackgroundColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Color originalColor;
};

#endif // BUTTON_H
