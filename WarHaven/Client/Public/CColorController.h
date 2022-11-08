#pragma once
#include "Component.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CMeshContainer;
END

BEGIN(Client)
class CColorController 
	: public CComponent
{
public:
	DECLARE_PROTOTYPE(CColorController);

private:
	CColorController(_uint iGroupIdx);
	~CColorController();

public:
	static CColorController* Create(_uint iGroupIdx);

public:
	void Set_ColorControll(_float4 vStartColor, _float4 vEndColor, _float EndTime);

public:
	// CComponent을(를) 통해 상속
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDead()	override;

private:
	_float4 m_vStartColor = {};
	_float4 m_vEndColor = {};
	_float4 m_vFadeInColor = {};
	_float4 m_vFadeOutColor = {};
	_float m_fEndTime = 0.f;
	_float m_fTimeAcc = 0.f;
	_float m_fFadeinTime = 0.f;
	_float m_fFadeOutTime = 0.f;
	_uint m_iMeshPartType = 0;

private:

	void LerpColor();
};
END
