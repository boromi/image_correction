#include "StdAfx.h"
#include "GpsData.h"

GpsData::GpsData()
{
	this->timestamp.wDay;
	this->timestamp.wDayOfWeek;
	this->timestamp.wHour;
	this->timestamp.wMilliseconds;
	this->timestamp.wMinute;
	this->timestamp.wMonth;
	this->timestamp.wSecond;
	this->timestamp.wYear;
	this->alt = 0.0;
	this->epe = 0.0;
	this->eph = 0.0;
	this->epv = 0.0;
	this->fix = 0;
	this->tow = 0.0;
	this->lat = 0.0;
	this->lon = 0.0;
	this->Vea = 0.0;
	this->Vno = 0.0;
	this->Vup = 0.0;
	this->msl_h = 0.0;
	this->leap = 0;
	this->wn_days = 0;
}

GpsData::GpsData(SYSTEMTIME timestamp,
		double alt,
		double epe,
		double eph,
		double epv,
		int fix,
		double tow,
		double lat,
		double lon,
		double Vea,
		double Vno,
		double Vup,
		double msl_h,
		int leap,
		int wn_days)
{
	this->timestamp.wDay = timestamp.wDay;
	this->timestamp.wDayOfWeek = timestamp.wDayOfWeek;
	this->timestamp.wHour = timestamp.wHour;
	this->timestamp.wMilliseconds = timestamp.wMilliseconds;
	this->timestamp.wMinute = timestamp.wMinute;
	this->timestamp.wMonth = timestamp.wMonth;
	this->timestamp.wSecond = timestamp.wSecond;
	this->timestamp.wYear = timestamp.wYear;
	this->alt = alt;
	this->epe = epe;
	this->eph = eph;
	this->epv = epv;
	this->fix = fix;
	this->tow = tow;
	this->lat = lat;
	this->lon = lon;
	this->Vea = Vea;
	this->Vno = Vno;
	this->Vup = Vup;
	this->msl_h = msl_h;
	this->leap = leap;
	this->wn_days = wn_days;

}

GpsData::GpsData(const GpsData& x)
{
	this->timestamp.wDay = x.timestamp.wDay;
	this->timestamp.wDayOfWeek = x.timestamp.wDayOfWeek;
	this->timestamp.wHour = x.timestamp.wHour;
	this->timestamp.wMilliseconds = x.timestamp.wMilliseconds;
	this->timestamp.wMinute = x.timestamp.wMinute;
	this->timestamp.wMonth = x.timestamp.wMonth;
	this->timestamp.wSecond = x.timestamp.wSecond;
	this->timestamp.wYear = x.timestamp.wYear;
	this->alt = x.alt;
	this->epe = x.epe;
	this->eph = x.eph;
	this->epv = x.epv;
	this->fix = x.fix;
	this->tow = x.tow;
	this->lat = x.lat;
	this->lon = x.lon;
	this->Vea = x.Vea;
	this->Vno = x.Vno;
	this->Vup = x.Vup;
	this->msl_h = x.msl_h;
	this->leap = x.leap;
	this->wn_days = x.wn_days;
}

GpsData* GpsData::readFromFile(std::string fileName, int& readedRecord )
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

	GpsData* ret = new GpsData[readedRecord];
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
				GpsData sd = GpsData::readFromLine(line);
				ret[readedRecord] = sd;
				readedRecord++;
			}
		}
	}

	ifs2.close();

	return ret;
}

int hexToDec(string input)
{
	int ret = 0;
	int p = 1;
	for (int i=0;i<input.length();i++)
	{
		int n = 0;
		if (input.at(input.length() - 1 - i) == '1')
			n = 1;
		else if (input.at(input.length() - 1 - i) == '2')
			n = 2;
		else if (input.at(input.length() - 1 - i) == '3')
			n = 3;
		else if (input.at(input.length() - 1 - i) == '4')
			n = 4;
		else if (input.at(input.length() - 1 - i) == '5')
			n = 5;
		else if (input.at(input.length() - 1 - i) == '6')
			n = 6;
		else if (input.at(input.length() - 1 - i) == '7')
			n = 7;
		else if (input.at(input.length() - 1 - i) == '8')
			n = 8;
		else if (input.at(input.length() - 1 - i) == '9')
			n = 9;
		else if (input.at(input.length() - 1 - i) == 'A')
			n = 10;
		else if (input.at(input.length() - 1 - i) == 'B')
			n = 11;
		else if (input.at(input.length() - 1 - i) == 'C')
			n = 12;
		else if (input.at(input.length() - 1 - i) == 'D')
			n = 13;
		else if (input.at(input.length() - 1 - i) == 'E')
			n = 14;
		else if (input.at(input.length() - 1 - i) == 'F')
			n = 15;

		ret += p * n;
		p *= 16;
	}
	return ret;
}


GpsData GpsData::readFromLine(string line)
{
	GpsData* gd = new GpsData();

	stringstream sline(line);

	string timestampstring = "";
	sline>>timestampstring;

	FILETIME ft;

	int tup = hexToDec(timestampstring.substr(0, 8));
	int tdown = hexToDec(timestampstring.substr(8, 8));;

	ft.dwLowDateTime = tdown;
	ft.dwHighDateTime = tup;

	FileTimeToSystemTime(&ft, &gd->timestamp);
	sline>>gd->alt;
	sline>>gd->epe;
	sline>>gd->eph;
	sline>>gd->epv;
	sline>>gd->fix;
	sline>>gd->tow;
	sline>>gd->lat;
	sline>>gd->lon;
	sline>>gd->Vea;
	sline>>gd->Vno;
	sline>>gd->Vup;
	sline>>gd->msl_h;
	sline>>gd->leap;
	sline>>gd->wn_days;

	return *gd;
	
}

