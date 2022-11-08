#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CShader;
END

BEGIN(Client)

class CUI_Object;

class CUI_Wrapper final : public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Wrapper);

private:
	CUI_Wrapper();
	CUI_Wrapper(const CUI_Wrapper& Prototype);
	virtual ~CUI_Wrapper();

public:
	static CUI_Wrapper* Create(wstring pUIName);

public:
	void	Set_ShaderResources(CShader* pShaderCom, const char* pConstantName);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	wstring	m_wstrUIName;
	CUI_Object* m_pUI = nullptr;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END