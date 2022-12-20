#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Revive : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Revive);
	DECLARE_GAMEOBJECT(CUI_Revive);

private:
	CUI_Revive();
	virtual ~CUI_Revive();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Position(CTransform* pReviveUnitTransform);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pReviveIcon = nullptr;

private:
	void Create_ReviveIcon();
};

END