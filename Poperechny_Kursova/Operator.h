#pragma once

#include "stdafx.h"
#include "CustomRectangle.h"

using namespace std;
using namespace System::Drawing;
using namespace System;

namespace Poperechny_Kursova
{	
	ref class Operator
	{
	private:
		Array ^rectangles;
		Color ^maxCount;		

		void setRandomColorsTypes();
		void readCoordsFromFile(string path);
		void createRectangles(Array ^points);
		void findAttachments();
		void writeCoordsInFile();
		Color getTypeOtTheBiggestAttachments();

		void prepareOperator(string path);
	public:
		Operator(void);
		Operator(string path);
		
		void drawRectangles(Windows::Forms::PaintEventArgs^ e);
	};
}
