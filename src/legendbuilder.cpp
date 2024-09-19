#include "ui/legendbuilder.h"

LegendBuilder::LegendBuilder(float xPosition, float yStartPosition, sf::Font& font) : xPos(xPosition), yPos(yStartPosition), yOffset(30.f), font(font)
{
   // Initialize the background rectangle
   background.setFillColor(sf::Color(255, 255, 255, 200));
   background.setOutlineColor(sf::Color::Black);
   background.setOutlineThickness(2.f);
   background.setPosition(xPos, yPos - 10.f);  // Adjusted to include the title

   // Initialize the title
   title.setFont(font);
   title.setString("Legend:");
   title.setCharacterSize(16);
   title.setFillColor(sf::Color::Black);
   title.setPosition(xPos + 10.f, yPos);
}

void LegendBuilder::addItem(const sf::Color& color, const std::string& text)
{
   LegendItem item;

   item.iconShape.setSize(sf::Vector2f(20.f, 20.f));
   item.iconShape.setFillColor(color);
   item.iconShape.setPosition(xPos + 10.f, yPos + 30.f + items.size() * yOffset);

   item.iconText.setFont(font);
   item.iconText.setString(text);
   item.iconText.setCharacterSize(14);
   item.iconText.setFillColor(sf::Color::Black);
   item.iconText.setPosition(xPos + 40.f, yPos + 30.f + items.size() * yOffset);

   items.push_back(item);

   updateBackgroundSize();
}

void LegendBuilder::updateBackgroundSize()
{
   float height = 20.f + items.size() * yOffset + 20.f;  // Top padding + items + bottom padding
   background.setSize(sf::Vector2f(180.f, height));
}

void LegendBuilder::draw(sf::RenderWindow& window)
{
   window.draw(background);
   window.draw(title);

   for (const auto& item : items)
   {
      window.draw(item.iconShape);
      window.draw(item.iconText);
   }
}
