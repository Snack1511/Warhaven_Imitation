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
class CUI_AbleHeroFire;
class CUI_Dead;
class CPlayerInfo;

class CUI_Cursor;
class CUI_Wrapper;
class CUI_HUD;
class CUI_Portrait;
class CUI_HpBar;
class CUI_HeroGauge;
class CUI_Crosshair;
class CUI_Skill;

class CUI_KillName;
class CUI_KillLog;
class CUI_Result;
class CUI_Popup;
class CUI_Oper;
class CUI_Paden;
class CUI_Revive;
class CUI_Interact;
class CUI_MiniMap;

class CTeamConnector;

class CFadeDark;

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


	void		Set_FixCursor(_bool bEnable) {
		//#ifdef _DEBUG
		if (bEnable != m_bFixCursor)
			::ShowCursor(!bEnable);
		//#endif 

		m_bFixCursor = bEnable;

	}

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
	void	Set_CrossHairPos(_float4 vPosition);

public:
	void SetUp_BloodOverlay();
	void Turn_BloodOverLay(_float fHpRatio);
	void SetUp_HeroGaugeFire();
	void Turn_HeroGaugeFire(_bool bTurnOn);
	void Turn_AbleHeroFire();

public:
	CUI_Wrapper* Get_HUD(_uint eHUD);

	void Set_UserPort(_uint iClass);
	void Set_HeroPort(_uint iType);
	void Set_HP(_float fCurValue, _float fMaxValue);
	void Set_HeroGauge(_float fCurValue, _float fMaxValue);
	void Set_SkillCoolTime(_uint iSkillIdx, _float fSkillCoolTime, _float fSkillMaxCoolTime);

	void SetActive_Cursor(_bool value);

	void SetActive_SquardInfo(_bool value);
	void SetActive_Result(_uint iResult, _bool value);
	void SetActive_HUD(_bool value);
	void SetActive_UnitHUD(_bool value);
	void SetActive_AbleHeroText(_bool value);
	void Transform_SkillUI(_uint iClass);

public:		// 파덴
	void Interat_PointUI(_bool bIsMainPlayerTeam, string strPadenPointKey);
	void Move_PointUI(string wstrPadenPointKey, _uint iTriggerState);

	void Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime);
	void Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum);

	void Set_MiniMapConquestTime(_uint iPoinIdx, _float fConquestTime, _float fMaxConquestTime);
	void Set_MiniMapGaugeColor(_bool IsMainTeam, _uint iPointIdx);
	void Set_MiniMapPointColor(_bool IsMainTeam, _uint iPointIdx);
	void Set_MiniMapPlayer(CPlayer* pPlayer);

	void Conquest_PointUI(string strPointName, _bool bIsMainPlayerTeam);

	void Set_TargetTransform(CTransform* pTargetTransform);
	void Set_TargetPointPos(_uint iTargetIdx);
	void SetActive_TargetPoint(_bool value);

	void Set_Team(CTeamConnector* pAllyTeam, CTeamConnector* pEnemyTeam);

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

	void SetActive_MiniMap(_bool value);

	void Set_TargetInfo(CPlayerInfo* pTargetInfo);
	void Toggle_DeadUI(_bool value, _bool isFall = false);
	void Disable_RevivalUI();

public:
	void Enable_Popup(_uint iPopupType);

public:	// 킬로그
	void Add_KillLog(CPlayer* attacker, CPlayer* victim);
	void Update_KillLog();

	void Add_KillName(wstring wstrEnemyName);
	void Update_KillName();

public:	// 크로스헤어
	void Set_ArcherPoint(_bool value);

public:	// 상호작용
	void SetActive_InteractUI(_bool value);
	void Set_InteractKey(_uint iKeyIndex);
	void Set_InteractText(wstring wstrText);
	void Set_InteractTarget(CGameObject* pInteractTarget);

private:
	CUI_HUD* m_pUI_HUD = nullptr;
	CUI_Portrait* m_pUI_Portrait = nullptr;
	CUI_HpBar* m_pUI_HP = nullptr;
	CUI_HeroGauge* m_pUI_HeroGauge = nullptr;
	CUI_Crosshair* m_pUI_Crosshair = nullptr;
	CUI_Skill* m_pUI_Skill = nullptr;
	CUI_Oper* m_pUI_Oper = nullptr;
	CUI_Popup* m_pUI_Popup = nullptr;
	CUI_Result* m_pUI_Result = nullptr;

private:	// 소생
	CUI_Revive* m_pReviveUI[7];
	_uint m_iReviveIdx = 0;

private:	// 상호작용
	CUI_Interact* m_pInteractUI = nullptr;

private:
	CBloodOverlay* m_pBloodOverlay = nullptr;
	CUI_Cursor* m_pCursor = nullptr;

	CUI_Training* m_pUI_Training = nullptr;
	CUI_Paden* m_pUI_Paden = nullptr;

	CUI_Animation* m_pFire = nullptr;
	CUI_AbleHeroFire* m_pUVFire = nullptr;
	CUI_Dead* m_pUI_Dead = nullptr;

	CUI_MiniMap* m_pMiniMap = nullptr;

	CUI_Damage* m_pUI_Damage[5];
	_uint m_iDamageFontIdx = 0;

private:	// KillLog
	list<CUI_KillLog*> m_pKillLogList;
	CUI_KillLog* m_pKillLog[10];
	_uint m_iKillLogIdx = 0;

private:
	list<CUI_KillName*> m_pKillNameList;
	CUI_KillName* m_pKillName[5];
	_uint m_iKillNameIdx = 0;


private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;
};
END
