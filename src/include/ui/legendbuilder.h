#ifndef LEGEND_BUILDER_H
#define LEGEND_BUILDER_H

#include <SFML/Graphics.hpp>
#include <vector>

class LegendBuilder
{
public:
    LegendBuilder(float xPosition, float yStartPosition, sf::Font& font);

    void addItem(const sf::Color& color, const std::string& text);
    void draw(sf::RenderWindow& window);

private:
    struct LegendItem
    {
        sf::RectangleShape iconShape;
        sf::Text           iconText;
    };

    float               xPos;
    float               yPos;
    float               yOffset;
    sf::Font&           font;
    sf::RectangleShape  background;
    sf::Text            title;
    std::vector<LegendItem> items;

    void updateBackgroundSize();
};

#endif // LEGEND_BUILDER_H
