#pragma once
#include "CEffect.h"
#include "Client_Defines.h"

BEGIN(Engine)

END

BEGIN(Client)
class CDefault_Effect final
	: public CEffect
{
	DECLARE_PROTOTYPE(CDefault_Effect);

private:
	CDefault_Effect();
	virtual ~CDefault_Effect();

public:
	static CDefault_Effect* Create();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

};
END
