#ifndef CISODOSECURVES_1365489521_H_
#define CISODOSECURVES_1365489521_H_

#include<vector>
using std::vector;
class CIsodoseCurves
{
public:
	CIsodoseCurves();
	~CIsodoseCurves();
	bool loadGrid(const char*);
	bool m_girdOk_;
	int m_x_;
	int m_y_;
	int m_z_;
	int m_spacing_;
	vector<double> m_doseGrid_;
};


#endif