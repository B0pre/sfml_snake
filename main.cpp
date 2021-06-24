#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <list>

#define SLEEP_MS 100

#define BOARD_SIZE 50
#define SHAPE_SIZE 5.f

class Direction {
	public:
		int _x;
		int _y;
		Direction(int x, int y): _x(x), _y(y) {}
		bool operator==(const Direction& d)
		{
			return _x == d._x && _y == d._y;
		}
		bool operator!=(const Direction& d)
		{
			return !(*this == d);
		}
};

class Point {
	public:
		int _x;
		int _y;
		Point(int x, int y): _x(x), _y(y) {}
		void nextPoint(const Direction& direction)
		{
			_x+=direction._x;
			_y+=direction._y;
			if(_x < 0)
				_x = BOARD_SIZE - 1;
			if(_x >= BOARD_SIZE)
				_x = 0;
			if(_y < 0)
				_y = BOARD_SIZE - 1;
			if(_y >= BOARD_SIZE)
				_y = 0;
		}
		bool operator==(const Point& p)
		{
			return _x == p._x && _y == p._y;
		}
};


int main(int argc, char ** argv)
{
	std::srand(std::time(nullptr));
	sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * SHAPE_SIZE * 2, BOARD_SIZE * SHAPE_SIZE * 2), "Snake");

	Direction left(-1,0);
	Direction right(1,0);
	Direction down(0,1);
	Direction up(0,-1);
	Point noApple(-1, -1);
	Point start(2,3);

	Direction currentDirection = down;
	Point snakeHead = start;
	Point apple(-1, -1);

	std::list<Point> markedPoints;
	markedPoints.push_back(snakeHead);

	unsigned long long score = 0;

	while (window.isOpen())
	{
		window.clear();

		if(apple == noApple)
		{
			int x = std::rand()/((RAND_MAX -1u)/(BOARD_SIZE-1));
			int y = std::rand()/((RAND_MAX -1u)/(BOARD_SIZE-1));
			apple = Point(x,y);
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		bool turned = false;
		if(!turned && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			turned = true;
			if(currentDirection != right)
				currentDirection = left;
		}
		if(!turned && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			turned = true;
			if(currentDirection != left)
				currentDirection = right;
		}
		if(!turned && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			turned = true;
			if(currentDirection != down)
				currentDirection = up;
		}
		if(!turned && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			turned = true;
			if(currentDirection != up)
				currentDirection = down;
		}


		snakeHead.nextPoint(currentDirection);
		markedPoints.push_front(snakeHead);

		if(apple == snakeHead)
		{
			apple = noApple;
			++score;
		}
		else
			markedPoints.pop_back();


		int countCur = 0;

		for(Point p : markedPoints)
		{
			sf::CircleShape currentShape(SHAPE_SIZE);
			currentShape.setFillColor(sf::Color::White);
			if(p == snakeHead)
			{
				currentShape.setFillColor(sf::Color::Red);
				++countCur;
			}
			currentShape.setPosition(p._x * SHAPE_SIZE*2, p._y * SHAPE_SIZE*2);
			window.draw(currentShape);
		}

		//game over
		if(countCur > 1)
		{
			std::cout << "GAME OVER with score: " << score << std::endl;
			score = 0;
			snakeHead = start;
			markedPoints = std::list<Point>();
			markedPoints.push_front(snakeHead);
			currentDirection = down;
		}

		sf::CircleShape appleShape(SHAPE_SIZE);
		appleShape.setFillColor(sf::Color::Green);
		appleShape.setPosition(apple._x * SHAPE_SIZE*2, apple._y * SHAPE_SIZE*2);
		window.draw(appleShape);

		window.display();

		sf::sleep(sf::milliseconds(SLEEP_MS));
	}

	return 0;
}
