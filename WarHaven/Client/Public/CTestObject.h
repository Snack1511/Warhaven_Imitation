#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)
class CTestObject
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTestObject);
	DECLARE_GAMEOBJECT(CTestObject);

private:
	CTestObject();
	virtual ~CTestObject();

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;


};
END
