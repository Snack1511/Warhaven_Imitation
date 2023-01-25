#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Oper : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Oper);
	DECLARE_GAMEOBJECT(CUI_Oper);

private:
	CUI_Oper();
	virtual ~CUI_Oper();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_Smoke(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_Timer(CShader* pShader, const char* pConstName);

public:
	virtual void On_PointDown_SelectBG(const _uint& iEventNum);
	virtual void On_PointDown_StrongHoldPoint(const _uint& iEventNum);

	virtual void On_PointDown_RespawnBtn(const _uint& iEventNum);

public:
	_bool Get_SelectTargetPoint() { return m_bSelectTargetPoint; }

public:
	void Set_Respawn(_bool value) { m_bIsRespawn = value; }

	void Set_PointColor(_bool IsMainTeam, _uint iPoinIdx);
	void Set_Player(CPlayer* pPlayer);
	void Set_BattlePlayer(_bool IsBattle);

	void SetActive_BG(_bool value);
	void SetActive_Profile(_bool value);
	void SetActive_LeftIcon(_bool value);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;

	_bool m_bIsBriefing = false;
	_uint m_iOperProgress = 0;
	_uint m_iOperRespawn = 0;

	_bool m_bIsRespawn = false;

private:
	list<CUI_Object*> m_pOperList;

	_float m_fDisableProfileTime = 4.f;

private:
	void Return_SidePos();

private:
	void Progress_Oper();
	void Enable_StrongHoldUI();

private:	// 텍스트 이미지
	enum TextImg { Text_Oper1, Text_Oper2, Text_SelectPoint, Text_End };
	CUI_Object* m_pTextImg[Text_End];

	enum TargetText { TargetText_BG, TargetText_Icon, TargetText_End };
	CUI_Object* m_pTargetText[TargetText_End];

private:
	void Create_TextImg();
	void Create_TargetText();

private:	// 작전회의 배경
	enum OperBG { OB_BG, OB_Smoke, OB_Black, OB_End };
	CUI_Object* m_pOperBG[OB_End];

	CUI_Object* m_pRealMap = nullptr;

	_float m_fSmokeUV = 0.f;

private:
	void Create_OperBG();

private:	// 작전회의 프로필
	CUI_Object* m_pOperProfile = nullptr;
	CUI_Object* m_pArrOperProfile[4];

	CUI_Object* m_pPositionText = nullptr;
	CUI_Object* m_pArrPositionText[4];

	CUI_Object* m_pMarkMeText = nullptr;

private:
	void Create_OperProfile();
	void Create_ProfileInfo();
	void Set_OperProfile();


private:	// 작전회의 캐릭터 선택 창
	CUI_Object* m_CharacterSideBG = nullptr;
	CUI_Object* m_pArrCharacterSideBG[2];

	enum CharacterPort { CP_PortBG, CP_Port, CP_SelectBG, CP_Class, CP_End };
	CUI_Object* m_pCharacterPort[CP_End];
	CUI_Object* m_pArrCharacterPort[CP_End][6];

	enum LeftIcon { Left_BG, Left_Icon, Left_SelectBG, Left_Num, Left_End };
	CUI_Object* m_pLeftUI[Left_End];
	CUI_Object* m_pArrLeftUI[4][Left_End];

	_uint m_iCurSelectEventNum = 0;
	_uint m_iPrvSelectEventNum = 0;

private:
	void Create_CharacterSelect();
	void Create_LeftIcon();
	void Init_CharacterSelect();

private:
	enum TeamIconUI { Team_Icon, Team_Outline, Team_End };

	CUI_Object* m_pTeamIcon[Team_End];
	CUI_Object* m_pArrTeamIcon[Team_End][2];

	_float4 m_vColorBlue = _float4(0.f, 0.8f, 1.f, 1.f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.f, 1.f);
	_float4 m_vColorLightGreen = _float4(0.6f, 0.85f, 0.3f, 1.f);

private:
	void Create_TeamIcon();
	void Init_TeamIcon();

private:
	enum StrongHoldPoint { SP_BG, SP_Outline, SP_Icon, SP_TEXT, SP_End };
	CUI_Object* m_pStrongHoldUI[SP_End];
	CUI_Object* m_pArrStrongHoldUI[SP_End][3];

	CUI_Object* m_pStrongHoldEffect = nullptr;
	CUI_Object* m_pArrStrongHoldEffect[6];

private:
	void Create_StrongHoldUI();
	void Create_StrongHoldEffect();
	void Init_StrongHoldUI();
	void Init_StrongHoldEffect();

private:	// 작전회의 타이머
	enum TimerUI { TU_BG, TU_Bar, TU_End };
	CUI_Object* m_pTimer[TU_End];

	_float m_fMaxOperTime = 12.f;
	_float m_fOperTime = 0.f;
	_float m_fTimerRatio = 1.f;

private:
	void Create_OperTimer();

private:	// 합류 버튼
	CUI_Object* m_pRespawnBtn = nullptr;

private:
	void Create_RespawnBtn();

private:
	CUI_Object* m_pBlackImg = nullptr;

private:
	void Create_BlackImg();

private:
	enum PointInfo { Info_BG, Info_Icon, Info_End };
	CUI_Object* m_pPointInfo[Info_End];
	CUI_Object* m_pArrPointInfo[3][Info_End];

private:
	void Create_PointInfo();
	void Init_PointInfo();

private:
	CUI_Object* m_pTargetPoint = nullptr;
	CUI_Object* m_pArrTargetPoint[2];

	_bool m_bSelectTargetPoint = false;

private:
	void Create_TargetPoint();

private:
	enum BriefingUI { BU_BG, BU_Icon, BU_End };
	CUI_Object* m_pBriefingUI[BU_End];

private:
	CUI_Object* m_pPlayerIcon[8];

	CPlayer* m_pPlayers[8];
	CTransform* m_pPlayerTransform[8];

	_uint m_iMainSquadIdx = 1;
	_uint m_iMainSquadMaxIdx = 3;

	_uint m_iMainTeamIdx = 4;
	_uint m_iMainTeamMaxIdx = 8;

	_bool m_bIsBattle = false;
	_float m_fBattleTime = 0.f;

private:
	void Create_PlayerIcon();

private:
	void Create_BriefingUI();

private:
	CUI_Object* m_pSelectEffect = nullptr;
	CUI_Object* m_pArrSelectEffect[7];

	_float m_fSelectEffect_RotValue = 0.f;

	CUI_Object* m_pConquestBlur = nullptr;
	CUI_Object* m_pArrConquestBlur[3];

	_bool	m_bIsOperation = true;
	_bool	m_bRespawnTriggerClicked = false;

private:
	void Create_SelectEffect();
	void Init_SelectEffect();
	void Create_ConquestBlur();
	void Init_ConquestBlur();

private:
	void Bind_Shader();
	void Bind_Btn();
};

END