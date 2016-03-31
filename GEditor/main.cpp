#include <map>
#include "Figures.h"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Geditor", sf::Style::Default, settings);
	sf::View view = sf::View(sf::FloatRect(0, 0, float(window.getSize().x), float(window.getSize().y)));
	window.setView(view);
	window.setVerticalSyncEnabled(true);
	sf::Event event;
	sf::CircleShape shape;
	Figures figures;

	std::map<std::string, sf::RectangleShape> controls{
		{"undo", sf::RectangleShape()},
		{"redo", sf::RectangleShape()},
		{"addTriangle",sf::RectangleShape()},
		{"addCircle",sf::RectangleShape()},
		{"addRectangle",sf::RectangleShape ()}
	};
	

	controls["undo"].setFillColor(sf::Color::Magenta);
	controls["redo"].setFillColor(sf::Color::Cyan);
	controls["addTriangle"].setFillColor(sf::Color::Blue);
	controls["addCircle"].setFillColor(sf::Color::Green);
	controls["addRectangle"].setFillColor(sf::Color::Red);

	unsigned i = 0;
	for (std::map<std::string, sf::RectangleShape>::iterator it = controls.begin(); it != controls.end(); ++it, i++)
	{
		it->second.setSize({ 20, 20 });
		it->second.setPosition({ 20.f * i, 20 });
	}

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		figures.draw(window);

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f cursorPosition = window.mapPixelToCoords(pixelPos);

		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased) {
				figures.click(cursorPosition);
				if (controls["undo"].getGlobalBounds().contains(cursorPosition))
					figures.undo();
				else if (controls["redo"].getGlobalBounds().contains(cursorPosition))
					figures.redo();
				else if (controls["addCircle"].getGlobalBounds().contains(cursorPosition)) {
					figures.add(0, 40, sf::Vector2f(window.getSize()) * 0.5f);
				}
				else if (controls["addTriangle"].getGlobalBounds().contains(cursorPosition)) {
					figures.add(1, 40, sf::Vector2f(window.getSize()) * 0.5f);
				}
				else if (controls["addRectangle"].getGlobalBounds().contains(cursorPosition)) {
					figures.add(2, 40, sf::Vector2f(window.getSize()) * 0.5f);
				}
			}else if (event.type == sf::Event::MouseButtonPressed) {
				figures.press(cursorPosition);
			}
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Delete) {
				figures.remove();
			}
		}
		figures.update(cursorPosition);
		for (auto it : controls) {
			window.draw(it.second);
		}
		window.display();
	}
	return 0;
}