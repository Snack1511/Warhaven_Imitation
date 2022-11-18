#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_WarHammer
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_WarHammer);

	enum BarricadeCnt { BarricadeCnt_END = 2};

private:
	CUnit_WarHammer();
	virtual ~CUnit_WarHammer();

public:
	static	CUnit_WarHammer* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	_uint	Get_Size() {
		return (_uint)m_Barricade.size();
	}

	CGameObject* Get_Barricade() {
		return m_Barricade.back();
	}

	void	Destroy_Barricade(CGameObject* pBarricade) {
		m_Barricade.push_back(pBarricade);
	}

	void	Creaete_Barricade() {
		
		if (!m_Barricade.empty())
		{
			ENABLE_GAMEOBJECT(m_Barricade.back());
			m_Barricade.pop_back();
		}
	}


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<CGameObject*>	m_Barricade;//[2];

};

END