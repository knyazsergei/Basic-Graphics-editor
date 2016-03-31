#include "Figures.h"



Figures::Figures()
{
}


Figures::~Figures()
{
	for (auto it : list) {
		delete it.shape;
	}
}

void Figures::draw(sf::RenderWindow & window) {
	for (auto it : list)
		it.draw(window);
}

void Figures::add(unsigned type, unsigned size, sf::Vector2f position) {
	list.push_back(Figure(type, size, position));
	lastIteration++;
	addEvent(0, { float(type), position.x, position.y, float(size), float(size) });
}

void Figures::click(sf::Vector2f position) {
	if (position == m_mousePressedPosition) {
		for (int i = list.size() - 1; i >= 0; i--) {
			if (list[i].shape->getGlobalBounds().contains(position)) {
				list[i].activeToggle = !list[i].activeToggle;
				break;
			}
		}
	}
	if (m_resizeNum != -1)
		m_resizeNum = -1;
	if (m_moving)
		m_moving = false;
}

void Figures::press(sf::Vector2f position) {
	m_mousePressedPosition = position;
	if (!m_moving) {
		bool onResizeCircle = false;
		bool onFigure = false;
		int resizeShift = 5;
		int resizePointNum;
		for (unsigned i = 0; i < list.size(); i++) {
			for (unsigned j = 0; j < list[i].m_resizeCirclePoints.size(); j++) {
				//Условия нажатия на одну из кнопок ресайза
				if ((list[i].m_resizeCirclePoints[j].x + resizeShift > position.x) &&
					(list[i].m_resizeCirclePoints[j].x - resizeShift < position.x) &&
					(list[i].m_resizeCirclePoints[j].y + resizeShift > position.y) &&
					(list[i].m_resizeCirclePoints[j].y - resizeShift < position.y)
					) 
				{
					resizePointNum = i;
					onResizeCircle = true;
					break;
				}
			}
			if (list[i].shape->getGlobalBounds().contains(position) && list[i].activeToggle) {
				onFigure = true;
				break;
			}
		}
		if (onResizeCircle) {
			list[resizePointNum].beginResize(position);
			m_resizeNum = resizePointNum;
			lastIteration++;
			m_typeOfAction.push_back(3);
			m_actionData.push_back({ float(resizePointNum),list[resizePointNum].getSize().x, list[resizePointNum].getSize().y });
		}
		if (onFigure) {
			m_moving = true;
			for (unsigned i = 0; i < list.size(); i++) {
				if (list[i].activeToggle) {
					list[i].beginMove(position);
					lastIteration++;
					m_typeOfAction.push_back(2);
					m_actionData.push_back({ float(i),list[i].shape->getPosition().x, list[i].shape->getPosition().y });
				}
			}
		}
	}
}

void Figures::update(sf::Vector2f positon) {
	if (m_moving) {
		for (unsigned i = 0; i < list.size();i++) {
			if (list[i].activeToggle) {
				list[i].move(positon);
			}
		}
	}
	if (m_resizeNum != -1) {
		list[m_resizeNum].resize(positon);
	}
}

void Figures::remove() {
	for (std::vector<Figure>::iterator i = list.begin(); i != list.end(); ) {
		if (i->activeToggle) {
			lastIteration++;
			m_typeOfAction.push_back(1);
			if (i->m_type == 2) {
				m_actionData.push_back(std::vector<float>({
					float(i->m_type),
					((sf::RectangleShape *)i->shape)->getPosition().x,
					((sf::RectangleShape *)i->shape)->getPosition().y,
					((sf::RectangleShape *)i->shape)->getSize().x,
					((sf::RectangleShape *)i->shape)->getSize().y
				}));
			}
			else {
				m_actionData.push_back(std::vector<float>({
					float(i->m_type),
					((sf::CircleShape *)i->shape)->getPosition().x,
					((sf::CircleShape *)i->shape)->getPosition().y,
					((sf::CircleShape *)i->shape)->getRadius() * 2
				}));
			}
			i = list.erase(i);
		}
		else {
			++i;
		}
	}
}

sf::Vector2f Figures::polarToCartesian(float radius, float alpha) {
	sf::Vector2f result;
	alpha = float(alpha * M_PI / 180);
	result.x = radius * cos(alpha);
	result.y = radius * sin(alpha);
	return result;
}

void Figures::addEvent(int type, std::vector<float> list) {
	m_typeOfAction.push_back(type);
	m_actionData.push_back(list);
}

void Figures::undo() {
	if (lastIteration > 0 && m_typeOfAction.size() > 0)
		return;

		lastIteration--;
		int type = m_typeOfAction[lastIteration];
		std::vector<float> data = m_actionData[lastIteration];
		switch (type) {
		case 0://undo add
			list.erase(list.begin() + int(data[0]));
			break;
		case 1://undo delete
			list.push_back(Figure(int(data[0]), sf::Vector2f(data[3], data[3]), sf::Vector2f(data[1], data[2])));
			break;
		case 2://undo moves
			list[int(data[0])].setPosition({ data[1], data[2] });
			break;
		case 3://undo resize
			list[int(data[0])].setSize({ data[1], data[2] });
			break;
		}
}
void Figures::redo() {
	
	int k = 0;
	for (auto i : m_actionData) {
		if (k == lastIteration)
			std::cout << ":::";
		std::cout << k << '(' << m_typeOfAction[0] << ')';
		for (auto j : i) {
			std::cout << j << ' ';
		}
		std::cout << '\n';
		k++;
	}
}