#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Client)
class CUI_Fire : public CGameObject
{
	DECLARE_GAMEOBJECT(CUI_Fire);
	DECLARE_PROTOTYPE(CUI_Fire);

private:
	CUI_Fire();
	virtual ~CUI_Fire();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start();

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();

private:
	CGameObject* m_pFire = nullptr;



};

END
