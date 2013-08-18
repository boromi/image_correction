#include "StdAfx.h"
#include "SensorData.h"

SensorData::SensorData()
{
	this->timestamp.wDay;
	this->timestamp.wDayOfWeek;
	this->timestamp.wHour;
	this->timestamp.wMilliseconds;
	this->timestamp.wMinute;
	this->timestamp.wMonth;
	this->timestamp.wSecond;
	this->timestamp.wYear;
	this->Qw = 0.0;
	this->Qx = 0.0;
	this->Qy = 0.0;
	this->Qz = 0.0;
	this->Ax = 0.0;
	this->Ay = 0.0;
	this->Az = 0.0;
	this->Mx = 0.0;
	this->My = 0.0;
	this->Mz = 0.0;
	this->Gx = 0.0;
	this->Gy = 0.0;
	this->Gz = 0.0;	
}

SensorData::SensorData(SYSTEMTIME timestamp,
		double Qw, double Qx, double Qy, double Qz,
		double Ax, double Ay, double Az,
		double Mx, double My, double Mz,
		double Gx, double Gy, double Gz)
{
	this->timestamp.wDay = timestamp.wDay;
	this->timestamp.wDayOfWeek = timestamp.wDayOfWeek;
	this->timestamp.wHour = timestamp.wHour;
	this->timestamp.wMilliseconds = timestamp.wMilliseconds;
	this->timestamp.wMinute = timestamp.wMinute;
	this->timestamp.wMonth = timestamp.wMonth;
	this->timestamp.wSecond = timestamp.wSecond;
	this->timestamp.wYear = timestamp.wYear;
	this->Qw = Qw;
	this->Qx = Qx;
	this->Qy = Qy;
	this->Qz = Qz;
	this->Ax = Ax;
	this->Ay = Ay;
	this->Az = Az;
	this->Mx = Mx;
	this->My = My;
	this->Mz = Mz;
	this->Gx = Gx;
	this->Gy = Gy;
	this->Gz = Gz;	
}

SensorData::SensorData(const SensorData& x)
{
	this->timestamp.wDay = x.timestamp.wDay;
	this->timestamp.wDayOfWeek = x.timestamp.wDayOfWeek;
	this->timestamp.wHour = x.timestamp.wHour;
	this->timestamp.wMilliseconds = x.timestamp.wMilliseconds;
	this->timestamp.wMinute = x.timestamp.wMinute;
	this->timestamp.wMonth = x.timestamp.wMonth;
	this->timestamp.wSecond = x.timestamp.wSecond;
	this->timestamp.wYear = x.timestamp.wYear;
	this->Qw = x.Qw;
	this->Qx = x.Qx;
	this->Qy = x.Qy;
	this->Qz = x.Qz;
	this->Ax = x.Ax;
	this->Ay = x.Ay;
	this->Az = x.Az;
	this->Mx = x.Mx;
	this->My = x.My;
	this->Mz = x.Mz;
	this->Gx = x.Gx;
	this->Gy = x.Gy;
	this->Gz = x.Gz;	
}

SensorData* SensorData::readFromFile(std::string fileName, int& readedRecord )
{
	// determine the number of records

	readedRecord = 0;

	ifstream ifs(fileName.c_str());
	string line;
	while(!ifs.eof())
	{	
		getline (ifs,line);

		if (line.length() > 1)
		{
			if (line[0] != '#')
			{
				readedRecord++;
			}
		}
	}

	ifs.close();

	SensorData* ret = new SensorData[readedRecord];
	readedRecord = 0;

	// create the instances, and fill with data

	ifstream ifs2(fileName.c_str());

	while(!ifs2.eof())
	{	
		getline (ifs2,line);

		if (line.length() > 1)
		{
			if (line[0] != '#')
			{
				SensorData sd = SensorData::readFromLine(line);
				ret[readedRecord] = sd;
				readedRecord++;
			}
		}
	}

	ifs2.close();

	return ret;
}


SensorData SensorData::readFromLine(string line)
{
	SensorData* sd = new SensorData();

	stringstream sline(line);

	string timestampstring = "";
	sline>>timestampstring;

	FILETIME ft;

    stringstream ss;
	string a = timestampstring.substr(0, 8);
	ss << std::hex << timestampstring.substr(0, 8);
	int tup;
	ss >> tup;

	string b = timestampstring.substr(8, 8);
    ss << std::hex << timestampstring.substr(8, 8);
	int tdown;
    ss >> tdown;


	ft.dwLowDateTime = tdown;
	ft.dwHighDateTime = tup;

	FileTimeToSystemTime(&ft, &sd->timestamp);

	sline>>sd->Qw;
	sline>>sd->Qx;
	sline>>sd->Qy;
	sline>>sd->Qz;
	sline>>sd->Ax;
	sline>>sd->Ay;
	sline>>sd->Az;
	sline>>sd->Mx;
	sline>>sd->My;
	sline>>sd->Mz;
	sline>>sd->Gx;
	sline>>sd->Gy;
	sline>>sd->Gz;	

	return *sd;
	
}

