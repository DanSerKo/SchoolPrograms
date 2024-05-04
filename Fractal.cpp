#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <iomanip>
#include <deque>
#include <iterator>
#include <string>
#include <queue>
#include <list>
#include <cassert>
#include "windows.h"
#define endl '\n'

using ll = long long;
using coor = long double;

using namespace sf;

struct vec {
	coor x, y;
};

vec operator+ (vec a, vec b) {
	return { a.x + b.x, a.y + b.y };
}
vec operator- (vec a, vec b) {
	return { a.x - b.x, a.y - b.y };
}
vec operator* (vec a, coor b) {
	return { a.x * b, a.y * b };
}
coor operator* (vec a, vec b) {
	return a.x * b.x + a.y * b.y;
}
coor operator% (vec a, vec b) {
	return a.x * b.y - a.y * b.x;
}
vec operator+= (vec& a, vec b) {
	return a = a + b;
}
vec operator-= (vec& a, vec b) {
	return a = a - b;
}
std::istream& operator >> (std::istream& cin, vec& a) {
	return cin >> a.x >> a.y;
}
std::ostream& operator << (std::ostream& cout, vec& a) {
	return cout << a.x << ' ' << a.y;
}
coor len2(vec a) {
	return a * a;
}
long double len(vec a) {
	return sqrt(a * a);
}
vec rev(vec a) {
	return { a.y, -a.x };
}

struct Myline {
	vec p0, p1;
	Vertex points[2];
	Myline (vec a, vec b){
		points[0] = Vertex(Vector2f(a.x, a.y));
		points[1] = Vertex(Vector2f(b.x, b.y));
		p0 = { a.x, a.y };
		p1 = { b.x, b.y };
	}
	Myline() {
		points[0] = Vertex(Vector2f(0, 0));
		points[1] = Vertex(Vector2f(0, 0));
		p0 = { 0, 0 };
		p1 = { 0, 0 };
	}
};

struct Star {
	std::vector <Myline> lines;
	void build(Star& a) {
		for (auto& r : a.lines) {
			vec P1 = (r.p1 - r.p0) * (1.0 / len(r.p1 - r.p0)) * (len(r.p1 - r.p0) / 3) + r.p0;
			vec P2 = (r.p1 - r.p0) * (1.0 / len(r.p1 - r.p0)) * (len(r.p1 - r.p0) * 2 / 3) + r.p0;
			vec P3 = (P2 - P1) * (long double)0.5 + P1;
			vec norm = rev(P3 - P1) * (1.0 / len(P3 - P1)) * sqrt(len2(r.p1 - r.p0) / 9 - len2(r.p1 - r.p0) / 36);

			P3 += norm;
			
			lines.push_back(*new Myline(r.p0, P1));
			lines.push_back(*new Myline(P2, r.p1));

			lines.push_back(*new Myline(P1, P3));
			lines.push_back(*new Myline(P3, P2));
		}
	}
};

Font font;
struct buttom {
	RectangleShape a;
	Text t;
	buttom(std::string s) {
		a.setSize(Vector2f(120, 50));
		t.setFont(font);
		t.setString(s);
		t.setCharacterSize(24);
		t.setColor(Color::Color(128, 128, 128));
		t.setFillColor(Color::White);
	}
	buttom() {
		a.setSize(Vector2f(120, 50));
		t.setFont(font);
		t.setString("???");
		t.setCharacterSize(24);
		t.setColor(Color::Color(128, 128, 128));
		t.setFillColor(Color::White);
	}
};

int main()
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "error";
		return 0;
	}
	RenderWindow window(VideoMode(1920, 1080), "Star", Style::Fullscreen);
	//RectangleShape line(sf::Vector2f(150, 5));
	//line.rotate(30);
	std::vector <Star> a(11);
	a[0].lines.resize(3);
	a[0].lines[0] = { {1600 - 600, 800 - 600 }, {2000 - 600, 1400 - 600} };
	a[0].lines[1] = { {2000 - 600, 1400 - 600}, {1200 - 600, 1400 - 600} };
	a[0].lines[2] = { {1200 - 600, 1400 - 600}, {1600 - 600, 800 - 600} };
	int q = 0;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (q > 0) {
				q--;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (q < 8) {
				q++;
				a[q].build(a[q - 1]);
			}
		}

		window.clear();
		for (auto& r : a[q].lines) {
			window.draw(r.points, 2, Lines);
		}
		window.display();
		Sleep(60);
	}

	return 0;
}