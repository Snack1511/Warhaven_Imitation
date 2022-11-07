#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
class CShader;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)
class CEffect abstract
	: public CGameObject
{
	
protected:
	CEffect();
	virtual ~CEffect();

public:
	void		Set_ShaderResource(CShader* pShader, const char* pConstantName);
	void		Set_EffectFlag(_float4 vEffectFlag);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

protected:
	wstring m_wstrModelFilePath;
	wstring m_wstrMaskMapFilePath;	//마스크맵 텍스쳐 경로
	wstring m_wstrColorMapFilePath;	//컬러맵 텍스쳐 경로
	wstring m_wstrNoiseMapFilePath;	//노이즈맵 텍스쳐 경로

protected:
	SHADER_FILE_ID	m_eShaderType = SHADER_VTXEFFECTS;
	VTXEFFECT_PASS_TYPE m_eEffectPassType = VTXEFFECT_PASS_DEFAULT;
	_float4		m_vEffectShaderFlag = SH_EFFECT_DEFAULT;
	_float4		m_vGlowFlag = _float4(1.f, 1.f, 1.f, 1.f);

	

protected:
	_float	m_fAlpha = 1.f;
	_float	m_fCurUVPlusX = 0.f;
	_float	m_fCurUVPlusY = 0.f;
	_float	m_fTimeAcc = 0.f;


	_float	m_fUVSpeedX = 0.f;
	_float	m_fUVSpeedY = 0.f;
	void Push_UV();

protected:
	_float4		m_vTurnDir = _float4(0.f, 0.f, 0.f, 0.f);
	_float		m_fTurnSpeed = 0.f;

protected:
	CGameObject* m_pFollowTarget = nullptr;
	_float4		m_vOffsetPos;
	//void	Update_FollowTarget();

protected:
	_float		m_fMoveSpeed = 0.f;
	_float4		m_vTargetPosition;
	//void	Update_TargetPos();
	//virtual void	On_Target();

protected:
	CHierarchyNode* m_pRefBone = nullptr;
	//virtual void	Stick_RefBone();

protected:
	enum FADETYPE
	{
		FADEINREADY,
		FADEIN,
		FADEOUTREADY,
		FADEOUT,
		FADE_END,
	};
	FADETYPE	m_eCurFadeType = FADEINREADY;
	_float	m_fFadeTimeAcc = 0.f;
	_float	m_fFadeInStartTime = 0.f;
	_float	m_fFadeInTime = 0.f;
	_float	m_fFadeOutStartTime = 9999.f;
	_float	m_fFadeOutTime = 0.f;

	_float4	m_vStartScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float4	m_vFadeInTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float4	m_vFadeOutTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float	m_fTargetAlpha = 1.f;



protected:
	CModel* m_pModelCom = nullptr;


protected:
	HRESULT SetUp_Model(wstring pFilePath);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

protected:
	void Update_Fade();

};
END
