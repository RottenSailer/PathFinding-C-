#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Button
{
public:
   Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor, const sf::Color& textColor,
          const std::string& text, sf::Font& font);

   virtual void draw(sf::RenderWindow& window);
   bool         isMouseOver(sf::RenderWindow& window);
   void         update(sf::RenderWindow& window);

   void setBackgroundColor(const sf::Color& color);
   void setTextColor(const sf::Color& color);

protected:
   sf::RectangleShape buttonShape;
   sf::Text           buttonText;
   sf::Color          originalColor;
};

// class DialogBox : public Button
// {
// public:
//    DialogBox(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor, const sf::Color& textColor,
//              const std::string& mainText, const std::vector<std::string>& options,
//              sf::Font& font);  // Reference to ButtonFactory
//
//    void draw(sf::RenderWindow& window) override;
//    int  show(sf::RenderWindow& window);
//
// private:
//    std::vector<Button*> optionButtons;  // Pointers to option buttons
//    bool                 isExpanded;     // Indicates if drop-down is expanded
//
//    void initializeOptions(const std::vector<std::string>& options, sf::Font& font);
//    int  handleEvents(sf::RenderWindow& window);
// };

class ButtonFactory
{
public:
   ButtonFactory(const sf::Color& buttonColor, const sf::Color& textColor, sf::Font& font);
   ButtonFactory(const sf::Color& buttonColor, const sf::Color& textColor, sf::Font& font, sf::Vector2f& size);

   ~ButtonFactory() = default;
   Button* createButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text);
   Button* createButton(const sf::Vector2f& position, const std::string& text);
   void    updateAll(sf::RenderWindow& window);
   void    drawAll(sf::RenderWindow& window);

private:
   sf::Vector2f size;
   sf::Color    buttonColor;
   sf::Color    textColor;
   sf::Font&    font;

   std::vector<std::unique_ptr<Button>> buttons;
};

#endif  // BUTTON_H
