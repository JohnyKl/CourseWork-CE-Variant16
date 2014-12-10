#include "stdafx.h"
#include "Operator.h"

using namespace Poperechny_Kursova;

Operator::Operator(void)
{
}

Operator::Operator(string path)//����������� � ���������� ����� �� ����� � ������������
{
	prepareOperator(path);
}

void Operator::readCoordsFromFile(string path)
{
	std::ifstream ifs(path); //��������� ��������� ������ �����

    std::vector<int> coords;//��������� ��������� �������, � ��� �������������� ����������

    if(ifs.is_open())//���� �������� ����� �������
    {
        while (! ifs.eof() )//���� �� ��������� ���� �����
        {
            int value = 0;//���������� � ����������� �����

            ifs >> value;//����� � ����� �������� � �����. �������� >> ����� ����� �� ������� ���������� (������ � �.�.)

            coords.push_back(value);//��������� � ������ ��������� ��������
        }

        int pointsNumber = coords.size()/2;//������� ��������� ������������ �� �������� ������� ������� � �� � �������� �� 2

        Array ^points = Array::CreateInstance(Point::typeid, pointsNumber);//����������� ������ � ������ ���� Point

        for (int i = 0; i < pointsNumber*2; i+=2)
        {
            points->SetValue(Point(coords[i],coords[i+1]),i/2);//������ � ����� ����� ���� �������� ����� � ���� ���������� ������� ���� �������� �� �������
        }

		createRectangles(points);//�������� ������������
    }

    ifs.close();//�������� ��������� ������
}

void Operator::setRandomColorsTypes()//���������� ���������� ���� ��� ������� ������������
{
	int colorsCount = rectangles->Length;
	
	int red = 0;
	int blue = 0;
	int green = 0; 

	srand (time(NULL));

	for(int i = 0; i < colorsCount; i++)//��� ������� ������������ ������ �������� �������� ��� ������� ������ � ��������� ����� RGB
	{
		red = rand() % 256;
		blue = rand() % 256;
		green = rand() % 256;

		Color color = Color::FromArgb(red, green, blue);//�������� ��˲�
		((CustomRectangle^)rectangles->GetValue(i))->setColorType(color);//���������� ������������ ����� ����
	}
}

void Operator::prepareOperator(string path)//������� �� 䳿 ��� ��������� �� ����� ������������
{	
	readCoordsFromFile(path);//������� ���������� � �����
	setRandomColorsTypes();	//���������� �������� ������� ��� ������� ������������
	findAttachments();//������ ��������� ������������
	writeCoordsInFile();//�������� ���������� ������������, �� ������� � �������� ��������� ��������, � ����
}

void Operator::drawRectangles(Windows::Forms::PaintEventArgs^ e)//���������� �� ������������
{
	for(int i = 0; i < rectangles->Length; i++)
	{
		CustomRectangle ^rect = (CustomRectangle^)rectangles->GetValue(i);//�������� �-�� �����������
		rect->drawRectangle(e);//���������� ���� �������� ��������
	}
}

void Operator::createRectangles(Array ^points)//�������� ������������
{
	int length = points->Length;

	rectangles = Array::CreateInstance(CustomRectangle::typeid, length/4);//������� ������������ � 4 ���� ����� �� ������� ��� ������

	for(int i = 0; i < length; i += 4)
	{
		Point locationPoint = (Point) points->GetValue(i);//�������� ��� ������ �������
		Point sideA = (Point) points->GetValue(i + 1);//�������� ������� ���������� ������
		Point sideB = (Point) points->GetValue(i + 2);

		int width, height;//��������� ���� ������ �� ������ ������������

		if(locationPoint.X == sideA.X)//���� ���������� ��� �� �� ������, ���������� � �������� � ������, � � �������� � ������
		{
			height = Math::Abs(locationPoint.Y - sideA.Y);//������ = ������ ��������� �� �� �������
			width  = Math::Abs(locationPoint.X - sideB.X);//������ = ������ ��������� �� �� ������
		}
		else if(locationPoint.Y == sideA.Y)//���� ���������� ��� �� �� �������, ���������� ������ � �������� �, � ������ � �������� �
		{
			width  = Math::Abs(locationPoint.X - sideA.X);
			height = Math::Abs(locationPoint.Y - sideB.Y);
		}

		rectangles->SetValue(gcnew CustomRectangle(locationPoint.X, locationPoint.Y, width, height),i/4);//������ ����� ����������� � �����
	}
}

void Operator::findAttachments()//������ ��������� ������������
{	
	CustomRectangle ^startRect;//���������� ��� ��������� �����������

	for(int i = 0; i < rectangles->Length; i++)
	{
		startRect = (CustomRectangle^)rectangles->GetValue(i);//������������ ���������� �����������

		for(int j = i + 1; j < rectangles->Length; j++)
		{
			CustomRectangle ^nextRect = (CustomRectangle^)rectangles->GetValue(j);//��������� ��� ��������� �����������
			if(startRect->isAttachment(nextRect->getRectangle()))//���� ���� ����������� �������� � �����
			{
				nextRect->setColorType(startRect->getColorType());//���������� ������������ �������� ���� ������������, � ���� �� ���������
			}
		}
	}
	maxCount = getTypeOtTheBiggestAttachments();//������ ����, ���� �������� ��������� ��������� ��������� ������������
}

void Operator::writeCoordsInFile()//�������� �������� � ����
{
	ofstream ofs;

	ofs.open("coordsOfRectanglesWithMaxAttachments.txt");//������� �������� ���� ��� ������

	for(int i = 0; i < rectangles->Length; i++)//��� ������� ������������ � ����� ������������
	{
		CustomRectangle ^rect = (CustomRectangle^)rectangles->GetValue(i);//�������� �����������
		
		if(rect->getColorType().Equals(maxCount))//���� ���� ������������ ���������� � ��������, ���� �������� �������� ������� ��������
		{
			ofs << rect->stringCoords().c_str();//�������� ������ � ������������ ������ ������������ � ����
		}
	}
	ofs.close();//������� �������� ����
}

Color Operator::getTypeOtTheBiggestAttachments()//������ ���� �������� ��������� ��������
{
	Color maxColor = ((CustomRectangle^)rectangles->GetValue(0))->getColorType();//������������ ���� �������� ���������
	int maxCount = 0;//������������ �������� �������� ������� ��������

	for(int i = 0; i < rectangles->Length; i++)//����, �� ��� ������� �������� � ����� ������������ �� ��������� ���������� ����� �������� �������� ������� �������� ������ ��������� � ��������� ������� ��������� �������
	{
		int currentCount = 0;
		Color currentColor = ((CustomRectangle^)rectangles->GetValue(i))->getColorType();//���� ����������� ��� ���� �������� ��������

		for(int j = 0; j < rectangles->Length; j++)
		{
			if(((CustomRectangle^)rectangles->GetValue(j))->getColorType().Equals(currentColor))//�������� ���� ����� �������� � �������� ������� ������ �������� � �����
			{
				currentCount++;//�������� �������� ��� �������� �������
			}
		}

		if(maxCount < currentCount)//���� � ������� �������� ����� ������� �������� ����� �� ������� �����������
		{
			maxCount = currentCount;//������� ����������� �������� 
			maxColor = currentColor;//������� �������� �������� �������
		}
	}
	return maxColor;//��������� ���� �������� ��������� ��������
}