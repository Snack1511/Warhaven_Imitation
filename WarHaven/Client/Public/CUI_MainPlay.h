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

private:
	CUI_Object* m_pPlayBtnUI[2];

	// Mode Select
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pPrototypeStageBtn = nullptr;
	CUI_Object* m_pStageSelectBtn[4];
	CUI_Object* m_pLock = nullptr;

private:
	void Bind_Btn();

private:
	void Set_ModeBtn();

private:
	void Create_ModeBG();
	void Create_StageBtn();
};