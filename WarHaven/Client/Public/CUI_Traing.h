#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Traing : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Traing);
	DECLARE_GAMEOBJECT(CUI_Traing);

private:
	CUI_Traing();
	virtual ~CUI_Traing();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick() override;
};

END