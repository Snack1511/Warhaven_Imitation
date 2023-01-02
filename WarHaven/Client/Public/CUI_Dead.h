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
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void Toggle_DeadUI(_bool value, _bool isFall = false);

	void Disable_RevivalUI();

	void Set_TargetInfo(CPlayerInfo* pTargetInfo) { m_pTargetInfo = pTargetInfo; }

	void SetActive_DeadUI(_bool value);
	void SetActive_RevivalUI(_bool value);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

private:
	enum DeadUI { DU_Profile, DU_EnemyName, DU_KillText, DU_End };
	CUI_Object* m_pDeadUI[DU_End];

	_float m_fDeadUIEnableTime = 5.f;

	_float4 m_vGaugeColor = _float4(1.f, 1.f, 1.f, 1.f);

	_bool m_bIsFall = false;

private:
	void Create_DeadUI();
	void Set_FadeDeadUI();

	void Fadenable_DeadUI();

private:
	enum RevivalIO { RU_BG, RU_Edge, RU_Bar, RU_Text, RU_Giving, RU_End };
	CUI_Object* m_pRevivalUI[RU_End];

	_float m_fRevivalTime = 5.f;
	_bool m_bAbleRevival = false;

private:
	void Create_RevivalUI();

private:
	CPlayerInfo* m_pTargetInfo = nullptr;

};

END