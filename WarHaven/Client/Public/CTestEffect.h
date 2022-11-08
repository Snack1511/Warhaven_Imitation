#pragma once
#include "CEffect.h"
#include "Client_Defines.h"

BEGIN(Engine)

END

BEGIN(Client)
class CTestEffect final
	: public CEffect
{
	DECLARE_PROTOTYPE(CTestEffect);

private:
	CTestEffect();
	virtual ~CTestEffect();

public:
	static CTestEffect* Create(wstring wstrModelPath, wstring wstrMaskPath, wstring wstrColorPath, wstring wstrNoisePath);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

};
END

