#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>


using namespace sf;
using namespace std;

Time Timer;

//Предназначен для объектов игрового мира в видимой области экрана, 
//камера может менять положение в игровом мире
//расчитывает координаты игрового мира относительно экрана
//координаты объекта в экране корректируются на глубину создавая перспективу
//является интерфейсом к очереди системных событий (Event)
//является источником времени для игрового мира 

class Camera: public RenderWindow
{
public:
	Clock clock;
	View view;
	Camera(float x, float y, VideoMode mode, const String& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings()) :RenderWindow(mode, title, style, settings)
	{
		RECT R;
		GetClientRect(this->getSystemHandle(), &R);
		view.reset({0, 0, (float)R.right, (float)R.bottom });
		setView(view);
		Focus = { x, y, (float)0.75 };
		setVerticalSyncEnabled(true);
	};
	Vector3<float> Focus;
	void virtual MoveBy(float dx, float dy);
	void virtual SetFocus(float x, float y);
	void virtual SetCadr(float x, float y);
	void Take(float x, float y, float z, Sprite picture);
	void virtual DoEvent(Event event);
};

//объект предназначен для отображения состояния объектов из текстур
//может отображать текстуру как:
//простой рисунок, 
//последовательность кадров,
//бесконечное повторенее текстуры
//для вывода на экран используется класс камера

class Clip : public Sprite
{	
	
	int countOfColums;
	int countOfRows;
	float currentFrame = 0;
public:	
	int countOfFrames;
	int sizeX;
	int sizeY;
	float duration = 1;
	bool visieble = true;
	float steps = 0;
	bool isPanorama = false;
	bool cycle = true;
	bool Revert = false;
	Clip(Texture &texture, int countOfFrames = 1, int  countOfColums = 1, bool isPanorama = false, float steps = 2, float duration = 1.0, bool cycle = true);
	void virtual Draw(Camera &screen, float x, float y, float z, bool left = false);
	void SetFrame(int numb);
	float virtual getSpeed();
	int virtual getFrame();
};

//-----------------------------------------------------------------------------------------------------------------------

void Clip::SetFrame(int numb)
{
	numb = numb % countOfFrames;
	currentFrame = numb;
}

float Clip::getSpeed()
{
	return sizeX * steps / duration;
}

void Camera::MoveBy(float dx, float dy)
{
	Focus.x += dx;
	Focus.y += dy;
}

void Camera::SetFocus(float x, float y)
{
	Focus.x = x;
	Focus.y = y;
}

void Camera::SetCadr(float x, float y)
{
	float sizeX = view.getSize().x/3;
	float sizeY = view.getSize().y/3;
	Focus.x = x - min(max(x - Focus.x, -sizeX), sizeX);
	Focus.y = y - min(max(y - Focus.y, -sizeY), sizeY);
}

//отображает клип с учетом координат этого клипа и камеры в игровом мире
//проводит необходимое масштабирование для эффекта перспективы

void Camera::Take(float x, float y, float z, Sprite picture)
{
	if (z != 0)
	{
		float dx = (x - Focus.x);
		float dy = Focus.y - y;
		dx *= pow(Focus.z, z);
		dy *= pow(Focus.z, z);
		picture.setPosition(dx + view.getSize().x / 2, dy + view.getSize().y);
	} else 	picture.setPosition(x, y);
	draw(picture);
}

void Camera::DoEvent(Event event)
{
	if (event.type == sf::Event::Resized)
	{
		sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
		this->setView(sf::View(visibleArea));
	}
}


Clip::Clip(Texture &texture, int countOfFrames, int countOfColums, bool isPanorama, float steps, float duration, bool cycle)
{
	setTexture(texture);
	setOrigin(texture.getSize().x / 2, texture.getSize().y);
	this->countOfRows = countOfRows;
	this->countOfRows = ceil((float)countOfFrames / (float) countOfColums);
	this->countOfColums = countOfColums;
	this->countOfFrames = countOfFrames;
	sizeY = texture.getSize().y / countOfRows;
	sizeX = texture.getSize().x / countOfColums;
	setOrigin(sizeX / 2, sizeY);
	isPanorama = isPanorama;
	this->steps = steps;
	this->duration = duration;
	this->cycle = cycle;
};

int Clip::getFrame()
{
	return (int)currentFrame;
}

//вычисляет текущий кадр
// при необходимости поворачивает вправо или в лево
//выделяет прямоугольник текущего кадра в текстуре 
//если необходимо тиражирует изображение горизонтально делая панораму
//корректирует координаты x и y c учетом глубины (z)

void Clip::Draw(Camera &screen, float x, float y, float z, bool left)
{	
	if (!visieble) return;
	float dFrame = 0;
	if(duration != 0) dFrame = (countOfFrames/duration) * Timer.asSeconds();
	currentFrame += dFrame;
	if (currentFrame >= countOfFrames) 
		if(cycle) currentFrame = 0;
		else currentFrame = countOfFrames - 1;
	int frame = int(currentFrame);
	int Col = frame % countOfColums;
	int Row = frame / countOfColums;
	int X = sizeX * Col;
	int Y = sizeY * Row;
	float dir = 1;
	if (left) dir = - dir;
	if (Revert) dir = -dir;
	if (dir < 0) X = X + sizeX;
	setTextureRect(IntRect(X, Y, sizeX*dir, sizeY));
	if(isPanorama) 
	{
		float k = pow(screen.Focus.z, z);
		float fx = (screen.Focus.x)*k;
		float S = sizeX / 2;
		float dx = abs(fx) - long(abs(fx) / (S))*(S);
		if (fx < 0) dx = S - dx;
		x = (fx - dx + S / 2) / k;
	}
	screen.Take(x, y, z, *this);
}

