#pragma once
#include "Component.h"

BEGIN(Engine)
class CGameObject;
class CShader;

enum WORLD {WORLD_RIGHT, WORLD_UP, WORLD_LOOK, WORLD_POS, WORLD_END};

class ENGINE_DLL CTransform final
	: public CComponent
{
	DECLARE_PROTOTYPE(CTransform);

private:
	CTransform(_uint iGroupID);
	virtual ~CTransform();

public:
	static CTransform* Create(_uint iGroupID);

public:
	TRANSFORM&			Get_Transform()		{ return m_tTransform;}
	_float4x4			Get_WorldMatrix(const _byte& matrixFlag = 0);
	_float4				Get_World(WORLD eType);
	_float4				Get_MyWorld(WORLD eType);
	_float4				Get_Scale() { return m_tTransform.vScale; }

	void				Set_World(WORLD eType, const _float4& vCol);
	void				Set_RealWorld(WORLD eType, const _float4& vCol);
	void				Set_MyMatrix(_float4x4 matWorld);
	void				Set_Look(const _float4& vLook);
	void				Set_Right(const _float4& vRight);
	void				Set_Up(const _float4& vUp);
	void				Set_Rect();

	void				Set_Scale(const _float4& vScale);
	void				Set_Y(const _float& fY);
	void				Set_ParentFlag(_byte parentFlag)			{ m_cParentFlag = parentFlag; }

public:
	virtual void		Set_ShaderResource(CShader* pShader, const char* pConstantName) override;

public:
	virtual void		OnCollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint) override;

public:
	void				Set_LerpLook(_float4 vLook, _float fMaxLerpTime);

public:
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void			Make_WorldMatrix();
	void			Rescale_WorldMatrix();

private:
	TRANSFORM	m_tTransform;
	_byte		m_cParentFlag = 0;

	_bool		m_bLerp = false;
	_float		m_fTimeAcc = 0.f;
	_float		m_fLerpTime = 0.f;
	_float4		m_vOriginLook;
	_float4		m_vTargetLook;
};
END
