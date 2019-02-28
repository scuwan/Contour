#include "isodosecurves.h"

#include<iostream>
#include<fstream>
using namespace::std;
namespace
{
	int my_round(double number)
	{
		return static_cast<int>((number > 0.0) ? (number + 0.5) : (number - 0.5));
	};
}

CIsodoseCurves::CIsodoseCurves()
{
}


CIsodoseCurves::~CIsodoseCurves()
{
}


bool CIsodoseCurves::loadGrid(const char* path)
{
	ifstream in(path);
	if (!in.is_open())
	{
		cout << "error:open file " << path << endl;
		return false;
	}
	char buf[512];
	in.getline(buf,512);
	in.getline(buf, 512);
	in >> m_x_;
	in >> m_y_;
	in >> m_z_;
	in >> m_spacing_;
	int zMax = m_z_ / m_spacing_;
	int yMax = m_y_ / m_spacing_;
	int xMax = m_x_ / m_spacing_;
	int xyzMax = zMax*yMax*xMax;
	m_doseGrid_.assign(xyzMax, 0.0);
	for (int i = 0; i < xyzMax;++i)
	{
		in >> m_doseGrid_[i];
		if (in.fail())
			return false;
	}
	m_girdOk_ = true;
	return true;
}