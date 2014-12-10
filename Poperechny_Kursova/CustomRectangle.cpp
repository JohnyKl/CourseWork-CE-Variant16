#include "stdafx.h"
#include "CustomRectangle.h"

using namespace Poperechny_Kursova;


CustomRectangle::CustomRectangle(void)
{
	rectangle = Rectangle();
	type = Color::Black;
}

CustomRectangle::CustomRectangle(int x, int y, int width, int height)//конструктор класу
{
	rectangle = Rectangle(x,y,width,height);

	type = Color::Chocolate;
}

void CustomRectangle::drawRectangle(System::Windows::Forms::PaintEventArgs^ e)//малює прямокутник
{
	e->Graphics->DrawRectangle(gcnew Pen(type, 4.0f), rectangle.X, rectangle.Y, rectangle.Width, rectangle.Height); 
}

string CustomRectangle::stringCoords()//формує стрічку з координатами вершин прямокутника
{
	int x = rectangle.X;
	int y = rectangle.Y;

	string vertexA = intToString(x) + " " + intToString(y);
	string vertexB = intToString(x) + " " + intToString(rectangle.Bottom);
	string vertexC = intToString(rectangle.Right) + " " + intToString(y);
	string vertexD = intToString(rectangle.Right) + " " + intToString(rectangle.Bottom);;

	string endLine = "\n";

	return vertexA + endLine + vertexB + endLine + vertexC + endLine + vertexD + endLine;
}

string CustomRectangle::intToString(int i)//конвертує int в std::string
{
	ostringstream s;
	s << i;

	return s.str();
}

Color CustomRectangle::getColorType()
{
	return type;
}

bool CustomRectangle::isAttachment(Rectangle rect)//перевіряє, чи область переданого в якості параметра прямокутника повністю знаходиться всередині області поточного прямокутника
{
	return rectangle.Contains(rect);
}

Rectangle CustomRectangle::getRectangle()
{
	return rectangle;
}

void CustomRectangle::setColorType(Color color)
{
	type = color;
}