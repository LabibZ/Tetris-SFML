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

bool collision_check()
{
	for (int i = 0; i < 4; i++) {
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= height) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	}
	return 1;
}

void line_check()
{
	int k = height - 1;
	for (int i = height - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < width; j++) {
			if (field[i][j]) count++;
			field[k][j] = field[i][j];
		}
		if (count < width) k--;
	}
}

void move(int dx) 
{
	for (int i = 0; i < 4; i++) {
		b[i] = a[i];
		a[i].x += dx;
	}
	if (!collision_check()) {
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
	if (!collision_check()) {
		for (int i = 0; i < 4; i++) {
			a[i] = b[i];
		}
	}
}

int main() 
{
	srand(time(0));
	RenderWindow window(VideoMode(320, 480), "Tetris");
	
	Texture t1, t2, t3;
	t1.loadFromFile("include/images/tiles.png");
	t2.loadFromFile("include/images/background.png");
	t3.loadFromFile("include/images/frame.png");

	Sprite s(t1), background(t2), frame(t3);

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
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05f;

		move(dx);

		if (isRotate)
			rotate();
		// tick
		if (timer > delay)
		{
			// piece falls down
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!collision_check())
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		line_check();

		// reset
		dx = 0;
		isRotate = false;
		delay = 0.3;

		// draw
		window.clear(Color::White);
		window.draw(background);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}
		}

		for (int i = 0; i < 4; i++) {
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(28, 31); //offset
			window.draw(s);
		}
		window.draw(frame);
		window.display();
	}

	return 0;
}