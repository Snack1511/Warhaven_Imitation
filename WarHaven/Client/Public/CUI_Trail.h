#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CHierarchyNode;
class CGameObject;
class CTransform;
class CShader;
END

BEGIN(Client)
class CCustomTrail;

class CUI_Trail
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Trail);

private:
	CUI_Trail();
	virtual ~CUI_Trail();

public:
	static CUI_Trail* Create(_uint iGroupIdx, _uint iTriCnt, _float fWide, _float fUVSPeed, _float fUVPower, _float4 vGlowFlag, _float4 vColor,
		wstring wstrMaskMapPath, wstring wstrColorMapPath);

public:
	void		Set_ShaderResource(CShader* pShader, const char* pConstantName);
	void	Set_EffectFlag(_float4 vEffectFlag) { m_vShaderFlag = vEffectFlag; }

public:
	void Add_Node(const _float4& vNode);
	void Clear_Nodes() { m_ListNodes.clear(); };
	void ReMap_TrailBuffers();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	HRESULT	SetUp_TrailEffect(_uint iGroupIdx, _uint iTriCnt, _float fWide, _float fUVSPeed, _float fUVPower, _float4 vGlowFlag,
		_float4 vColor, wstring wstrMaskMapPath, wstring wstrColorMapPath);

protected:
	virtual void	My_LateTick() override;

private:
	list<_float4>	m_ListNodes;
	_float		m_fUVSpeed = 0.f;

	_float		m_fUVPower = 0.f;

	_float		m_fCurUV = 0.f;
	_float		m_fWide = 0.f;

private:
	_float4		m_vColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float4		m_vShaderFlag = SH_EFFECT_NOBLOOM;
	_float4		m_vGlowFlag = GLOW_CHAKRA(1.f);

private:
	CCustomTrail* m_pCustomTrailCom = nullptr;
	_float		m_fTimeAcc = 0.f;


};

END