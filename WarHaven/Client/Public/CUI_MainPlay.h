#pragma once
#include "CUI_Wrapper.h"
class CUI_MainPlay : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_MainPlay);
	DECLARE_GAMEOBJECT(CUI_MainPlay);

public:
	enum Select_Map { Paden, Hwara, Training = 3, Stage_End };

private:
	CUI_MainPlay();
	virtual ~CUI_MainPlay();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual void Set_Shader_StageHighlight(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_StageClickRect(CShader* pShader, const char* pConstName);
	virtual void Set_Shader_BtnHighlight(CShader* pShader, const char* pConstName);

public:
	virtual void On_PointEnter_PlayBtn(const _uint& iEventNum);

	virtual void On_PointExit_Start(const _uint& iEventNum);
	virtual void On_PointExit_Mode(const _uint& iEventNum);

	virtual void On_PointUpEvent_Start(const _uint& iEventNum);
	virtual void On_PointUpEvent_Mode(const _uint& iEventNum);

	virtual void On_PointEnter_Stage(const _uint& iEventNum);
	virtual void On_PointStay_Stage(const _uint& iEventNum);
	virtual void On_PointExit_Stage(const _uint& iEventNum);

	virtual void On_PointDown_Stage(const _uint& iEventNum);

private:
	_bool m_bIsMouseEvent = false;

	list<CUI_Object*> m_pUIList;

private:
	CUI_Object* m_pPlayBtnUI[2];

	// Mode Select
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pTextModeSelect = nullptr;
	CUI_Object* m_pEscKey = nullptr;
	CUI_Object* m_pLine = nullptr;

	CUI_Object* m_pStageHighlight = nullptr;
	CUI_Object* m_pStageHighlights[4];

	CUI_Object* m_pBtnHightlight = nullptr;
	CUI_Object* m_pStageSelectRect = nullptr;
	CUI_Object* m_pStageNameRect = nullptr;

	CUI_Object* m_pPrototypeStageBtn = nullptr;
	CUI_Object* m_pStageSelectBtn[4];

	CUI_Object* m_pPlayBtnMouseEnterLine = nullptr;
	CUI_Object* m_pPlayBtnMouseEnterLineArr[2];

	CUI_Object* m_pPrototypeLock = nullptr;
	CUI_Object* m_pLockBtn[2];

	CUI_Object* m_pSelectTextRect = nullptr;

	static Select_Map m_eStage;
	static wstring m_wstrModeText;

private:
	CUI_Object* m_pTarget = nullptr;

private:	// 폰트 설정
	_float2 vFontOffset = { -100.f, 120.f };
	_float m_fFontSize = 0.33f;
	_float4 m_vFontColor = _float4(0.773f, 0.714f, 0.596f, 1.f);

private:
	void Bind_Shader();
	void Bind_Btn();

	void Set_FadeModeWindow();

private:
	void SetActive_ModeWindow();
	void Enable_StageHighlight(_float4 vPos);
	void Enable_StageClickRect(_float4 vPos);

private:
	void Create_PlayBtn();
	void Create_ModeBG();
	void Create_StageBtn();
	void Create_LockImg();
	void Create_StageHighlight();
	void Create_BtnHighlight();
	void Create_StageNameRect();
	void Crerate_PlayBtnMouseEnterLine();
	void Create_SelectTextRect();

private:
	_bool m_bIsEnable = false;
	_float m_fEnableTime = 0.f;
	_float m_fEnableMaxTime = 0.1f;
	void Late_Enable();
};