#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Interact : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Interact);
	DECLARE_GAMEOBJECT(CUI_Interact);

private:
	CUI_Interact();
	virtual ~CUI_Interact();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_InteractKey(_uint iKeyIndex);
	void Set_InteractText(wstring wstrText);
	void Set_InteractTarget(CGameObject* pTargetTransform);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	CUI_Object* m_pInteractUI = nullptr;
	CTransform* m_pTargetTransform = nullptr;

	CUI_Object* m_pCircleEffect[3];

	_bool m_bEnableCircleEffect = false;
	_uint m_iCircleEffectIdx = 0;

private:
	void Create_InteractUI();
	void Create_CircleEffect();
};

END