#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CCamera;

class CCamera_Manager
{
	DECLARE_SINGLETON(CCamera_Manager);

private:
	CCamera_Manager();
	virtual ~CCamera_Manager();

public:
	void	Set_LoadFinish() { m_bOnceCheck = true; }

public:
	_float4x4	Get_View() { return m_tView.matView; }
	_float4x4	Get_Proj() { return m_tProj.matProj; }
	_float4x4	Get_ViewProj() { return m_tView.matView * m_tProj.matProj; }
	_float4x4	Get_BillBoard();
	_float4x4	Get_OrthoView() { return m_matOrthoView; }
	_float4x4	Get_OrthoProj() { return m_matOrthoProj; }
	_float4x4	Get_OldProj() { return m_matOldProj; }
	_float4x4	Get_OldView() { return m_matOldView; }

	CCamera* Get_CurCam() { return m_pCurCam; }

	_float4 Get_ViewPos();

public:
	HRESULT	SetUp_ShaderResources(_bool Ortho = false);
	HRESULT	SetUp_ShaderResources(_uint iShaderIndex, _bool Ortho = false);

public:
	HRESULT	Initialize(const GRAPHICDESC& GraphicDesc);
	void	Make_ViewProj();
	
public:
	CCamera*	Change_Camera(wstring strKey);
	CCamera*	Reset_Camera(wstring strKey);
	void		Add_Camera(wstring strKey, CCamera* pCamera);

	void		Add_Camera_Level(wstring strKey, CCamera* pCamera);
	CCamera*	Find_Camera(wstring strKey);

	void		Clear_LevelCam();

private:
	VIEW_TRANSFORM			m_tView;
	PROJ_TRANSFORM			m_tProj;
	_float					m_fAspect = 0.f;

	CCamera*				m_pCurCam = nullptr;
	CCamera* m_pNextCamera = nullptr;

	map<_hashcode, CCamera*>	m_mapCam;

	/* Level ��ȯ�� ������� ī�޶� */
	map<_hashcode, CCamera*>	m_mapLevelCam;

private:
	_bool			m_bOnceCheck = false;

private:
	_float4x4										m_matOrthoView;
	_float4x4										m_matOrthoProj;


private:
	_float4x4										m_matOldView;
	_float4x4										m_matOldProj;

private:
	void	Make_ViewMatrix();
	void	Make_ProjMatrix();
};

END