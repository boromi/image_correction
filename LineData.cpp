#include "StdAfx.h"
#include "LineData.h"

LineData::LineData()
{
	this->width = 0;
	this->datas = 0;
}

LineData::LineData(int width)
{
	this->width = width;
	this->datas = new cpp::Color[width];
	for (int i=0;i<width;i++)
		this->datas[i] = cpp::Color(0,0,0);
}
