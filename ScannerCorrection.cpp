#include "StdAfx.h"
#include "ScannerCorrection.h"

ScannerCorrection::ScannerCorrection() { }

// calculate the time difference between the given start and end date
int calculateDT(SYSTEMTIME time1, SYSTEMTIME time2)
{
	FILETIME ft1, ft2;
	SystemTimeToFileTime(&time1, &ft1);
	SystemTimeToFileTime(&time2, &ft2);
	return ft1.dwLowDateTime - ft2.dwLowDateTime;
}


// calculate the impact point of the ray with the given starting position and direction
Vector calculateGroundPosition(Vector position, Vector direction)
{
	double t = 0.0 - position.y / direction.y;
	return position + direction * t;
}

// rotate the (0, -1, 0) Vector with the given angles to get the ray direction
Vector calculateRayDirection(double angleX, double angleY)
{
	Vector direction(0, -1, 0);
	direction = Matrix::Rotate_X(angleX) * direction;
	direction = Matrix::Rotate_Y(angleY) * direction;
	return direction;
}

cpp::Color* ScannerCorrection::DoCorrection(
		int lines, 
		int outputWidth, int outputHeight,
		LineData* lineDatas,
		GpsData* gpsDatas,
		SensorData* sensorDatas)
{
	// return color buffer, fulfill with black color
	cpp::Color* colorBuffer = new cpp::Color[outputWidth * outputHeight];
	for (int i=0;i<outputWidth * outputHeight;i++)
	{
		colorBuffer[i] = cpp::Color(0.0, 0.0, 0.0);
	}

	// determine the width
	int width = lineDatas[0].width;

	// create storage for projection
	ProjectedLineData* pld = new ProjectedLineData[lines];
	for (int i=0;i<lines;i++)
	{
		pld[i] = ProjectedLineData(width);
	}

	// airPlane starting position (other position will be relative to this point (0,0,0))
	Vector airPlanePosition = Vector(0,0,0);

	// process every input line, calculate the line left and right corner, and do the projection
	for (int i=0;i<lines;i++)
	{
		if (i != 0)
		{
			double dt = calculateDT(gpsDatas[i-1].timestamp, gpsDatas[i].timestamp) / 10000000.0;
			airPlanePosition += Vector(gpsDatas[i].Vea, 0, gpsDatas[i].Vno) * dt;
		}

		double angleY = atan2(gpsDatas[i].Vno, gpsDatas[i].Vea);
		angleY *= 180.0 / M_PI;

		Vector leftDirection =	calculateRayDirection(30.0, angleY);
		Vector rightDirection = calculateRayDirection(-30.0, angleY);

		airPlanePosition.y = gpsDatas[i].alt;

		Vector leftGroundPosition =  calculateGroundPosition(airPlanePosition, leftDirection);
		Vector rightGroundPosition = calculateGroundPosition(airPlanePosition, rightDirection);

		pld[i].datas[0] = leftGroundPosition;
		pld[i].datas[width - 1] = rightGroundPosition;
	}

	double minX = pld[0].datas[0].x;
	double maxX = pld[0].datas[0].x;
	double minZ = pld[0].datas[0].z;
	double maxZ = pld[0].datas[0].z;

	// search the bounds of the projected lines
	for (int i=0;i<lines;i++)
	{
		if (pld[i].datas[0].x < minX)
			minX = pld[i].datas[0].x;
		if (pld[i].datas[width - 1].x < minX)
			minX = pld[i].datas[width - 1].x;
		if (pld[i].datas[0].x > maxX)
			maxX = pld[i].datas[0].x;
		if (pld[i].datas[width - 1].x > maxX)
			maxX = pld[i].datas[width - 1].x;
		if (pld[i].datas[0].z < minZ)
			minZ = pld[i].datas[0].z;
		if (pld[i].datas[width - 1].z < minZ)
			minZ = pld[i].datas[width - 1].z;
		if (pld[i].datas[0].z > maxZ)
			maxZ = pld[i].datas[0].z;
		if (pld[i].datas[width - 1].z > maxZ)
			maxZ = pld[i].datas[width - 1].z;
	}

	// create the output image
	for (int i=0;i<lines;i++)
	{
		// two corners point of a line (x0,y0 left, x1,y1 right)
		int x0 = (int)( (double)(pld[i].datas[0].x - minX) / (double)(maxX - minX) * (outputWidth - 1));
		int y0 = (int)( (double)(pld[i].datas[0].z - minZ) / (double)(maxZ - minZ) * (outputHeight - 1));

		int x1 = (int)( (double)(pld[i].datas[width - 1].x - minX) / (double)(maxX - minX) * (outputWidth - 1));
		int y1 = (int)( (double)(pld[i].datas[width - 1].z - minZ) / (double)(maxZ - minZ) * (outputHeight - 1));

		// write the line Color information to the return buffer with the calculated projection positions
		for (int j=0;j<width;j++)
		{
			int x = x0 + (int)((x1 - x0) * (double)j / (double)width);
			int y = y0 + (int)((y1 - y0) * (double)j / (double)width);

			colorBuffer[outputWidth * y + x] = lineDatas[i].datas[j];			
		}
	}

	return colorBuffer;
}