#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int width = 10;
const int height = 20;

int field[height][width] = { 0 };

struct Point {
	int x, y;
} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i < 4; i++) {
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= height) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	}
	return 1;
}

void move(int dx) 
{
	for (int i = 0; i < 4; i++) {
		b[i] = a[i];
		a[i].x += dx;
	}
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			a[i] = b[i];
		}
	}
}

void rotate() 
{
	Point p = a[1]; // center of rotation
	for (int i = 0; i < 4; i++) {
		int x = a[i].y - p.y;
		int y = a[i].x - p.x;
		a[i].x = p.x - x;
		a[i].y = p.y + y;
	}
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			a[i] = b[i];
		}
	}
}

int main() 
{
	srand(time(0));
	RenderWindow window(VideoMode(320, 480), "Tetris");
	
	Texture t;
	t.loadFromFile("include/images/tiles.png");

	Sprite s(t);
	s.setTextureRect(IntRect(0, 0, 18, 18));

	int dx = 0;
	bool isRotate = false;
	int colorNum = 1;
	float timer = 0.0f;
	float delay = 0.3f;

	Clock clock;

	while (window.isOpen()) 
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Up) isRotate = true;
				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;
			}
		}
		// move
		move(dx);
		// rotate
		if (isRotate)
			rotate();
		// tick
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}
			timer = 0;
		}

		int n = 3; // figure
		if (a[0].x==0)
		for (int i = 0; i < 4; i++) {
			a[i].x = figures[n][i] % 2;
			a[i].y = figures[n][i] / 2;
		}

		dx = 0;
		isRotate = false;


		window.clear(Color::White);
		for (int i = 0; i < 4; i++) {
			s.setPosition(a[i].x * 18, a[i].y * 18);
			window.draw(s);
		}
		window.display();
	}

	return 0;
}