#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)
class CProjectile abstract
	: public CGameObject
{
protected:
	CProjectile();
	virtual ~CProjectile();

public:
	virtual HRESULT	Initialize_Protoype();
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();



};

END