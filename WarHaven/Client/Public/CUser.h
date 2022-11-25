#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;
class CBloodOverlay;
class CUI_Cursor;
class CPlayer;
class CUI_HUD;
class CUI_Damage;
class CUI_Training;

class CUser
{
	DECLARE_SINGLETON(CUser);

private:
	CUser();
	~CUser();

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	Set_Player(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* Get_PlayerObejects() { return m_pPlayer; }
	CUnit* Get_Player();
	void	Set_FixCursor(_bool bEnable) { m_bFixCursor = bEnable;  ::ShowCursor(!bEnable); }

	CUI_Cursor* Get_Cursor() { return m_pCursor; }
	void Set_Cursor(CUI_Cursor* pCursor) { m_pCursor = pCursor; }

public:
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();

	KEY		Get_LastKey() {
		if (m_KeyCommands.empty())
			return KEY::LAST;
		return m_KeyCommands.back();
	}
	void	Clear_KeyCommands() { m_KeyCommands.clear(); }
	void	Update_KeyCommands();

public:
	void	Time_Slow();

private:
	CPlayer* m_pPlayer = nullptr;

private:
	_bool	m_bFixCursor = false;
	POINT	m_ptCenter;
	list<KEY>		m_KeyCommands;

public:
	void SetUp_BloodOverlay();
	void Turn_BloodOverLay(_float fHpRatio);

public:
	void On_EnterLevel();
	void On_ExitLevel();
	void On_EnterStageLevel();
	void On_ExitStageLevel();

	void Set_HP(_float fMaxHP, _float fCurHP);
	void Set_HeroGauge(_float fMaxGauge, _float fCurGauge);

	void SetActive_OxenJumpText(_bool value);
	void Set_SkillCoolTime(_uint iSkillType, _float fSkillCoolTime, _float fMaxCoolTime);
	void Enable_DamageFont(_float fDmg, _bool bHeadShot = false);
	void SetActive_TrainingPopup(_bool value, _uint iIndex);

private:
	CBloodOverlay* m_pBloodOverlay = nullptr;
	CUI_Cursor* m_pCursor = nullptr;
	CUI_HUD* m_pUI_HUD = nullptr;
	CUI_Training* m_pUI_Training = nullptr;

	CUI_Damage* m_pUI_Damage[5];
	_uint m_iDamageFontIdx = 0;
};
END
