#include "StdAfx.h"
#include "ProjectedLineData.h"

ProjectedLineData::ProjectedLineData()
{
	this->width = 0;
	this->datas = 0;
}

ProjectedLineData::ProjectedLineData(int width)
{
	this->width = width;
	this->datas = new Vector[width];
	for (int i=0;i<width;i++)
		this->datas[i] = Vector(0,0,0);
}