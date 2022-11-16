#include "stdafx.h"
#include "CEffect.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "CNavigation.h"
#include "Texture.h"

#include "Transform.h"
//#include "CComponent_Factory.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"

#include "CDefault_Effect.h"
#include "Functor.h"
#include "CCollider_Sphere.h"
#include "CRectEffects.h"
#include "CUtility_Transform.h"



CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create_Effect_FromBinFile(string strFileKey)
{
	CEffect* pNewEffect = nullptr;

	string savePath = "../bin/effects/";
	savePath += strFileKey;
	savePath += ".bin";

	ifstream	readFile(savePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Load 실패 ??!?!");
		return nullptr;
	}
	EFFECT_TYPE	eType = CEffect::MESH;
	readFile.read((char*)&eType, sizeof(_uint));

	switch (eType)
	{
	case Client::CEffect::MESH:
		pNewEffect = CDefault_Effect::Create(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
			SAFE_DELETE(pNewEffect);
			return nullptr;
		}

		{
			if (FAILED(pNewEffect->CEffect::Initialize_Prototype()))
			{
				Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
				SAFE_DELETE(pNewEffect);
				return nullptr;
			}

			CModel* pModelCom = GET_COMPONENT_FROM(pNewEffect, CModel);
			//Roughness : 노이즈
			pModelCom->Change_Texture(0, aiTextureType_REFLECTION, pNewEffect->m_wstrMaskMapPath);
			pModelCom->Change_Texture(0, 1, pNewEffect->m_wstrColorMapPath);
			pModelCom->Change_Texture(0, aiTextureType_DIFFUSE_ROUGHNESS, pNewEffect->m_wstrNoiseMapPath);

		}

		break;

	case Client::CEffect::PARTICLE:
		pNewEffect = CRectEffects::Create(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create : CRectEffects");
			return nullptr;
		}
		break;

	case Client::CEffect::ANIMPARTICLE:
		pNewEffect = CRectEffects::Create_Anim(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create : CRectEffects");
			return nullptr;
		}
		break;

	case Client::CEffect::EFFECT_END:
		break;

	default:
		break;
	}

	readFile.close();
	pNewEffect->m_hcMyCode = Convert_ToHash(CFunctor::To_Wstring(strFileKey));


	return pNewEffect;
}

void CEffect::Self_Reset(CGameObject* pGameObject, _float4 vStartPos)
{
	m_pFollowTarget = pGameObject;
	Reset(pGameObject->Get_Transform()->Get_WorldMatrix());
}

void CEffect::Self_Reset_Turn(CGameObject* pGameObject, _float4 vStartPos)
{
	m_pFollowTarget = pGameObject;
	m_fTurnAngle = frandom(0.f, 180.f);
	Reset(vStartPos);
}

void CEffect::Reset(_float4 vStartPos)
{
	_float4 vPos = vStartPos + m_vOffsetPos;

	m_pTransform->Set_World(WORLD_POS, vPos);
	m_pTransform->Make_WorldMatrix();
	ENABLE_GAMEOBJECT(this);
}

void CEffect::Reset(CGameObject* pFollowTarget)
{
	m_pFollowTarget = pFollowTarget;
	ENABLE_GAMEOBJECT(this);
}

void CEffect::Reset(_float4 vStartPos, _float4 vTargetPosition)
{
	m_vTargetPosition = vTargetPosition;
	Reset(vStartPos);
}

void CEffect::Reset(_float4 vStartPos, _float4x4 matTrans)
{
	m_matTrans = matTrans;
	Reset(vStartPos);
}

void CEffect::Reset(_float4x4 matWorld)
{
	m_pTransform->Get_Transform().matMyWorld = matWorld;
	_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(matWorld);
	m_pTransform->Set_World(WORLD_POS, vOffsetPos);

	m_pTransform->Make_WorldMatrix();
	ENABLE_GAMEOBJECT(this);

}

void CEffect::ReBind_Texture()
{
	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_REFLECTION, m_wstrMaskMapPath);
	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_DIFFUSE, m_wstrColorMapPath);
	GET_COMPONENT(CModel)->Change_Texture(0, aiTextureType_DIFFUSE_ROUGHNESS, m_wstrNoiseMapPath);
}

void CEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType)
{
	if (!(m_bEffectFlag & EFFECT_COLLIDER))
		DISABLE_GAMEOBJECT(this);
}

void CEffect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
	pShader->Set_RawValue("g_vFlag", &m_vEffectFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pShader->Set_RawValue("g_fColorPower", &m_fColorPower, sizeof(_float));
	pShader->Set_RawValue("g_vPlusColor", &m_vPlusColor, sizeof(_float4));
	pShader->Set_RawValue("g_fDissolvePower", &m_fDissolvePower, sizeof(_float));
}

void CEffect::Set_ColliderOn(_float fRadius, COL_GROUP_CLIENT eColType)
{
	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	GET_COMPONENT(CCollider_Sphere)->Get_ColInfo().front().fRadius = fRadius;
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(eColType);
	m_bColliderOn = true;
	m_fColliderAcc = 0.f;
}


HRESULT CEffect::Start()
{
	__super::Start();

	GET_COMPONENT(CModel)->Set_ShaderPassToAll(m_iPassType);

	if (m_eShaderType == SHADER_VTXEFFECTS)
	{
		if (m_iPassType == VTXEFFECT_PASS_DISTORTION)
		{
			GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_DISTORTION);
		}
	}

	SHADER_BINDING(CEffect, "g_fUVPlus");

	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectFlag);

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, placeholders::_2);


	CallBack_CollisionEnter += bind(&CEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2);




	return S_OK;
}

HRESULT CEffect::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, m_eShaderType,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = nullptr;
	if (m_iPassType == VTXEFFECT_PASS_DISTORTION && m_eShaderType == SHADER_VTXEFFECTS)
	{
		pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_DISTORTION, m_iPassType
			, _float4(0.f, 0.f, 0.f, 1.f));
	}
	else
	{
		pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, m_iPassType
			, _float4(0.f, 0.f, 0.f, 1.f));
	}

	

	Add_Component<CRenderer>(pRenderer);
	Add_Component(CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, m_wstrPath.c_str(), m_matTrans));

	Add_Component(CPhysics::Create(CP_BEFORE_RENDERER));
	/*if (m_bEffectFlag & EFFECT_COLLIDER)
	{
		CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
		pCollider->Get_ColInfo().fRadius = m_vFadeInTargetScale.x;
		Add_Component(pCollider);
	}*/

	//pCollider->Initialize();

	return S_OK;
}

HRESULT CEffect::Initialize()
{
	m_pTransform->Set_Scale(m_vStartScale);

	return S_OK;
}

void CEffect::Update_FollowTarget()
{
	_float4 vLook = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK);
	_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

	_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(m_pFollowTarget->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
	vPos += vOffsetPos;

	m_pTransform->Set_Look(vLook);
	m_pTransform->Set_World(WORLD_POS, vPos);

	/*if (m_fTurnAngle > 0.f)
	{
		CUtility_Transform::Turn_ByAngle(m_pTransform, vLook, m_fTurnAngle);
	}*/
	m_pTransform->Make_WorldMatrix();
}

void CEffect::Update_TargetPos()
{
	On_Target();
}

void CEffect::On_Target()
{
	_float fDistance = (m_vTargetPosition - m_pTransform->Get_World(WORLD_POS)).Length();

	if (fDistance <= 0.2f)
	{
		m_eCurFadeType = FADEOUT;
		GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	}
	else
		GET_COMPONENT(CPhysics)->Set_Accel(100.f);

}

void CEffect::Stick_RefBone()
{
	_float4 vBonePos = m_vOffsetPos;

	_float4x4 BoneMatrix = m_pRefBone->Get_BoneMatrix();

	//콤바인 위치 * 월드매트릭스
	vBonePos = vBonePos.MultiplyCoord(BoneMatrix);

	//vBonePos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

	m_pTransform->Get_Transform().matMyWorld = BoneMatrix;
	m_pTransform->Set_World(WORLD_POS, vBonePos);
	m_pTransform->Make_WorldMatrix();
}

void CEffect::My_Tick()
{
	if (m_bColliderOn)
	{
		m_fColliderAcc += fDT(0);
		if (m_fColliderAcc > m_fColliderMaxTime)
		{
			m_bColliderOn = false;
			m_fColliderAcc = 0.f;
			DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
		}
	}

	if (m_bEffectFlag & EFFECT_TARGETPOS)
	{
		On_Target();
	}
}

void CEffect::My_LateTick()
{
	UPdate_Turn();

	if (m_bEffectFlag & EFFECT_REFBONE)
	{
		Stick_RefBone();
	}

	//Follow


	else if (m_bEffectFlag & EFFECT_FOLLOWTARGET)
	{
		Update_FollowTarget();
	}

	//UV
	m_fCurUVPlusY += m_fUVSpeedY * fDT(0);
	m_fCurUVPlusX += m_fUVSpeedX * fDT(0);

	Update_Disable();

	if (fabs(m_fCurUVPlusX) >= 1.f)
		m_fCurUVPlusX = 0.f;
	if (fabs(m_fCurUVPlusY) >= 1.f)
		m_fCurUVPlusY = 0.f;

	Update_Fade();

	if (m_bEffectFlag & EFFECT_BILLBOARD)
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		m_pTransform->Set_Look(vLook * -1.f);
		m_pTransform->Make_WorldMatrix();
	}
}

void CEffect::OnDisable()
{
	__super::OnDisable();
	CEffects_Factory::Get_Instance()->Collect_Effect(m_hcMyCode, this);
	m_fTimeAcc = 0.f;
	m_fCurUVPlusX = 0.f;
	m_fCurUVPlusY = 0.f;
	m_fFadeTimeAcc = 0.f;
	m_fTurnAngle = 0.f;

	if (m_eDisableType == FADE)
		m_fAlpha = 0.f;
	m_eCurFadeType = FADEINREADY;
	/*if (m_fUVSpeed > 0.f)
		SHADER_BINDINGOUT(CEffect, "g_fUVPlus");*/
}

void CEffect::Update_Disable()
{
	switch (m_eDisableType)
	{
	case Client::CEffect::NONE:
		m_fAlpha = 1.f;
		break;
	default:
		break;
	}
}

void CEffect::Update_Fade()
{
	if (m_eDisableType != FADE)
		return;

	m_fFadeTimeAcc += fDT(0);

	switch (m_eCurFadeType)
	{
	case FADEINREADY:
		if (m_fFadeTimeAcc >= m_fFadeInStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEIN;
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
			m_fCurUVPlusX = 0.f;
			m_fCurUVPlusY = 0.f;
		}
		else
		{
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
		}


		break;

	case FADEIN:
		if (m_fFadeTimeAcc >= m_fFadeInTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUTREADY;
			m_pTransform->Set_Scale(m_vFadeInTargetScale);
			m_fAlpha = m_fTargetAlpha;
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeInTime;
			_float4 vScale =
				XMVectorLerp(m_vStartScale.XMLoad(), m_vFadeInTargetScale.XMLoad(), fRatio);

			m_fAlpha = m_fTargetAlpha * fRatio;
			m_pTransform->Set_Scale(vScale);

		}

		break;

	case FADEOUTREADY:
		if (m_fFadeTimeAcc >= m_fFadeOutStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUT;
		}

		break;

	case FADEOUT:
		if (m_fFadeTimeAcc >= m_fFadeOutTime)
		{
			DISABLE_GAMEOBJECT(this);
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeOutTime;
			_float4 vScale =
				XMVectorLerp(m_vFadeInTargetScale.XMLoad(), m_vFadeOutTargetScale.XMLoad(), sqrtf(fRatio));

			m_fAlpha = m_fTargetAlpha * (1.f - fRatio);
			m_pTransform->Set_Scale(vScale);

		}

		break;

	default:
		break;
	}

	m_pTransform->Make_WorldMatrix();


}

void CEffect::UPdate_Turn()
{
	if (m_bRotation)
		CUtility_Transform::Rotation(m_pTransform, m_vRotationDir.Normalize(), m_fAngle);

	CUtility_Transform::Turn(m_pTransform, m_vTurnDir.Normalize(), m_fTurnSpeed);

	m_pTransform->Make_WorldMatrix();

}
