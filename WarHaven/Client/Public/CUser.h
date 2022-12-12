#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CUnit;
class CBloodOverlay;
class CPlayer;
class CPlayerInfo;
class CUI_Damage;
class CUI_Training;
class CUI_Animation;
class CUI_Dead;
class CPlayerInfo;

class CUI_Cursor;
class CUI_Wrapper;
class CUI_HUD;
class CUI_Portrait;
class CUI_HpBar;
class CUI_HeroGauge;
class CUI_Skill;

class CUI_Result;
class CUI_Popup;
class CUI_Oper;
class CUI_Paden;

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
	CPlayer* Get_PlayerObejects() { return m_pPlayer; }
	CUnit* Get_Player();
	void		Set_MainPlayerInfo(CPlayerInfo* pPlayerInfo) { m_pPlayerInfo = pPlayerInfo; }
	CPlayerInfo* Get_MainPlayerInfo() { return m_pPlayerInfo; }
	LEVEL_TYPE_CLIENT	Get_CurLevel() { return m_eLoadLevel; }


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
	CUI_Wrapper* Get_HUD(_uint eHUD);

	void Set_UserPort(_uint iClass);
	void Set_HeroPort(_uint iType);
	void Set_HP(_float fCurValue, _float fMaxValue);
	void Set_HeroGauge(_float fCurValue, _float fMaxValue);
	void Set_SkillCoolTime(_uint iSkillIdx, _float fSkillCoolTime, _float fSkillMaxCoolTime);

	void SetActive_Cursor(_bool value);

	void SetActive_Result(_uint iResult, _bool value);
	void SetActive_HUD(_bool value);
	void SetActive_UnitHUD(_bool value);
	void SetActive_AbleHeroText(_bool value);
	void Transform_SkillUI(_uint iClass);

public:		// 파덴
	void Interat_PointUI(_bool bIsMainPlayer, _bool bIsMainPlayerTeam, string wstrPadenPointKey, _uint iTriggerState);
	void Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime);
	void Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum);

	void Conquest_PointUI(string strPointName, _uint iTeamType);

	void Set_TargetPointPos(_uint iTargetIdx);
	void SetActive_TargetPoint(_bool value);

	void Set_Score(_uint iTeamType, _uint iScore, _uint iMaxScore);

	void SetActive_PadenUI(_bool value);

public:		// 브리핑
	void Set_Respawn(_bool value);
	void SetActive_OperUI(_bool value);
	_bool Get_SelectTargetPoint();

public:
	void On_EnterLevel();
	void On_ExitLevel();
	void On_EnterStageLevel();
	void On_ExitStageLevel();

	void Set_HUD(CLASS_TYPE eClass);

	void SetActive_OxenJumpText(_bool value);
	void SetActive_HeroTransformGauge(_bool value);

	_bool Is_OnHeroGauge();

	void Enable_DamageFont(_uint eType, _float fDmg);
	void SetActive_TrainingPopup(_bool value, _uint iIndex);

	void Set_TargetInfo(CPlayerInfo* pTargetInfo);
	void Toggle_DeadUI(_bool value, _bool isFall = false);

public:		// 알림
	void Enable_ConquestPopup(wstring Text = TEXT(""));

private:
	CUI_HUD* m_pUI_HUD = nullptr;
	CUI_Portrait* m_pUI_Portrait = nullptr;
	CUI_HpBar* m_pUI_HP = nullptr;
	CUI_HeroGauge* m_pUI_HeroGauge = nullptr;
	CUI_Skill* m_pUI_Skill = nullptr;
	CUI_Oper* m_pUI_Oper = nullptr;
	CUI_Popup* m_pUI_Popup = nullptr;
	CUI_Result* m_pUI_Result = nullptr;

private:
	CBloodOverlay* m_pBloodOverlay = nullptr;
	CUI_Cursor* m_pCursor = nullptr;

	CUI_Training* m_pUI_Training = nullptr;
	CUI_Paden* m_pUI_Paden = nullptr;

	CUI_Animation* m_pFire = nullptr;

	CUI_Dead* m_pUI_Dead = nullptr;

	CUI_Damage* m_pUI_Damage[5];
	_uint m_iDamageFontIdx = 0;

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;
};
END
