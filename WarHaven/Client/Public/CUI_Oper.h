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

private:	// 텍스트 이미지
	enum TextImg { TI_Oper1, TI_Oper2, TI_SelectPoint, TI_End };
	CUI_Object* m_pTextImg[TI_End];

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
	enum TeamIcon { TI_Icon, TI_Outline, TI_End };
	CUI_Object* m_pTeamIcon[TI_End];
	CUI_Object* m_pArrTeamIcon[TI_End][2];
};

END