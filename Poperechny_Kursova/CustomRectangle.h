#pragma once

#include "stdafx.h"

using namespace System::Drawing;
using namespace std;

namespace Poperechny_Kursova
{
	ref class CustomRectangle
	{
		Rectangle rectangle;
		Color type;

	public:
		CustomRectangle(void);
		CustomRectangle(int a, int b, int c, int d);

		Rectangle getRectangle();
		Color getColorType();
		string stringCoords();
		void drawRectangle(System::Windows::Forms::PaintEventArgs^ e);
		bool isAttachment(Rectangle rect);
		void setColorType(Color color);
		string intToString(int i);
	};
}
