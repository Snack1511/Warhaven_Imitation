#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CFader;

class CFadeDark
	: public CGameObject
{
	DECLARE_PROTOTYPE(CFadeDark);
	DECLARE_GAMEOBJECT(CFadeDark);

private:
	CFadeDark();
	virtual ~CFadeDark();

public:
	void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void	Start_Fade(_float fFadeInTime, _float fFadeOutStartTime, _float fFadeOutTime);

private:
	CFader* m_pFader = nullptr;

};

END