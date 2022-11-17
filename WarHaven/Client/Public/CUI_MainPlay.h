#pragma once
#include "CUI_Wrapper.h"
class CUI_MainPlay : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MainPlay);
	DECLARE_GAMEOBJECT(CUI_MainPlay);

public:
	enum Select_Stage { Test, Training, Stage_End };

private:
	CUI_MainPlay();
	virtual ~CUI_MainPlay();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

	virtual void My_Tick() override;

public:
	virtual void On_PointUpEvent_Start(const _uint& iEventNum);
	virtual void On_PointUpEvent_Mode(const _uint& iEventNum);

	virtual void On_PointStay_Stage(const _uint& iEventNum);

	virtual void On_PointDown_Stage0(const _uint& iEnventNum);
	virtual void On_PointDown_Stage1(const _uint& iEnventNum);
	virtual void On_PointDown_Stage2(const _uint& iEnventNum);
	virtual void On_PointDown_Stage3(const _uint& iEnventNum);

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

	Select_Stage m_eStage = Select_Stage::Stage_End;

private:	// 폰트 설정
	_float2 vFontOffset = { -100.f, 120.f };
	_float m_fFontSize = 0.33f;

private:
	void Bind_Btn();

private:
	void SetActive_ModeWindow();

private:
	void Set_ModeBtn();
	void Set_LockImg();

private:
	void Create_ModeBG();
	void Create_StageBtn();
	void Create_LockImg();
};