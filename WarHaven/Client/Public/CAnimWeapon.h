#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CAnimator;
class CHierarchyNode;
END


BEGIN(Client)

class CUnit;

class CAnimWeapon
	: public CGameObject
{
	DECLARE_PROTOTYPE(CAnimWeapon);

protected:
	CAnimWeapon();
	virtual ~CAnimWeapon();

public:
	static CAnimWeapon* Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName);

public:
	enum eAnimState {eIDLE, eATTACKBEGIN, eATTACKLOOP, eATTACKCANCEL, eATTACKLAUNCH, eCNT};

public:
	void	Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);

public:
	_float4x4& Use_OwnerBoneOffset() { return m_OwnerBoneOffsetMatrix; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

private:
	CUnit* m_pOwnerUnit = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CHierarchyNode* m_pOwnerBone = nullptr;
	eAnimState	m_eCurState = eIDLE;

private:
	_float4x4	m_OwnerBoneOffsetMatrix;

private:
	HRESULT		SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName);

private:
	virtual void	Late_Tick() override;
};

END