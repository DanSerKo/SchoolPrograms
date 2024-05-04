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
//#include <complex>
#include "windows.h"
#define endl '\n'

using ll = long long;
using ld = long double;
using namespace sf;
const ld INF = 1e9, w = 800, h = 800;
std::mt19937 rng(time(0));

int rnd(ld scale) {
	//return 0;
	return (rng() % 100) * scale;
}

void setPix(int w1, int h1, int w2, int h2, ld s, Image& a) {
	int x = (w2 + w1) / 2, y = (h2 + h1) / 2;
	if (a.getPixel(x, y) != Color(0, 0, 0)) {
		return;
	}
	std::vector <std::pair <int, int>> t = { {w1, h1}, {w1, h2}, {w2, h1}, {w2, h2} };
	int r = 0, g = 0, b = 0;
	for (auto& u : t) {
		r += a.getPixel(u.first, u.second).r;
		g += a.getPixel(u.first, u.second).g;
		b += a.getPixel(u.first, u.second).b;
	}
	r /= 4;
	g /= 4;
	b /= 4;

	r += rnd(s) * (-1 + (rng() % 2) * 2);
	g += rnd(s) * (-1 + (rng() % 2) * 2);
	b += rnd(s) * (-1 + (rng() % 2) * 2);
	if (r <= 0)
		r += 20;
	if (g <= 0)
		g += 20;
	if (b <= 0)
		b += 20;

	if (r >= 255)
		r -= 20;
	if (g >= 255)
		g -= 20;
	if (b >= 255)
		b -= 20;
	a.setPixel(x, y, Color(r, g, b));
}

void dfs(int w1, int h1, int w2, int h2, ld s, Image& a) {
	if (w1 + 1 >= w2 && h1 + 1 >= h2) {
		return;
	}
	int x = (w2 + w1) / 2, y = (h2 + h1) / 2;
	setPix(w1, h1, w2, h1, s, a);
	setPix(w1, h2, w2, h2, s, a);
	setPix(w1, h1, w1, h2, s, a);
	setPix(w2, h1, w2, h2, s, a);

	setPix(w1, h1, w2, h2, s, a);

	dfs(w1, h1, x, y, s * 0.6, a);
	dfs(x, y, w2, h2, s * 0.6, a);
	dfs(w1, y, x, h2, s * 0.6, a);
	dfs(x, h1, w2, y, s * 0.6, a); 
}

Image z;
Texture y;
Sprite x;
void create() {
	z.create(w + 1, h + 1, Color(0, 0, 0));
	z.setPixel(0, 0, Color(rng() % 256, rng() % 256, rng() % 256));
	z.setPixel(w, h, Color(rng() % 256, rng() % 256, rng() % 256));
	z.setPixel(w, 0, Color(rng() % 256, rng() % 256, rng() % 256));
	z.setPixel(0, h, Color(rng() % 256, rng() % 256, rng() % 256));
	dfs(0, 0, w, h, 1, z);
	y.loadFromImage(z);
	x.setTexture(y);
}

int main() {
	RenderWindow window(VideoMode(w, h), "...", Style::Titlebar | Style::Close);
	create();
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Enter)) {
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			create();
		}
		window.clear();
		window.draw(x);
		window.display();
	}
	return 0;
}