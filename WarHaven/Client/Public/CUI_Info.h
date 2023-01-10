#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Info : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Info);
	DECLARE_GAMEOBJECT(CUI_Info);

private:
	CUI_Info();
	virtual ~CUI_Info();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	LEVEL_TYPE_CLIENT m_eLoadLevel = LEVEL_END;

	_float fPosX = 450.f;

	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);

private:
	enum Info { Info_BG, Info_Line, Info_Name, Info_Mode, Info_End };
	CUI_Object* m_pInfoUI[Info_End];

	enum Info_Text { Text_0, Text_1, Text_2, Text_End };
	CUI_Object* m_pInfoText[Text_End];

private:
	void Create_InfoUI();
	void Create_InfoText();

	void Init_InfoUI();
	void Init_InfoText();
};

END