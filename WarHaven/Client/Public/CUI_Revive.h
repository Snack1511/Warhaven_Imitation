#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;

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
	void Set_RevivePos(CTransform* pReviveUnitTransform);

	void Set_ReviveIcon(_uint iIconIndex);
	void Set_ClassIcon(CPlayer* pDeadPlayer);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pReviveIcon = nullptr;

	enum ClassIcon { Class_BG, Class_Icon, Class_End };
	CUI_Object* m_pClassIcon[Class_End];

private:
	void Create_ReviveIcon();
	void Create_ClassIcon();
};

END