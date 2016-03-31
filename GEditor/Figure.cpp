#include "Figure.h"

Figure::Figure(unsigned type, unsigned size, sf::Vector2f position) :m_type(type) {
	init(type, sf::Vector2f(size, size), position);
}
Figure::Figure(unsigned type, sf::Vector2f size, sf::Vector2f position) : m_type(type) {
	init(type, size, position);
}

void Figure::init(unsigned type, sf::Vector2f size, sf::Vector2f position)
{
	sf::Shape * buffer;
	switch (type)
	{
	case 0://Circle
		shape = new sf::CircleShape(size.x * 0.5f);
		shape->setOrigin(size.x * 0.5f, size.y * 0.5f);
		break;
	case 1://triangle
		buffer = new sf::CircleShape(size.x * 0.5f);
		((sf::CircleShape *)buffer)->setPointCount(3);
		shape = buffer;
		shape->setOrigin(size.x * 0.5f, size.y * 0.5f);
		break;
	case 2://rectangle
		shape = new sf::RectangleShape(sf::Vector2f(float(size.x), float(size.y)));
		shape->setOrigin(size.x * 0.5f, size.y * 0.5f);
		break;
	default:
		shape = nullptr;
		break;
	}

	shape->setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 100));

	m_border.setSize(sf::Vector2f(size.x, size.y));
	m_border.setOrigin({size.x * 0.5f, size.y * 0.5f});
	m_border.setOutlineThickness(1);
	m_border.setOutlineColor(sf::Color(0, 148, 255, 220));
	m_border.setFillColor(sf::Color::Transparent);

	m_resizeCircle.setRadius(2);
	m_resizeCircle.setFillColor(sf::Color::Red);
	m_resizeCircle.setOrigin(2, 2);
	m_resizeCirclePoints.resize(4);

	setPosition(position);
}


Figure::~Figure()
{
	//delete shape;
}

void Figure::draw(sf::RenderWindow & window) {
	if (!hidden) {
		window.draw(*shape);
		if (activeToggle) {
			window.draw(m_border);

			for (auto it : m_resizeCirclePoints) {
				m_resizeCircle.setPosition(it);
				window.draw(m_resizeCircle);
			}
		}
	}
}

void Figure::setPosition(const sf::Vector2f & position){
	shape->setPosition(position);
	m_border.setPosition(position);

	m_resizeCirclePoints[0] = sf::Vector2f(m_border.getPosition().x - m_border.getSize().x * 0.5f, m_border.getPosition().y - m_border.getSize().y * 0.5f);
	m_resizeCirclePoints[1] = sf::Vector2f(m_border.getPosition().x + m_border.getSize().x * 0.5f, m_border.getPosition().y - m_border.getSize().y * 0.5f);
	m_resizeCirclePoints[2] = sf::Vector2f(m_border.getPosition().x + m_border.getSize().x * 0.5f, m_border.getPosition().y + m_border.getSize().y * 0.5f);
	m_resizeCirclePoints[3] = sf::Vector2f(m_border.getPosition().x - m_border.getSize().x * 0.5f, m_border.getPosition().y + m_border.getSize().y * 0.5f);

}

void Figure::setSize(const sf::Vector2f & size){
	if (m_type == 2) {
		((sf::RectangleShape *)shape)->setSize(size * 2.f);
		shape->setOrigin(size);
		m_border.setSize(size * 2.f);
	}
	else {
		float radius = sqrt(pow(size.x, 2) + pow(size.y, 2));
		((sf::CircleShape *)shape)->setRadius(radius);
		shape->setOrigin(radius, radius);
		m_border.setSize(sf::Vector2f(1, 1) * radius * 2.f);
	}
	m_border.setOrigin(sf::Vector2f(m_border.getSize()) * 0.5f);
	setPosition(m_border.getPosition());
	
}
void Figure::setScale(const sf::Vector2f & size) {
	shape->setScale(size);
}

void Figure::beginMove(const sf::Vector2f & position) {
	m_shift = m_border.getPosition() - position;
}

void Figure::beginResize(const sf::Vector2f & position) {
	resizePos = position;
}

void Figure::move(const sf::Vector2f & position) {
	setPosition(position + m_shift);
}

void Figure::resize(const sf::Vector2f & position) {
	if (position != resizePos) {
		sf::Vector2f scaleVector = position - shape->getPosition();// position - resizePos;
		setSize(scaleVector);
	}
}


sf::Vector2f Figure::getSize() {
	if (m_type == 2) {
		return ((sf::RectangleShape *)shape)->getSize();
	}
	else {
		return sf::Vector2f(((sf::CircleShape *)shape)->getRadius(), 0);
	}
}