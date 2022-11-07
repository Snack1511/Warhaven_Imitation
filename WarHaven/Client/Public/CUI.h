#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
protected:
	CUI() = default;
	CUI(const CUI& Prototype);
	virtual ~CUI() = default;

public:
	virtual CGameObject* Clone() PURE;

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize() PURE;
	virtual HRESULT	Start();
	virtual void	Tick();
	virtual void	Late_Tick();

protected:	// Component
	CMesh* m_pMesh = nullptr;
	CModel* m_pModel = nullptr;


protected:

private:

private:	
};

END