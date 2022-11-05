#pragma once
#include "Engine_Defines.h"

/* */

BEGIN(Engine)

class CTime_Manager
{
	DECLARE_SINGLETON(CTime_Manager);

private:
	CTime_Manager();
	~CTime_Manager();

public:
	_double	Get_DT(_uint iIndex = 0);
	void	Set_TimeSpeed(_uint iIndex, _float fSpeed) { m_arrTimeSpeeds[iIndex] = fSpeed; };
	void	Set_FPSUpdateTime(_double dTime) { m_dFPSUpdateTime = dTime; }

	_double	Get_FPSLimitTime()				{ return m_dFPSLimitTime; }
	void	Set_FPSLimitTIme(_double dTime) { m_dFPSLimitTime = dTime; }
	
	_bool	Can_Update() { return m_bFPSUpdate; }

public:
	HRESULT	Initialize(HWND hWnd, const TIMEDESC& tTimeDesc);
	void	Tick();
	void	Render();

private:
	HWND			m_hWnd = 0;
	LARGE_INTEGER	m_llCurCount = {};
	LARGE_INTEGER	m_llPrevCount = {};
	LARGE_INTEGER	m_llFrequency = {};

	_bool			m_bFPSUpdate = false;

	_double			m_dFPSLimitAcc = 0.;
	_double			m_dFPSLimitTime = 0.;
	_double			m_dFPSUpdateTime = 1.;
	_double			m_dDT = 0.;
	_double			m_dAcc = 0.; 
	_uint			m_iCallCount = 0;
	_uint			m_iFPS = 0;

	_uint			m_iNumTimeScales = 0;
	_float*			m_arrTimeSpeeds;
};

}