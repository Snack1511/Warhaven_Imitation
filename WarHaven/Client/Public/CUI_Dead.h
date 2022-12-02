#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayerInfo;

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

	void Set_TargetInfo(CPlayerInfo* pTargetInfo) { m_pTargetInfo = pTargetInfo; }

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

private:
	enum DeadUI { DU_Profile, DU_EnemyName, DU_KillText, DU_End };
	CUI_Object* m_pDeadUI[DU_End];

	//_float m_fDeadUIEnableTime = 5.f;
	_float m_fDeadUIEnableTime = 1.f;

private:
	void Create_DeadUI();
	void Set_FadeDeadUI();

private:
	enum RevivalIO { RU_BG, RU_Edge, RU_Bar, RU_Text, RU_Giving, RU_End };

	CUI_Object* m_pRevivalUI[RU_End];

	_float m_fMaxRevivalTime = 5.f;
	_float m_fRevivalTime = 0.f;

private:
	void Create_RevivalUI();

private:
	CPlayerInfo* m_pTargetInfo = nullptr;

};

END