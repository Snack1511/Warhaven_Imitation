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
	void Set_Respawn(_bool value) { m_bIsRespawn = value; }

	void SetActive_BG(_bool value);
	void SetActive_Profile(_bool value);

private:
	virtual void My_Tick() override;
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

	_float m_fSmokeUV = 0.f;

private:
	void Create_OperBG();

private:	// 작전회의 프로필
	CUI_Object* m_pOperProfile = nullptr;
	CUI_Object* m_pArrOperProfile[4];

private:
	void Create_OperProfile();

private:	// 작전회의 캐릭터 선택 창
	CUI_Object* m_CharacterSideBG = nullptr;
	CUI_Object* m_pArrCharacterSideBG[2];

	enum CharacterPort { CP_PortBG, CP_Port, CP_SelectBG, CP_Class, CP_End };
	CUI_Object* m_pCharacterPort[CP_End];
	CUI_Object* m_pArrCharacterPort[CP_End][6];

	enum SideMapIcon { Map_BG, Map_Icon, Map_SelectBG, Map_End };
	CUI_Object* m_pSideMapUI[Map_End];

	_uint m_iCurSelectEventNum = 0;
	_uint m_iPrvSelectEventNum = 0;

private:
	void Create_CharacterSelect();
	void Init_CharacterSelect();

private:
	enum TeamIconUI { Team_Icon, Team_Outline, Team_End };

	CUI_Object* m_pTeamIcon[Team_End];
	CUI_Object* m_pArrTeamIcon[Team_End][2];

	_float4 m_vColorBlue = _float4(0.f, 0.8f, 1.f, 1.f);
	_float4 m_vColorRed = _float4(1.f, 0.2f, 0.f, 1.f);

private:
	void Create_TeamIcon();
	void Init_TeamIcon();

private:
	enum StrongHoldPoint { SP_BG, SP_Outline, SP_Icon, SP_TEXT, SP_End };
	CUI_Object* m_pStrongHoldUI[SP_End];
	CUI_Object* m_pArrStrongHoldUI[SP_End][3];

	CUI_Object* m_pStrongHoldEffect = nullptr;
	CUI_Object* m_pArrStrongHoldEffect[4];

private:
	void Create_StrongHoldUI();
	void Create_StrongHoldEffect();
	void Init_StrongHoldUI();
	void Init_StrongHoldEffect();

private:	// 작전회의 타이머
	enum TimerUI { TU_BG, TU_Bar, TU_End };
	CUI_Object* m_pTimer[TU_End];

	_float m_fMaxOperTime = 30.f;
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
	CUI_Object* m_pTargetPoint = nullptr;
	CUI_Object* m_pArrTargetPoint[2];

private:
	void Create_TargetPoint();

private:
	enum BriefingUI { BU_BG, BU_Icon, BU_End };
	CUI_Object* m_pBriefingUI[BU_End];

private:
	void Create_BriefingUI();

private:
	void Bind_Shader();
	void Bind_Btn();
};

END