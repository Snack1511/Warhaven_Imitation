#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Result : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Result);
	DECLARE_GAMEOBJECT(CUI_Result);

private:
	CUI_Result();
	virtual ~CUI_Result();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	// 0 ���º�, 1 �¸�, 2 �й�
	void SetActive_Result(_uint iResult, _bool value);
	void SetActive_Result(_bool value);

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	enum ResultUI 
	{ 
		Result_BG, 
		Result_TextBG0, 
		Result_TextBG1, 
		Result_Text0,
		Result_Text1, 
		Result_Line,
		Result_End 
	};

	CUI_Object* m_pResultUI[Result_End];

	_bool m_bLerpLine = false;
	_bool m_bLerpText0 = false;
	_bool m_bLerpText1 = false;

private:
	void Create_ResultUI();
};

END