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
const ld INF = 1e9, w = 1280, h = 720;

template <typename T> class complex {
private:
	T r, i;
public:
	complex() {
		r = i = 0;
	}
	complex(T R, T I) {
		r = R;
		i = I;
	}
	complex operator+ (complex <T> a) {
		return { a.r + r, a.i + i };
	}
	complex operator* (complex <T> a) {
		return { a.r * r - a.i * i, a.r * i + a.i * r };
	}
	complex operator- (complex <T> a) {
		return { r - a.r, i - a.i };
	}
	T real() {
		return r;
	}
	T imag() {
		return i;
	}
};

std::vector <Color> x = {
Color(255, 241, 14),
Color(255, 233, 0),
Color(255, 224, 0),
Color(255, 215, 0),
Color(255, 206, 0),
Color(255, 198, 0),
Color(255, 189, 0),
Color(255, 180, 0),
Color(255, 170, 0),
Color(255, 161, 5),
Color(255, 152, 18),
Color(255, 142, 28),
Color(255, 132, 35),
Color(255, 123, 42),
Color(255, 113, 49),
Color(255, 103, 54),
Color(255, 92, 60),
Color(255, 81, 66),
Color(255, 70, 71),
Color(255, 58, 76),
Color(255, 45, 81),
Color(255, 28, 86),
Color(255, 0, 91),
Color(250, 0, 96),
Color(244, 0, 100),
Color(237, 0, 105),
Color(230, 0, 110),
Color(223, 0, 114),
Color(214, 0, 118),
Color(206, 0, 122),
Color(196, 0, 126),
Color(187, 0, 130),
Color(176, 0, 133),
Color(166, 0, 136),
Color(154, 0, 139),
Color(142, 0, 142),
Color(129, 0, 144),
Color(115, 0, 146),
Color(100, 0, 147),
Color(84, 0, 148),
};

Color f(int k) {
	return x[(x.size() - k + x.size()) % x.size()];
}

Image build(ld cw, ld ch, ld scale) {
	Image q;
	q.create(w, h);
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			complex <ld> a = { 0, 0 };
			complex <ld> c = { (ld)(i) / scale + cw, (ld)(j) / scale + ch };
			q.setPixel(i, j, Color::Black);
			for (int t = 0; t < 300; t++) {
				a = a * a + c;
				if (a.real() * a.real() + a.imag() * a.imag() > 4 * 4) {
					q.setPixel(i, j, f(t));
					break;
				}
			}
		}
	}
	return q;
}

int main() {
	RenderWindow window(VideoMode(w, h), "...", Style::Titlebar | Style::Close);
	Texture y;
	ld scale = 200, W = -(ld)w / 2 / 200, H = -(ld)h / 2 / 200;
	y.loadFromImage(build(W, H, scale));
	Sprite x;
	x.setTexture(y);
	RectangleShape r(Vector2f(120, 50));
	r.setFillColor(Color(255, 255, 255, 300));
	int f = 0, Px, Py;
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		window.clear();
		window.draw(x);
		if (Mouse::isButtonPressed(Mouse::Left)) {
			auto p = window.getPosition();
			int px = Mouse::getPosition().x - p.x - 10;
			int py = Mouse::getPosition().y - p.y - 40;
			if (!f) {
				r.setPosition(px, py);
				f = 1;
				Px = px;
				Py = py;
			}
			else {
				r.setSize(Vector2f(px - Px, py - Py));
			}
			window.draw(r);
		}
		else {
			if (f && r.getSize().x * r.getSize().y != 0) {
				if (0 > r.getSize().x) {
					r.setPosition(r.getPosition().x + r.getSize().x, r.getPosition().y);
					r.setSize(Vector2f(-r.getSize().x, r.getSize().y));
				}
				if (0 > r.getSize().y) { 
					r.setPosition(r.getPosition().x, r.getPosition().y + r.getSize().y);
					r.setSize(Vector2f(r.getSize().x, -r.getSize().y));
				}
				//std::cout << r.getPosition().x << ' ' << r.getPosition().y << endl << r.getSize().x << ' ' << r.getSize().y << endl << endl;
				W = r.getPosition().x / scale + W;
				H = r.getPosition().y / scale + H;
				scale *= std::min(w / r.getSize().x, h / r.getSize().y);
				y.loadFromImage(build(W, H, scale));
				x.setTexture(y);
			}
			f = 0;
		}
		window.display();
	}
	return 0;
}