#include "..\Public\Time_Manager.h"

#include "GameInstance.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)


CTime_Manager::CTime_Manager()
{
}


CTime_Manager::~CTime_Manager()
{
	SAFE_DELETE_ARRAY(m_arrTimeSpeeds);
}

HRESULT CTime_Manager::Initialize(HWND hWnd, const TIMEDESC& tTimeDesc)
{
	m_hWnd = hWnd;
	m_iNumTimeScales = tTimeDesc.iNumTimeScale;
	m_arrTimeSpeeds = new _float[tTimeDesc.iNumTimeScale];

	for (_uint i = 0; i < tTimeDesc.iNumTimeScale; ++i)
	{
		m_arrTimeSpeeds[i] = 1.f;
	}

	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);
	


	return S_OK;
}

_double CTime_Manager::Get_DT(_uint iIndex)
{
	// 진짜 DT가 FPSLimit (예 : 60분의 1)보다 작으면
	// 프레임이 더 잘나오고 있는거다.
	// 하지만 제한을 걸어야 하므로 1/60 을 리턴.
	//return (m_dDT < m_dFPSLimitTime) ? m_dFPSLimitTime : m_dDT;
#ifdef _DEBUG
	if (m_dDT * m_arrTimeSpeeds[iIndex] > 0.16)
		return 1. / 60.;

#endif // _DEBUG


	return m_dDT * m_arrTimeSpeeds[iIndex];
}


void CTime_Manager::Tick()
{
	m_bFPSUpdate = false;

	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;
	

	m_dFPSLimitAcc += m_dDT;
	Render();

	if (m_dFPSLimitAcc > m_dFPSLimitTime)
	{
		m_dFPSLimitAcc = 0.;
		m_bFPSUpdate = true;
	}
}
void CTime_Manager::Render()
{
	if (m_dFPSLimitAcc > m_dFPSLimitTime)
		++m_iCallCount;

	m_dAcc += m_dDT;

	if (m_dFPSUpdateTime <= m_dAcc) {

		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"War Haven (jusin 128기)                                        FPS : %d,  DT : %f", m_iFPS, m_dDT);
		SetWindowText(m_hWnd, szBuffer);

	}
}
