
#include <fstream>
#include <iostream>
#include "ConformityIndex.h"

#define JUDGE_CONDITION_TRUE(con, exe) if(con) { exe;}

/*
*	Summary: 四舍五入函数
*	Parameters:
*	@param1:dVal	数值
*	@param2:iPlaces 保留的位数
*	Return : 四舍五入后的数值
*/
double Round(double dVal, short iPlaces)
{
	double dRetval;
	double dMod = 0.0000001;
	if (dVal < 0.0) dMod = -0.0000001;
	dRetval = dVal;
	dRetval += (5.0 / pow(10.0, iPlaces + 1.0));
	dRetval *= pow(10.0, iPlaces);
	dRetval = floor(dRetval + dMod);
	dRetval /= pow(10.0, iPlaces);
	return(dRetval);
}

/*
*	Summary: 除数保护函数
*	Parameters:
*	@param1:iDivisor   除数
*	@param2:iDividend  被除数
*	Return : 返回商
*/
double DivisorProtect(int iDivisor, int iDividend)
{
	if (0 == iDividend)
	{
		return 0;
	}
	return iDivisor / (iDividend * 1.0);
}

/*
*	Summary: 除数保护函数
*	Parameters:
*	@param1:iDivisor   除数
*	@param2:iDividend  被除数
*	Return : 返回商
*/
double DivisorProtect(double dDivisor, int iDividend)
{
	if (0 == iDividend)
	{
		return 0;
	}
	return dDivisor / iDividend;
}

CConformityIndex::CConformityIndex() : m_iParam(0), m_bReadGradFile(false), m_bReadConfFile(false), m_bReadParamOrNot(false),
	m_bReadDoseOrNot(false), m_bReadTumOrNot(false), m_dGradientIndex(0.0), m_dConformityIndex(0.0), m_bIsGrad(false)
{
	m_strDosePath = "DataCurrentCase/dosegrid.txt";
	m_strTumPath = "DataCurrentCase/tumorgrid.txt";
	m_strParamPath = "DataCurrentCase/Distr_Input_Parameters.txt";
	m_strWriteGradient = "DataCurrentCase/gradient_index_file.txt";
	m_strWriteConformity = "DataCurrentCase/conformity_index_file.txt";
}

CConformityIndex::~CConformityIndex()
{

}

CConformityIndex& CConformityIndex::getInstance()
{
	static CConformityIndex instance;
	return instance;
}

/*
*	Summary: 折射率对外接口
*	Parameters:
*	@param1:
*	@param2:
*	Return : 返回折射率
*/
double CConformityIndex::GetGradientIndex()
{
	GradientIndexProc();
	return m_dGradientIndex;
}

/*
*	Summary: 适形指数对外接口
*	Parameters:
*	@param1:
*	@param2:
*	Return : 返回适形指数
*/
double CConformityIndex::GetConformityIndex()
{
	GradientIndexProc(true);
	return m_dConformityIndex;
}

/*
*	Summary: 主体流程函数
*	Parameters:
*	@param1:
*	@param2:
*	Return :
*/
void CConformityIndex::GradientIndexProc(bool bGrad)
{
	m_bIsGrad = bGrad;
	if (!m_bReadGradFile && !bGrad)
	{
		m_bReadGradFile = true;
		JUDGE_CONDITION_TRUE(!m_bReadParamOrNot, ReadParam());
		JUDGE_CONDITION_TRUE(!m_bReadDoseOrNot, ReadFileDose());
		JUDGE_CONDITION_TRUE(!m_bReadTumOrNot, ReadFileTum());
		CalcFunc();
		WriteFile();
	}
	if (!m_bReadConfFile && bGrad)
	{
		m_bReadConfFile = true;
		JUDGE_CONDITION_TRUE(!m_bReadParamOrNot, ReadParam());
		JUDGE_CONDITION_TRUE(!m_bReadDoseOrNot, ReadFileDose());
		JUDGE_CONDITION_TRUE(!m_bReadTumOrNot, ReadFileTum());
		CalcFunc();
		WriteFile(true);
	}
}

/*
*	Summary: 计算数值函数
*	Parameters:
*	@param1:
*	@param2:
*	Return :
*/
void CConformityIndex::CalcFunc()
{
	int iReceivHALF(0), iReceivPrcptionDose(0), iTumVilume(0);
	CountTumVilume(iTumVilume);
	CountPrescription(iReceivPrcptionDose, iReceivHALF);
	if (!m_bIsGrad)
	{
		m_dGradientIndex = DivisorProtect(iReceivHALF, iReceivPrcptionDose);
	}
	else
	{
		m_dConformityIndex = DivisorProtect(iReceivPrcptionDose * pow(m_DoseReturnValue.dSpacing, 3), iTumVilume);
	}
}

/*
*	Summary: 根据公式计算肿瘤体积
*	Parameters:
*	@param1:iTumVilume 数值为1的数量
*	@param2:
*	Return :
*/
void CConformityIndex::CountTumVilume(int& iTumVilume)
{
	int iU = m_TumReturnValue.iU;
	int iV = m_TumReturnValue.iV;
	int iW = m_TumReturnValue.iW;

	for (int iu = 0; iu < iU; ++iu)
	{
		for (int iv = 0; iv < iV; ++iv)
		{
			for (int iw = 0; iw < iW; ++iw)
			{
				if (1 == m_TumGridVec[iu][iv][iw])
				{
					++iTumVilume;
				}
			}
		}
	}
}

/*
*	Summary: 统计处方剂量
*	Parameters:
*	@param1:iReceivPrcptionDose 容积接受全处方剂量
*	@param2:iReceivHALF			容积接受半处方剂量
*	Return :
*/
void CConformityIndex::CountPrescription(int& iReceivPrcptionDose, int& iReceivHALF)
{
	int iU = m_DoseReturnValue.iU;
	int iV = m_DoseReturnValue.iV;
	int iW = m_DoseReturnValue.iW;
	for (int iu = 0; iu < iU; ++iu)
	{
		for (int iv = 0; iv < iV; ++iv)
		{
			for (int iw = 0; iw < iW; ++iw)
			{
				if (m_iParam <= m_matrixVec[iu][iv][iw])
				{
					++iReceivPrcptionDose;
				}
				if ((m_iParam / 2.0) <= m_matrixVec[iu][iv][iw])
				{
					++iReceivHALF;
				}
			}
		}
	}
}

/*
*	Summary: 设置dosegrid文件的路径
*	Parameters:
*	@param1:strPath 存放文件的路径
*	@param2:
*	Return :
*/
void CConformityIndex::SetDosePath(string strPath)
{
	m_strDosePath = strPath;
	m_bReadGradFile = false;
	m_bReadConfFile = false;
}

/*
*	Summary: 设置tumorgrid文件的路径
*	Parameters:
*	@param1:strPath 存放文件的路径
*	@param2:
*	Return :
*/
void CConformityIndex::SetTumPath(string strPath)
{
	m_strTumPath = strPath;
	m_bReadGradFile = false;
	m_bReadConfFile = false;
}

/*
*	Summary: 设置Distr_Input_Parameters文件的路径
*	Parameters:
*	@param1:strPath 存放文件的路径
*	@param2:
*	Return :
*/
void CConformityIndex::SetParamPath(string strPath)
{
	m_strParamPath = strPath;
	m_bReadGradFile = false;
	m_bReadConfFile = false;
}

/*
*	Summary: 设置参数值
*	Parameters:
*	@param1:iVal 要设置的参数值
*	@param2:
*	Return :
*/
void CConformityIndex::SetParamVal(int iVal)
{
	m_iParam = iVal;
	m_bReadParamOrNot = true;
}

/*
*	Summary: 设置dosegrid的容器值
*	Parameters:
*	@param1:matrixVec 要设置的容器值
*	@param2:
*	Return :
*/
bool CConformityIndex::SetMatrixVec(const matrixVec& matrixVec, int iU, int iV, int iW, double dSpacing)
{
	if (matrixVec.empty())
	{
		cout << "the vector is empty, SetMatrixVec fail! " << endl;
		return false;
	}
	iU = static_cast<int>(Round(iU / dSpacing, 0));
	iV = static_cast<int>(Round(iV / dSpacing, 0));
	iW = static_cast<int>(Round(iW / dSpacing, 0));
	if (iU != matrixVec.size() || iV != matrixVec[0].size() || iW != matrixVec[0][0].size())
	{
		cout << "the vector's size error, SetMatrixVec fail! " << endl;
		return false;
	}
	m_DoseReturnValue.SetValude(iU, iV, iW, dSpacing);
	m_matrixVec = matrixVec;
	m_bReadDoseOrNot = true;
	return true;
}

/*
*	Summary: 设置tumorgrid的容器值
*	Parameters:
*	@param1:tumGridVec 要设置的容器值
*	@param2:
*	Return :
*/
bool CConformityIndex::SetTumGridVec(const TumGridVec& tumGridVec, int iU, int iV, int iW)
{
	if (tumGridVec.empty())
	{
		cout << "the vector is empty, SetTumGridVec fail! " << endl;
		return false;
	}
	if (iU != tumGridVec.size() || iV != tumGridVec[0].size() || iW != tumGridVec[0][0].size())
	{
		cout << "the vector's size error, SetTumGridVec fail! " << endl;
		return false;
	}
	m_DoseReturnValue.SetValude(iU, iV, iW, 0.0);
	m_TumGridVec = tumGridVec;
	m_bReadTumOrNot = true;
	return true;
}

/*
*	Summary: 文件读取数值，并保存
*	Parameters:
*	@param1:doseValue	模板容器
*	@param2:in			读入文件流
*   @param3:returnVal	文件头信息
*	Return : 函数执行是否成功
*/
template <typename TVec, typename TVar>
bool CountFunc(TVec& doseValue, ifstream& in, const ReturnValueStru& returnVal)
{
	TVar value;
	for (int iw = 0; iw < returnVal.iW; ++iw)
	{
		for (int iv = 0; iv < returnVal.iV; ++iv)
		{
			for (int iu = 0; iu < returnVal.iU; ++iu)
			{
				in >> value;
				if (in.fail())
				{
					in.close();
					cout << "Error position: CConformityIndex::CountFunc " << endl;
					return false;
				}
				doseValue[iu][iv][iw] = value;
			}
		}
	}
	return true;
}

/*
*	Summary: 初始化容器
*	Parameters:
*	@param1:doseValue 要初始化的容器对象
*	@param2:
*	Return :
*/
void CConformityIndex::RectorResize(matrixVec& doseValue)
{
	doseValue.resize(m_DoseReturnValue.iU, vector<vector<double>>());
	for (int iu = 0; iu < m_DoseReturnValue.iU; ++iu)
	{
		doseValue[iu].resize(m_DoseReturnValue.iV, vector<double>());
		for (int iv = 0; iv < m_DoseReturnValue.iV; ++iv)
		{
			doseValue[iu][iv].resize(m_DoseReturnValue.iW, double());
		}
	}
}

/*
*	Summary: 初始化容器
*	Parameters:
*	@param1:tumGridVec 要初始化的容器对象
*	@param2:
*	Return :
*/
void CConformityIndex::RectorResize(TumGridVec& tumGridVec)
{
	tumGridVec.resize(m_TumReturnValue.iU, vector<vector<short>>());
	for (int iu = 0; iu < m_TumReturnValue.iU; ++iu)
	{
		tumGridVec[iu].resize(m_TumReturnValue.iV, vector<short>());
		for (int iv = 0; iv < m_TumReturnValue.iV; ++iv)
		{
			tumGridVec[iu][iv].resize(m_TumReturnValue.iW, short());
		}
	}
}

/*
*	Summary: 读取dosegrid文件
*	Parameters:
*	@param1:
*	@param2:
*	Return : 是否读取并保存成功
*/
bool CConformityIndex::ReadFileDose()
{
	ifstream in(m_strDosePath);
	if (!in.is_open())
	{
		cout << "Error opening file: " << m_strDosePath.c_str() << endl;
		return false;
	}
	char cArr[256];
	in.getline(cArr, 255);
	in.getline(cArr, 255);
	in >> m_DoseReturnValue.iU;
	in >> m_DoseReturnValue.iV;
	in >> m_DoseReturnValue.iW;
	in >> m_DoseReturnValue.dSpacing;
	if (abs(m_DoseReturnValue.dSpacing) <= 1e-15)
	{
		return false;
	}
	m_DoseReturnValue.iU = static_cast<int>(Round(m_DoseReturnValue.iU / m_DoseReturnValue.dSpacing, 0));
	m_DoseReturnValue.iV = static_cast<int>(Round(m_DoseReturnValue.iV / m_DoseReturnValue.dSpacing, 0));
	m_DoseReturnValue.iW = static_cast<int>(Round(m_DoseReturnValue.iW / m_DoseReturnValue.dSpacing, 0));
	RectorResize(m_matrixVec);
	if (!CountFunc<matrixVec, double>(m_matrixVec, in, m_DoseReturnValue))
	{
		in.close();
		return false;
	}
	in.close();
	return true;
}

/*
*	Summary: 读取tumorgrid文件
*	Parameters:
*	@param1:
*	@param2:
*	Return : 是否读取并保存成功
*/
bool CConformityIndex::ReadFileTum()
{
	ifstream in(m_strTumPath);
	if (!in.is_open())
	{
		cout << "Error opening file: " << m_strTumPath.c_str() << endl;
		return false;
	}
	in >> m_TumReturnValue.iU;
	in >> m_TumReturnValue.iV;
	in >> m_TumReturnValue.iW;
	RectorResize(m_TumGridVec);
	if (!CountFunc<TumGridVec, int>(m_TumGridVec, in, m_TumReturnValue))
	{
		in.close();
		return false;
	}
	in.close();
	return true;
}

/*
*	Summary: 读取Distr_Input_Parameters文件
*	Parameters:
*	@param1:
*	@param2:
*	Return : 是否读取并保存成功
*/
bool CConformityIndex::ReadParam()
{
	ifstream in(m_strParamPath);
	if (!in.is_open())
	{
		cout << "Error opening file: " << m_strParamPath.c_str() << endl;
		return false;
	}
	in >> m_iParam;
	in.close();
	return true;
}

/*
*	Summary: 写文件
*	Parameters:
*	@param1:bWrite 区分是适形还是折射 默认值true
*	@param2:
*	Return :
*/
void CConformityIndex::WriteFile(bool bWrite)
{
	if (!bWrite)
	{
		ofstream of(m_strWriteGradient);
		of << m_dGradientIndex;
		of.close();
	}
	else
	{
		ofstream of(m_strWriteConformity);
		of << m_dConformityIndex;
		of.close();
	}

}

