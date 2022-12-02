#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Client)

class CUnit;

class CUI_HUD : public CUI_Wrapper
{
	enum HUD_Type { Crosshair, Port, Skill, HeroGauge, HpBar, HUD_END };

	DECLARE_PROTOTYPE(CUI_HUD);
	DECLARE_GAMEOBJECT(CUI_HUD);

private:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick();

public:
	virtual void On_PointEnter_Port(const _uint& iEventNum);
	virtual void On_PointExit_Port(const _uint& iEventNum);
	virtual void On_PointDown_Port(const _uint& iEventNum);

	virtual void On_PointDown_SelectBG(const _uint& iEventNum);
	virtual void On_PointDown_Point(const _uint& iEventNum);

public:
	virtual void Set_Shader_Smoke(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Timer(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_HeroTransformGauge(CShader* pShader, const char* pConstName);

public:
	void Set_HUD(CLASS_TYPE eClass);
	void Set_HP(_float fMaxHP, _float fCurHP);
	void Set_HeroGauge(_float fMaxGauge, _float fCurGauge);
	void Set_SkillCoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime);

	void SetActive_OperUI(_bool value);
	void SetActive_PlayerInfoUI(_bool value);

	void SetActive_HeroPortrait(_bool value);
	void SetActive_OxenJumpText(_bool value);
	void SetActive_HeroTransformGauge(_bool value);

public:
	_bool Is_OnHeroGauge();

private:
	CUI_Wrapper* m_pWrap[HUD_END];
	CUI_Wrapper* m_pDeadUI = nullptr;

	CUnit* m_pPlayer = nullptr;

	CUnit::UNIT_STATUS m_tStatus;
	CLASS_TYPE m_eCurClass;
	CLASS_TYPE m_ePrvClass;

private:	// 체력바
	_float m_fMaxHP = 0.f;
	_float m_fCurHP = 0.f;
	_float m_fPrvHP = 0.f;
	_float m_fHealthRatio = 0.f;

private:	// 히어로 게이지
	_float m_fMaxGauge = 0.f;
	_float m_fCurGauge = 0.f;
	_float m_fGaugeRatio = 0.f;
	_bool m_bIsEnableHeroPort = false;

private:	// 작전회의
	CUI_Object* m_pOperWindow = nullptr;
	CUI_Object* m_pSmokeBG = nullptr;
	CUI_Object* m_pOperBlackBG = nullptr;
	CUI_Object* m_pOperTextImg = nullptr;
	CUI_Object* m_pOperTextImg2 = nullptr;
	CUI_Object* m_pSquardTextImg = nullptr;

	CUI_Object* m_pOperProfile = nullptr;
	CUI_Object* m_pArrOperProfile[4];

	CUI_Object* m_pOperSideBG = nullptr;
	CUI_Object* m_pArrOperSideBG[2];

private:
	void Create_OperWindow(LEVEL_TYPE_CLIENT eLoadLevel);
	void Create_OperProfile();
	void Create_OperSideBG();

private:	// 작전회의 캐릭터 선택 창
	enum OperSelectType { ST_Char, ST_Port, ST_BG, ST_Icon, ST_End };

	CUI_Object* m_pOperSelectUI[ST_End];
	CUI_Object* m_pArrOperSelectUI[ST_End][6];

	_uint m_iCurSelectEventNum = 0;
	_uint m_iPrvSelectEventNum = 0;

private:
	void Create_OperSelectCharacter();

private:
	CUI_Object* m_pOperMapIcon = nullptr;
	CUI_Object* m_pOperMapBG = nullptr;

private:
	void Create_OperMap();

private:	// 작전회의 팀 아이콘
	enum TeamUI { TU_Icon, TU_Outline, TU_End };

	CUI_Object* m_pTeamUI[TU_End];
	CUI_Object* m_pArrTeamUI[TU_End][2];

private:
	void Create_TeamUI();

private:	// 작전회의 거점 아이콘
	enum OperPointType { PT_Point, PT_Gauge, PT_Icon, PT_Text, PT_End };

	CUI_Object* m_pOperPointUI[PT_End];
	CUI_Object* m_pArrOperPointUI[PT_End][3];

	CUI_Object* m_pOperAttackPointText = nullptr;

	CUI_Object* m_pTargetPoint = nullptr;
	CUI_Object* m_pArrTargetPoint[2];

	CUI_Object* m_pOperPointCircleEffect = nullptr;
	CUI_Object* m_pArrOperPointCircleEffect[4];

private:
	void Create_OperPoint();
	void Create_OperPointEffect();

private:	// 작전회의 타이머
	enum OperTimerType { TT_BG, TT_Bar, TT_End };
	CUI_Object* m_pOperTimer[TT_End];

	_float m_fMaxOperTime = 10.f;
	_float m_fOperTime = 0.f;
	_float m_fTimerRatio = 1.f;

private:
	void Create_OperTimer();

private:	// 작전회의 목표 설정 글자
	enum BriefingUI { BU_BG, BU_Icon, BU_End };
	CUI_Object* m_pBriefingUI[BU_End];

private:
	void Create_BriefingUI();

private:
	_uint m_iOperWindowCnt = 0;
	_float m_fSmokeUV = 0.f;

private:	// 클래스 변경 창
	enum BootCampUI { BC_Port, BC_PortBG, BC_Icon, BC_Highlight, BC_Line, BC_End };

	CUI_Object* m_pBG = nullptr;

	CUI_Object* m_pBootCampUI[BC_End];
	CUI_Object* m_pArrBootCampUI[BC_End][6];

	_uint m_iCurBootCharEventNum = 0;
	_uint m_iPrvBootCharEventNum = 0;

private:
	void Create_CharacterSelectWindow();

private:
	CUI_Object* m_pClassInfo = nullptr;
	CUI_Object* m_pClassInfoIcon = nullptr;

	CUI_Object* m_pChangeClassText = nullptr;
	CUI_Object* m_pInactiveHeroText = nullptr;
	CUI_Object* m_pHeroGaugeText = nullptr;
	CUI_Object* m_pOxenJumpText = nullptr;
	CUI_Object* m_pHpText = nullptr;

private:
	void BootCamp_CharacterWindow();

	CUI_Object* m_pPlayerNameText = nullptr;

	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;

private:	// 변신 게이지
	enum HeroTransformUI { HT_BG, HT_Bar, HT_End };
	CUI_Object* m_pHeroTransformUI[HT_End];

	_float m_fMaxHeroTransformValue = 1.44272f;
	_float m_fHeroTransformValue = 0.f;
	_float m_fHeroTransformGaugeRatio = 0.f;

private:
	void Create_HeroTransformUI();

private:
	void Bind_Btn();
	void Bind_Shader();

	void Set_FadePortHighlight();
	void Set_FadeOperSelectChaderUI();

private:
	void SetActive_CharacterSelectWindow(_bool value);
	void Set_ClassInfo(CLASS_TYPE eClass);

	void Update_HP();
	void Update_HeroGauge();
	void Update_HeorTransformGauge();

private:	// 작전 회의
	void Update_OperWindow();
	void Enable_OperPointUI();

	void Create_TraingText();
	void Create_HeroGaugeText();
	void Create_OxenJumpText();
	void Create_HpText();
	void Create_PlayerNameText();
	void Create_InactiveHeroText();

	void	On_OperTimeOver();
	void	Disable_AllOperUIs();
};

END