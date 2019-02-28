#ifndef _CONFORMITY_INDEX_H_
#define _CONFORMITY_INDEX_H_

#include <vector>

using namespace std;

typedef vector<vector<vector<double>>> matrixVec;
typedef vector<vector<vector<short>>> TumGridVec;

typedef struct RETURN_VALUE_STRU
{
	int iU;
	int iV;
	int iW;
	double dSpacing;

	RETURN_VALUE_STRU() : iU(0), iV(0), iW(0), dSpacing(0.0)
	{

	}

	void SetValude(int iu, int iv, int iw, double dSpac)
	{
		iU = iu;
		iV = iv;
		iW = iw;
		dSpacing = dSpac;
	}

} ReturnValueStru;

class CConformityIndex
{
public:

	static CConformityIndex& getInstance();

	// ����ӿں���  
	double GetGradientIndex();
	double GetConformityIndex();
	
	// ���ö�ȡ·��/��ԱĬ��ֵ
	void SetDosePath(string strPath);
	void SetTumPath(string strPath);
	void SetParamPath(string strPath);
	void SetParamVal(int iVal);
	bool SetMatrixVec(const matrixVec& matrixVec, int iU, int iV, int iW, double dSpacing);
	bool SetTumGridVec(const TumGridVec& tumGridVec, int iU, int iV, int iW);

private:
	CConformityIndex();
	~CConformityIndex();
	CConformityIndex(const CConformityIndex&);
	CConformityIndex& operator=(const CConformityIndex&);
	// ��д�ļ�����
	bool ReadFileDose();
	void RectorResize(matrixVec& doseValue);
	bool ReadFileTum();
	void RectorResize(TumGridVec& tumGridVec);
	bool ReadParam();
	void WriteFile(bool bWrite = false);

	// GradientIndex������������
	void GradientIndexProc(bool bGrad = false);
	void CountTumVilume(int& iTumVilume);
	void CountPrescription(int& iReceivPrcptionDose, int& iReceivHALF);
	void CalcFunc();

	// ���̿��� ��־ 
	bool m_bReadGradFile;
	bool m_bReadConfFile;
	bool m_bReadParamOrNot;
	bool m_bReadDoseOrNot;
	bool m_bReadTumOrNot;
	bool m_bIsGrad;

	// ���շ��ؽ��
	double m_dGradientIndex;
	double m_dConformityIndex;

	// ����Ĭ�ϻ�������ֵ ��Ա����
	int m_iParam;
	string m_strDosePath;
	string m_strTumPath;
	string m_strParamPath;
	string m_strWriteGradient;
	string m_strWriteConformity;
	matrixVec m_matrixVec;
	TumGridVec m_TumGridVec;

	// ������� ��Ա����
	ReturnValueStru m_DoseReturnValue;
	ReturnValueStru m_TumReturnValue;
};

#endif // _CONFORMITY_INDEX_H_
