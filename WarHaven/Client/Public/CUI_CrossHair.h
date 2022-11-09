#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Crosshair final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Crosshair);
	DECLARE_GAMEOBJECT(CUI_Crosshair);

private:
	CUI_Crosshair();
	CUI_Crosshair(const CUI_Crosshair& Prototype);
	virtual ~CUI_Crosshair();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	map<wstring, CUI_Object*> m_pUIMap;

private:
	// 여기에 불러올 유아이 종류 받아와서 부모에 넣자
	void Read_File();
};

END