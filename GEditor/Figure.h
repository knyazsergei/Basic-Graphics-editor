#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Figure
{
private:
	sf::RectangleShape m_border;
	sf::Vector2f m_shift;
	sf::CircleShape m_resizeCircle;
	sf::Vector2f resizePos;
public:
	bool hidden = false;
	unsigned m_type;
	std::vector<sf::Vector2f> m_resizeCirclePoints;
	bool activeToggle = false;
	sf::Shape * shape = nullptr;
	Figure(unsigned type, unsigned size, sf::Vector2f position);
	Figure(unsigned type, sf::Vector2f size, sf::Vector2f position);
	void init(unsigned type, sf::Vector2f size, sf::Vector2f position);
	~Figure();
	
	void draw(sf::RenderWindow &);
	void setPosition(const sf::Vector2f &);
	void setSize(const sf::Vector2f &);
	void setScale(const sf::Vector2f &);
	void beginMove(const sf::Vector2f & position);
	void beginResize(const sf::Vector2f & position);
	void move(const sf::Vector2f &);
	void resize(const sf::Vector2f &);

	sf::Vector2f getSize();
};

