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
class CPlayerInfo;
class CUI_HUD;
class CUI_Damage;
class CUI_Training;
class CUI_Animation;
class CUI_Dead;
class CPlayerInfo;

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
	void		Set_Player(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer*	Get_PlayerObejects() { return m_pPlayer; }
	CUnit*		Get_Player();
	void		Set_MainPlayerInfo(CPlayerInfo* pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	CPlayerInfo* Get_MainPlayerInfo() { return m_pPlayerInfo; }


	void		Set_FixCursor(_bool bEnable) { m_bFixCursor = bEnable;  ::ShowCursor(!bEnable); }

	CUI_Cursor* Get_Cursor() { return m_pCursor; }
	void		Set_Cursor(CUI_Cursor* pCursor) { m_pCursor = pCursor; }

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
	CPlayerInfo* m_pPlayerInfo = nullptr;
	CPlayer* m_pPlayer = nullptr;

private:
	_bool	m_bFixCursor = false;
	POINT	m_ptCenter;
	list<KEY>		m_KeyCommands;

public:
	void SetUp_BloodOverlay();
	void Turn_BloodOverLay(_float fHpRatio);
	void SetUp_HeroGaugeFire();
	void Turn_HeroGaugeFire(_bool bTurnOn);

public:
	void On_EnterLevel();
	void On_ExitLevel();
	void On_EnterStageLevel();
	void On_ExitStageLevel();

	void Set_HUD(CLASS_TYPE eClass);
	void Set_HP(_float fMaxHP, _float fCurHP);
	void Set_HeroGauge(_float fMaxGauge, _float fCurGauge); 
	void Set_SkillCoolTime(_uint iSkillType, _float fSkillCoolTime, _float fMaxCoolTime);

	void SetActive_HeroPortrait(_bool value);
	void SetActive_OxenJumpText(_bool value);
	void SetActive_HeroTransformGauge(_bool value);

	_bool Is_OnHeroGauge(); 

	void Enable_DamageFont(_uint eType, _float fDmg);
	void SetActive_TrainingPopup(_bool value, _uint iIndex);

	void Set_TargetInfo(CPlayerInfo* pTargetInfo);
	void Enable_DeadUI();

private:
	

private:
	CBloodOverlay* m_pBloodOverlay = nullptr;
	CUI_Cursor* m_pCursor = nullptr;
	CUI_HUD* m_pUI_HUD = nullptr;
	CUI_Training* m_pUI_Training = nullptr;

	CUI_Animation* m_pFire = nullptr;

	CUI_Dead* m_pUI_Dead = nullptr;

	CUI_Damage* m_pUI_Damage[5];
	_uint m_iDamageFontIdx = 0;
};
END
