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
	virtual void Set_Shader_StageHighlight(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_StageClickRect(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_BtnHighlight(CShader* pShader, const char* pConstName);

public:
	virtual void On_PointEnter_PlayBtn(const _uint& iEventNum);
	virtual void On_PointExit_PlayBtn(const _uint& iEventNum);

	virtual void On_PointUpEvent_Start(const _uint& iEventNum);
	virtual void On_PointUpEvent_Mode(const _uint& iEventNum);

	virtual void On_PointEnter_Stage(const _uint& iEventNum);
	virtual void On_PointStay_Stage(const _uint& iEventNum);
	virtual void On_PointExit_Stage(const _uint& iEventNum);

	virtual void On_PointDown_Stage(const _uint& iEventNum);

private:
	CUI_Object* m_pPlayBtnUI[2];

	// Mode Select
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pTextModeSelect = nullptr;
	CUI_Object* m_pEscKey = nullptr;
	CUI_Object* m_pLine = nullptr;
	CUI_Object* m_pStageHighlight = nullptr;
	CUI_Object* m_pBtnHightlight = nullptr;
	CUI_Object* m_pStageSelectRect = nullptr;
	CUI_Object* m_pStageNameRect = nullptr;

	CUI_Object* m_pPrototypeStageBtn = nullptr;
	CUI_Object* m_pStageSelectBtn[4];

	CUI_Object* m_pPlayBtnMouseEnterLine = nullptr;

	CUI_Object* m_pPrototypeLock = nullptr;
	CUI_Object* m_pLockBtn[3];

	Select_Stage m_eStage = Select_Stage::Test;

private:
	CUI_Object* m_pTarget = nullptr;

	_bool m_bIsPlayBtnOnMouseEnter = false;
	_bool m_bIsMovePlayBtnEnter = false;

	_bool m_bIsPlayBtnOnMouseExit = false;
	_bool m_bIsMovePlayBtnExit = false;

private:	// 폰트 설정
	_float2 vFontOffset = { -100.f, 120.f };
	_float m_fFontSize = 0.33f;

private:
	void Bind_Shader();
	void Bind_Btn();

private:
	void SetActive_ModeWindow();
	void Enable_StageHighlight(_float4 vPos);
	void Enable_StageClickRect(_float4 vPos);

private:
	void Set_LockImg();

private:
	void Create_PlayBtn();
	void Create_ModeBG();
	void Create_StageBtn();
	void Create_LockImg();
	void Create_StageHighlight();
	void Create_BtnHighlight();
	void Create_StageNameRect();
	void Crerate_PlayBtnMouseEnterLine();
};