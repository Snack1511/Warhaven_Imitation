#pragma once
#include "CUI_Wrapper.h"
BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Portrait);
	DECLARE_GAMEOBJECT(CUI_Portrait);

private:
	CUI_Portrait();
	CUI_Portrait(const CUI_Portrait& Prototype);
	virtual ~CUI_Portrait();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Portrait(_uint iIndex);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

private:
	void Get_Texture();
};

END