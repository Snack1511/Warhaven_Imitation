#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Dead : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Dead);
	DECLARE_GAMEOBJECT(CUI_Dead);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void Enable_DeadUI();

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

private:
	enum DeadUI { DU_Profile, DU_EnemyName, DU_KillText, DU_End };
	CUI_Object* m_pDeadUI[DU_End];

private:
	void Create_DeadUI();
	void Set_FadeDeadUI();
};

END