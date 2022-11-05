#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CLevel abstract
{
	friend class CLevel_Manager;

protected:
	CLevel();
	virtual ~CLevel();

public:
	void	Destroy_Instance() { delete this; }

public:
	_float		Get_LoadingFinish() { return m_fLoadingFinish; }

public:
	//virtual void Load() PURE; // ���� ������ �ҷ��ͼ� Prototype �̸� ä������
	virtual HRESULT	Initialize() PURE;
	virtual HRESULT	SetUp_Prototypes() PURE;
	virtual HRESULT	Enter();
	virtual void	Tick() PURE;
	virtual void	Late_Tick() PURE;
	virtual HRESULT Render() PURE;
	virtual HRESULT	Exit();

protected:
	_float	m_fLoadingFinish = 0.f;
	vector<pair<CGameObject*, _uint>>	m_vecGameObjects;

protected:
	void	Ready_GameObject(CGameObject* pGameObject, _uint iGroupIndex);
};

END