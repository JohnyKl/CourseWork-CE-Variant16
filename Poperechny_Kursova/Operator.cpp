#include "stdafx.h"
#include "Operator.h"

using namespace Poperechny_Kursova;

Operator::Operator(void)
{
}

Operator::Operator(string path)//конструктор з параметром шляху до файлу з координатами
{
	prepareOperator(path);
}

void Operator::readCoordsFromFile(string path)
{
	std::ifstream ifs(path); //створення файлового потоку вводу

    std::vector<int> coords;//створення структури вектору, у яку додаватимуться координати

    if(ifs.is_open())//якщо файловий опток відкрито
    {
        while (! ifs.eof() )//доки не досягнуто кінця файлу
        {
            int value = 0;//оголошення і ініціалізація змінної

            ifs >> value;//запис у змінну значення з файлу. Оператор >> зчитує число до першого розділювача (пробілу і т.д.)

            coords.push_back(value);//додавання у вектор зчитаного значення
        }

        int pointsNumber = coords.size()/2;//кількість координат обчислюється як загальна кількість значень Х та У розділена на 2

        Array ^points = Array::CreateInstance(Point::typeid, pointsNumber);//ініціалізація масиву з даними типу Point

        for (int i = 0; i < pointsNumber*2; i+=2)
        {
            points->SetValue(Point(coords[i],coords[i+1]),i/2);//додати у масив точок нову створену точку з двох послідовних значень осей ординати та абсциси
        }

		createRectangles(points);//створити прямокутники
    }

    ifs.close();//закриття файлового потоку
}

void Operator::setRandomColorsTypes()//встановити випадковий колір для кожного прямокутника
{
	int colorsCount = rectangles->Length;
	
	int red = 0;
	int blue = 0;
	int green = 0; 

	srand (time(NULL));

	for(int i = 0; i < colorsCount; i++)//для кожного прямокутника обрати випадкові значення для кожного каналу в кольоровій моделі RGB
	{
		red = rand() % 256;
		blue = rand() % 256;
		green = rand() % 256;

		Color color = Color::FromArgb(red, green, blue);//СТВОРИТИ КОЛІР
		((CustomRectangle^)rectangles->GetValue(i))->setColorType(color);//встановити прямокутнику новий колір
	}
}

void Operator::prepareOperator(string path)//виконаті усі дії для виведення на екран прямокутників
{	
	readCoordsFromFile(path);//зчитати координати з файлу
	setRandomColorsTypes();	//встановити випадкові кольори для кожного прямокутника
	findAttachments();//знайти вкладення прямокутників
	writeCoordsInFile();//записати координати прямокутників, які входять у найбільшу сукупність вкладень, у файл
}

void Operator::drawRectangles(Windows::Forms::PaintEventArgs^ e)//намалювати всі прямокутники
{
	for(int i = 0; i < rectangles->Length; i++)
	{
		CustomRectangle ^rect = (CustomRectangle^)rectangles->GetValue(i);//отримати і-ий прямокутник
		rect->drawRectangle(e);//намалювати його потрібним кольором
	}
}

void Operator::createRectangles(Array ^points)//створити прямокутники
{
	int length = points->Length;

	rectangles = Array::CreateInstance(CustomRectangle::typeid, length/4);//кількість прямокутників в 4 рази менша за кількість всіх вершин

	for(int i = 0; i < length; i += 4)
	{
		Point locationPoint = (Point) points->GetValue(i);//отримати ліву верхню вершину
		Point sideA = (Point) points->GetValue(i + 1);//отримати наступні координати вершин
		Point sideB = (Point) points->GetValue(i + 2);

		int width, height;//оголосити змінні висоти та ширини прямокутника

		if(locationPoint.X == sideA.X)//якщо координати рівні по вісі абсцис, обрахувати з вершиною А висоту, а з вершиною В ширину
		{
			height = Math::Abs(locationPoint.Y - sideA.Y);//висота = різниця координат по вісі ординат
			width  = Math::Abs(locationPoint.X - sideB.X);//ширина = різниця координат по вісі абсцис
		}
		else if(locationPoint.Y == sideA.Y)//якщо координати рівні по вісі ординат, обрахувати висоту з вершиною В, а ширину з вершиною А
		{
			width  = Math::Abs(locationPoint.X - sideA.X);
			height = Math::Abs(locationPoint.Y - sideB.Y);
		}

		rectangles->SetValue(gcnew CustomRectangle(locationPoint.X, locationPoint.Y, width, height),i/4);//додати новий прямокутник в масив
	}
}

void Operator::findAttachments()//знайти вкладення прямокутників
{	
	CustomRectangle ^startRect;//початковий для порівняння прямокутник

	for(int i = 0; i < rectangles->Length; i++)
	{
		startRect = (CustomRectangle^)rectangles->GetValue(i);//ініціалізувати початковий прямокутник

		for(int j = i + 1; j < rectangles->Length; j++)
		{
			CustomRectangle ^nextRect = (CustomRectangle^)rectangles->GetValue(j);//наступний для порівняння прямокутник
			if(startRect->isAttachment(nextRect->getRectangle()))//якщо один прямокутник вкладенй у інший
			{
				nextRect->setColorType(startRect->getColorType());//вкладеному прямокутнику присвоїти колір прямокутника, в який він вкладений
			}
		}
	}
	maxCount = getTypeOtTheBiggestAttachments();//знайти колір, який присвоєно найбільшій сукупності вкладених прямокутників
}

void Operator::writeCoordsInFile()//записати координа у файл
{
	ofstream ofs;

	ofs.open("coordsOfRectanglesWithMaxAttachments.txt");//відкрити файловий потік для запису

	for(int i = 0; i < rectangles->Length; i++)//для кожного прямокутника в масиві прямокутників
	{
		CustomRectangle ^rect = (CustomRectangle^)rectangles->GetValue(i);//отримати прямокутник
		
		if(rect->getColorType().Equals(maxCount))//якщо колір прямокутника ідентичний з кольором, яким познаено найбільша кількість вкладень
		{
			ofs << rect->stringCoords().c_str();//записати стрічку з координатами вершин прямокутника у файл
		}
	}
	ofs.close();//закрити файловий потік
}

Color Operator::getTypeOtTheBiggestAttachments()//знайти колір найбільшої сукупності вкладень
{
	Color maxColor = ((CustomRectangle^)rectangles->GetValue(0))->getColorType();//ініціалізувати колір найбільшої сукупності
	int maxCount = 0;//ініціалізувати лічильник найбільшої кількості вкладень

	for(int i = 0; i < rectangles->Length; i++)//цикл, що для кожного елемента в масиві прямокутників за допомогою вкладеного циклу підраховує найбільшу кількість вкладень шляхом порівняння і підрахунку кількості повторень кольору
	{
		int currentCount = 0;
		Color currentColor = ((CustomRectangle^)rectangles->GetValue(i))->getColorType();//колір початкового для даної ітерації елемента

		for(int j = 0; j < rectangles->Length; j++)
		{
			if(((CustomRectangle^)rectangles->GetValue(j))->getColorType().Equals(currentColor))//порівняти колір цього елемента з кольором кожного іншого елемента в масиві
			{
				currentCount++;//збільшити лічильник при співпадінні кольорів
			}
		}

		if(maxCount < currentCount)//якщо в поточній ітерації циклу кількість вкладень більша за поточну максимальну
		{
			maxCount = currentCount;//оновити максимальне значення 
			maxColor = currentColor;//оновити відповідне значення кольору
		}
	}
	return maxColor;//повернути колір найбільшої сукупності вкладень
}