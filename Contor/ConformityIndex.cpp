
#include <fstream>
#include <iostream>
#include "ConformityIndex.h"

#define JUDGE_CONDITION_TRUE(con, exe) if(con) { exe;}

/*
*	Summary: �������뺯��
*	Parameters:
*	@param1:dVal	��ֵ
*	@param2:iPlaces ������λ��
*	Return : ������������ֵ
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
*	Summary: ������������
*	Parameters:
*	@param1:iDivisor   ����
*	@param2:iDividend  ������
*	Return : ������
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
*	Summary: ������������
*	Parameters:
*	@param1:iDivisor   ����
*	@param2:iDividend  ������
*	Return : ������
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
*	Summary: �����ʶ���ӿ�
*	Parameters:
*	@param1:
*	@param2:
*	Return : ����������
*/
double CConformityIndex::GetGradientIndex()
{
	GradientIndexProc();
	return m_dGradientIndex;
}

/*
*	Summary: ����ָ������ӿ�
*	Parameters:
*	@param1:
*	@param2:
*	Return : ��������ָ��
*/
double CConformityIndex::GetConformityIndex()
{
	GradientIndexProc(true);
	return m_dConformityIndex;
}

/*
*	Summary: �������̺���
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
*	Summary: ������ֵ����
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
*	Summary: ���ݹ�ʽ�����������
*	Parameters:
*	@param1:iTumVilume ��ֵΪ1������
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
*	Summary: ͳ�ƴ�������
*	Parameters:
*	@param1:iReceivPrcptionDose �ݻ�����ȫ��������
*	@param2:iReceivHALF			�ݻ����ܰ봦������
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
*	Summary: ����dosegrid�ļ���·��
*	Parameters:
*	@param1:strPath ����ļ���·��
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
*	Summary: ����tumorgrid�ļ���·��
*	Parameters:
*	@param1:strPath ����ļ���·��
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
*	Summary: ����Distr_Input_Parameters�ļ���·��
*	Parameters:
*	@param1:strPath ����ļ���·��
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
*	Summary: ���ò���ֵ
*	Parameters:
*	@param1:iVal Ҫ���õĲ���ֵ
*	@param2:
*	Return :
*/
void CConformityIndex::SetParamVal(int iVal)
{
	m_iParam = iVal;
	m_bReadParamOrNot = true;
}

/*
*	Summary: ����dosegrid������ֵ
*	Parameters:
*	@param1:matrixVec Ҫ���õ�����ֵ
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
*	Summary: ����tumorgrid������ֵ
*	Parameters:
*	@param1:tumGridVec Ҫ���õ�����ֵ
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
*	Summary: �ļ���ȡ��ֵ��������
*	Parameters:
*	@param1:doseValue	ģ������
*	@param2:in			�����ļ���
*   @param3:returnVal	�ļ�ͷ��Ϣ
*	Return : ����ִ���Ƿ�ɹ�
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
*	Summary: ��ʼ������
*	Parameters:
*	@param1:doseValue Ҫ��ʼ������������
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
*	Summary: ��ʼ������
*	Parameters:
*	@param1:tumGridVec Ҫ��ʼ������������
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
*	Summary: ��ȡdosegrid�ļ�
*	Parameters:
*	@param1:
*	@param2:
*	Return : �Ƿ��ȡ������ɹ�
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
*	Summary: ��ȡtumorgrid�ļ�
*	Parameters:
*	@param1:
*	@param2:
*	Return : �Ƿ��ȡ������ɹ�
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
*	Summary: ��ȡDistr_Input_Parameters�ļ�
*	Parameters:
*	@param1:
*	@param2:
*	Return : �Ƿ��ȡ������ɹ�
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
*	Summary: д�ļ�
*	Parameters:
*	@param1:bWrite ���������λ������� Ĭ��ֵtrue
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

