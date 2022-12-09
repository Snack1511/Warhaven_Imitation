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

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void SetActive_OperBG(_bool value);
	void SetActive_OperProfile(_bool value);

private:
	_bool m_bIsBriefing = false;
	_uint m_iOperProgress = 0;

private:
	void Progress_Oper();
	void Enable_StrongHoldUI();

private:	// 텍스트 이미지
	enum TextImg { Text_Oper1, Text_Oper2, Text_SelectPoint, Text_End };
	CUI_Object* m_pTextImg[Text_End];

private:
	void Create_TextImg();

private:	// 작전회의 배경
	enum OperBG { OB_BG, OB_Smoke, OB_Black, OB_End };
	CUI_Object* m_pOperBG[OB_End];

	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_TYPE_CLIENT::LEVEL_END;

private:
	void Create_OperBG();

private:	// 작전회의 프로필
	CUI_Object* m_pOperProfile = nullptr;
	CUI_Object* m_pArrOperProfile[4];

private:
	void Create_OperProfile();

private:	// 작전회의 캐릭터 선택 창
	CUI_Object* m_pOperSideBG = nullptr;
	CUI_Object* m_pArrOperSideBG[2];

	enum CharacterPort { CP_PortBG, CP_Port, CP_SelectBG, CP_Class, CP_End };
	CUI_Object* m_pCharacterPort[CP_End];
	CUI_Object* m_pArrCharacterPort[CP_End][6];

	enum SideMapIcon { Map_BG, Map_Icon, Map_SelectBG, Map_End };
	CUI_Object* m_pSideMapUI[Map_End];

	_uint m_iCurSelectEventNum = 0;
	_uint m_iPrvSelectEventNum = 0;

private:
	void Create_OperCharacterSelect();
	void Init_OperCharacterSelect();

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
	void StrongHoldEffect();
};

END