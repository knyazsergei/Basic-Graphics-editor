#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Figure.h"

class Figures
{
private:
	sf::Vector2f polarToCartesian(float radius, float alpha);
	bool m_moving = false;
	int m_resizeNum = -1;
	sf::Vector2f m_mousePressedPosition;
	std::vector<int> m_typeOfAction;
	std::vector<std::vector<float>> m_actionData;
	int lastIteration = 0;
public:
	std::vector<Figure> list;
	Figures();
	~Figures();
	
	void draw(sf::RenderWindow & window);
	void add(unsigned type, unsigned size, sf::Vector2f position);
	void click(sf::Vector2f position);
	void press(sf::Vector2f position);
	void update(sf::Vector2f posiiton);
	void remove();

	void addEvent(int type, std::vector<float> list);
	void undo();
	void redo();
};

