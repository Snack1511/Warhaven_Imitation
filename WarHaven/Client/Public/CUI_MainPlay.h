#pragma once
#include "CUI_Wrapper.h"
class CUI_MainPlay : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MainPlay);
	DECLARE_GAMEOBJECT(CUI_MainPlay);

private:
	CUI_MainPlay();
	virtual ~CUI_MainPlay();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void On_PointUpEvent_Start(const _uint& iEventNum);
	virtual void On_PointUpEvent_Mode(const _uint& iEventNum);

	virtual void On_PointEnterStay_Stage(const _uint& iEventNum);

private:
	CUI_Object* m_pPlayBtnUI[2];

	// Mode Select
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pTextModeSelect = nullptr;
	CUI_Object* m_pEscKey = nullptr;
	CUI_Object* m_pLine = nullptr;

	CUI_Object* m_pPrototypeStageBtn = nullptr;
	CUI_Object* m_pStageSelectBtn[4];

	CUI_Object* m_pPrototypeLock = nullptr;
	CUI_Object* m_pLockBtn[3];

private:	// 폰트 설정
	_float2 vFontOffset = { -100.f, 120.f };
	_float m_fFontSize = 0.33f;

private:
	void Bind_Btn();

private:
	void Set_ModeBtn();
	void Set_LockImg();

private:
	void Create_ModeBG();
	void Create_StageBtn();
	void Create_LockImg();
};