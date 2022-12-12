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

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	// ���	 ���� ��׶��� 0 1	 ���� 0 1	 ����
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

private:
	void Create_ResultUI();
};

END