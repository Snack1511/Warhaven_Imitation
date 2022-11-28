#include "stdafx.h"
#include "CRectEffects.h"


#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "Texture.h"

#include "Transform.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"

#include "CRect_Instance.h"

#include "CInstanceMesh.h"
#include "CUtility_File.h"
#include "Functor.h"

#include "HIerarchyNode.h"
#include "CUser.h"




CRectEffects::CRectEffects()
{
}

CRectEffects::CRectEffects(const CRectEffects& _origin)
	: CInstancingEffects(_origin)
	, m_bBillBoard(_origin.m_bBillBoard)
	, m_bSorting(_origin.m_bSorting)
	, m_bZeroSpeedDisable(_origin.m_bZeroSpeedDisable)
	, m_bLoop(_origin.m_bLoop)
	, m_fDuration(_origin.m_fDuration)
	, m_fLoopTime(_origin.m_fLoopTime)
	, m_iWidthSize(_origin.m_iWidthSize)
	, m_iHeightSize(_origin.m_iHeightSize)
	, m_bPlayOnce(_origin.m_bPlayOnce)
	, m_eCurveType(_origin.m_eCurveType)
	, m_bBlackBackGround(_origin.m_bBlackBackGround)
	, m_fDurationRange(_origin.m_fDurationRange)
	, m_strBoneName(_origin.m_strBoneName)
	, m_fDiscardPower(_origin.m_fDiscardPower)
	, m_bKeepSticked(_origin.m_bKeepSticked)
{
	if (_origin.m_pDatas)
	{
		m_pDatas = new DATAS[m_tCreateData.iNumInstance];
		memcpy(m_pDatas, _origin.m_pDatas, sizeof(DATAS) * m_tCreateData.iNumInstance);
	}

	if (_origin.m_pFinalRectInstances)
	{
		m_pFinalRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
		memcpy(m_pFinalRectInstances, _origin.m_pFinalRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);
	}

}

CRectEffects::~CRectEffects()
{
	Release();
}

CRectEffects* CRectEffects::Create(_uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, wstring wstrTexturePath,
	_hashcode _hcCode, _bool bBillBoard, _bool bSorting, _bool bZeroSpeedDisable, _bool bLoop, _bool bPlayOnce)
{
	CRectEffects* pInstance = new CRectEffects();

	pInstance->m_wstrPath = wstrTexturePath;
	pInstance->m_tCreateData.iNumInstance = iNumInstance;
	pInstance->m_tCreateData = tCreateData;
	pInstance->m_hcMyCode = _hcCode;
	pInstance->m_bBillBoard = bBillBoard;
	pInstance->m_bSorting = bSorting;
	pInstance->m_bZeroSpeedDisable = bZeroSpeedDisable;
	pInstance->m_bLoop = bLoop;
	pInstance->m_bPlayOnce = bPlayOnce;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CRectEffects* CRectEffects::Create(_float4 vStartPos)
{
	CRectEffects* pInstance = new CRectEffects();

	pInstance->Get_Transform()->Set_World(WORLD_POS, vStartPos);
	pInstance->Get_Transform()->Make_WorldMatrix();


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CRectEffects* CRectEffects::Create(ifstream* pReadFile)
{
	CRectEffects* pInstance = new CRectEffects;

	if (FAILED(pInstance->SetUp_RectEffects(pReadFile)))
	{
		Call_MsgBox(L"Failed to SetUp_RectEffects : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CRectEffects* CRectEffects::Create_Anim(ifstream* pReadFile)
{
	CRectEffects* pInstance = new CRectEffects;

	if (FAILED(pInstance->SetUp_RectEffects_Anim(pReadFile)))
	{
		Call_MsgBox(L"Failed to SetUp_RectEffects_Anim : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CRectEffects::Self_Reset(CGameObject* pGameObject, _float4 vStartPos)
{
	//__super::Self_Reset(pGameObject, vStartPos);
	CEffect::Reset(vStartPos);
	m_pFollowTarget = pGameObject;
	//이거 파티클 빌보드 잘 안대는거 확인중
	m_matTrans = pGameObject->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);

	if (m_bEffectFlag & EFFECT_REFBONE)
	{
		if (m_pFollowTarget)
		{
			m_pRefBone = GET_COMPONENT_FROM(m_pFollowTarget, CModel)->Find_HierarchyNode(m_strBoneName.c_str());
		}
	}

	//if (m_bBillBoard)
	//{
	//	m_matTrans = pGameObject->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);
	//}
	//else
	//	m_matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);

}

void CRectEffects::Reset(_float4x4 matWorld)
{
	m_matTrans = matWorld;

	CEffect::Reset(_float4(m_matTrans.m[3][0], m_matTrans.m[3][1], m_matTrans.m[3][2]));

	m_matTrans.m[3][0] = 0.f;
	m_matTrans.m[3][1] = 0.f;
	m_matTrans.m[3][2] = 0.f;
}


void CRectEffects::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
		m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR ||
		m_iPassType == VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
	{
		pShader->Set_RawValue("g_iWidthSize", &m_iWidthSize, sizeof(_uint));
		pShader->Set_RawValue("g_iHeightSize", &m_iHeightSize, sizeof(_uint));
		pShader->Set_RawValue("g_bBlackBG", &m_bBlackBackGround, sizeof(_bool));
		pShader->Set_RawValue("g_fDissolvePower", &m_fDissolvePower, sizeof(_float));
	}

	pShader->Set_RawValue("g_fDiscardPower", &m_fDiscardPower, sizeof(_float));

	__super::Set_ShaderResource(pShader, pConstantName);
}


HRESULT CRectEffects::Initialize_Prototype()
{
	m_eEffectType = CEffect::PARTICLE;

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXRECTINSTANCE,
		VTXRECT_INSTANCE_DECLARATION::Element, VTXRECT_INSTANCE_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_ALPHA, VTXRECTINSTANCE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	Add_Component<CMesh>(CRect_Instance::Create(m_tCreateData.iNumInstance));

	if (m_wstrMaskMapPath.empty())
		m_wstrMaskMapPath = L"../bin/resources/textures/T_EFF_Gradation_10_M.png";

	Add_Component<CTexture>(CTexture::Create(0, m_wstrMaskMapPath.c_str(), 1));


	if (m_wstrColorMapPath.empty())
		m_wstrColorMapPath = L"../bin/resources/textures/White.png";

	Add_Component<CTexture>(CTexture::Create(0, m_wstrColorMapPath.c_str(), 1));

	if (m_wstrNoiseMapPath.empty())
		m_wstrNoiseMapPath = L"../bin/resources/textures/White.png";

	Add_Component<CTexture>(CTexture::Create(0, m_wstrNoiseMapPath.c_str(), 1));

	
	m_pDatas = new DATAS[m_tCreateData.iNumInstance];
	ZeroMemory(m_pDatas, sizeof(DATAS) * m_tCreateData.iNumInstance);

	m_pFinalRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
	ZeroMemory(m_pFinalRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);

	m_matTrans.Identity();
	//m_bBillBoard = true;

	//m_bEffectFlag = 0;

	return S_OK;
}

HRESULT CRectEffects::Initialize()
{
	m_fTimeAcc = 0.f;
	m_iNumDead = 0;
	//m_bSorting = true;

	GET_COMPONENT(CRenderer)->Set_Pass(m_iPassType);

	for (_uint i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		m_pDatas[i].InstancingData.eCurFadeType = INSTANCING_DATA::FADEINREADY;

		_float4 vStartPos;
		vStartPos = _float4(0.f, 0.f, 0.f, 1.f);

		_float4 vStartDir = _float4(
			frandom(-m_tCreateData.vStartDirRange.x, m_tCreateData.vStartDirRange.x),
			frandom(-m_tCreateData.vStartDirRange.y, m_tCreateData.vStartDirRange.y),
			frandom(-m_tCreateData.vStartDirRange.z, m_tCreateData.vStartDirRange.z),
			0.f);

		vStartDir += m_tCreateData.vStartDir;
		vStartDir.Normalize();

		_float4 vMoveDir = _float4(
			frandom(-m_tCreateData.vMoveDirRange.x, m_tCreateData.vMoveDirRange.x),
			frandom(-m_tCreateData.vMoveDirRange.y, m_tCreateData.vMoveDirRange.y),
			frandom(-m_tCreateData.vMoveDirRange.z, m_tCreateData.vMoveDirRange.z),
			0.f);


		vMoveDir += m_tCreateData.vMoveDir;
		vMoveDir.Normalize();

		_float	fStartDistance = m_tCreateData.fStartDistance + frandom(-m_tCreateData.fStartDistanceRange, m_tCreateData.fStartDistanceRange);

		vStartPos = vStartDir * fStartDistance;

		m_pDatas[i].InstancingData.vStartPureLocalPos = vStartPos;
		m_pDatas[i].InstancingData.vStartPureLocalDir = vMoveDir;



		_float4 vUpDir = { 0.f, 1.f, 0.f };
		if ((vMoveDir.y < 1.1f && vMoveDir.y > 0.9f) ||
			(vMoveDir.y > -1.1f && vMoveDir.y < -0.9f)
			)
			vUpDir = _float4(0.f, 0.f, 1.f, 0.f);

		vUpDir.Normalize();
		m_pDatas[i].InstancingData.vStartPureLocalRight = vUpDir.Cross(vMoveDir);

		_float4x4 matCurveRot;

		_float fStartCurveAngle = m_tCreateData.fCurveAngle + frandom(-m_tCreateData.fCurveAngleRange, m_tCreateData.fCurveAngleRange);
		matCurveRot = XMMatrixRotationAxis(m_pDatas[i].InstancingData.vStartPureLocalDir.XMLoad(), ToRadian(fStartCurveAngle));
		m_pDatas[i].InstancingData.vStartPureLocalRight = m_pDatas[i].InstancingData.vStartPureLocalRight.MultiplyNormal(matCurveRot);
		m_pDatas[i].InstancingData.fCurvePower = m_tCreateData.fCurvePower + frandom(-m_tCreateData.fCurvePowerRange, m_tCreateData.fCurvePowerRange);
		m_pDatas[i].InstancingData.fCurveFrequency = m_tCreateData.fCurveFrequency + frandom(-m_tCreateData.fCurveFrequencyRange, m_tCreateData.fCurveFrequencyRange);

		if (CURVE_SPIRAL == m_eCurveType)
		{
			_float4x4 matRot;
			
			vUpDir = _float4(0.f, cosf(ToRadian(fStartCurveAngle)), sinf(ToRadian(fStartCurveAngle)));

			matRot = XMMatrixRotationAxis(vUpDir.XMLoad(),
				ToRadian(frandom(0.f, 360.f)));

			m_pDatas[i].InstancingData.vStartPureLocalDir = m_pDatas[i].InstancingData.vStartPureLocalDir.MultiplyNormal(matRot);
			m_pDatas[i].InstancingData.vStartPureLocalRight = m_pDatas[i].InstancingData.vStartPureLocalRight.MultiplyNormal(matRot);
		}


		Set_NewStartPos(i);

		m_pDatas[i].InstancingData.fSpeed = m_pDatas[i].InstancingData.fOriginSpeed = m_tCreateData.fSpeed + frandom(-m_tCreateData.fSpeedRange, m_tCreateData.fSpeedRange);
		m_pDatas[i].InstancingData.fTurnSpeed = m_tCreateData.fTurnSpeed + frandom(-m_tCreateData.fTurnSpeedRange, m_tCreateData.fTurnSpeedRange);
		m_pDatas[i].InstancingData.fJumpPower = m_tCreateData.fJumpPower + frandom(-m_tCreateData.fJumpPowerRange, m_tCreateData.fJumpPowerRange);
		m_pDatas[i].InstancingData.fGravity = m_tCreateData.fGravity + frandom(-m_tCreateData.fGravityRange, m_tCreateData.fGravityRange);
		m_pDatas[i].InstancingData.fCurGroundY = m_tCreateData.fCurGroundY + frandom(-m_tCreateData.fCurGroundYRange, m_tCreateData.fCurGroundYRange);
		m_pDatas[i].InstancingData.fCurGroundY = -100.f;
		m_pDatas[i].InstancingData.fSpeedChangeSpeed = m_tCreateData.fSpeedChangeSpeed + frandom(-m_tCreateData.fSpeedChangeSpeedRange, m_tCreateData.fSpeedChangeSpeedRange);
		m_pDatas[i].InstancingData.fTargetAlpha = m_tCreateData.fTargetAlpha;

		m_pDatas[i].InstancingData.fFadeInStartTime = m_tCreateData.fFadeInStartTime + frandom(-m_tCreateData.fFadeInStartTimeRange, m_tCreateData.fFadeInStartTimeRange);
		m_pDatas[i].InstancingData.fFadeInTime = m_tCreateData.fFadeInTime + frandom(-m_tCreateData.fFadeInTimeRange, m_tCreateData.fFadeInTimeRange);
		m_pDatas[i].InstancingData.fFadeOutStartTime = m_tCreateData.fFadeOutStartTime + frandom(-m_tCreateData.fFadeOutStartTimeRange, m_tCreateData.fFadeOutStartTimeRange);
		m_pDatas[i].InstancingData.fFadeOutTime = m_tCreateData.fFadeOutTime + frandom(-m_tCreateData.fFadeOutTimeRange, m_tCreateData.fFadeOutTimeRange);


		if (m_pDatas[i].InstancingData.fJumpPower > 0.f)
			m_pDatas[i].InstancingData.vDir.y = 0.f;

		m_pDatas[i].InstancingData.vColor = _float4(
			frandom(-m_tCreateData.vColorRange.x, m_tCreateData.vColorRange.x),
			frandom(-m_tCreateData.vColorRange.y, m_tCreateData.vColorRange.y),
			frandom(-m_tCreateData.vColorRange.z, m_tCreateData.vColorRange.z),
			0.f);
		m_pDatas[i].InstancingData.vColor += m_tCreateData.vColor;

		m_pDatas[i].InstancingData.vDir.Normalize();

		m_pDatas[i].InstancingData.vScale = _float4(
			frandom(-m_tCreateData.vScaleRange.x, m_tCreateData.vScaleRange.x),
			frandom(-m_tCreateData.vScaleRange.y, m_tCreateData.vScaleRange.y),
			frandom(-m_tCreateData.vScaleRange.z, m_tCreateData.vScaleRange.z),
			0.f);

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
			m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR ||
			m_iPassType == VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
		{
			m_pDatas[i].InstancingData.vScale.y = m_pDatas[i].InstancingData.vScale.x;
		}

		m_pDatas[i].InstancingData.vScale += m_tCreateData.vScale;

		if (m_pDatas[i].InstancingData.vScale.x <= 0.f)
			m_pDatas[i].InstancingData.vScale.x = 0.1f;
		if (m_pDatas[i].InstancingData.vScale.y <= 0.f)
			m_pDatas[i].InstancingData.vScale.y = 0.1f;
		if (m_pDatas[i].InstancingData.vScale.z <= 0.f)
			m_pDatas[i].InstancingData.vScale.z = 0.1f;

		m_pDatas[i].InstancingData.vStartScale = m_pDatas[i].InstancingData.vScale;

		m_pDatas[i].InstancingData.vFadeInTargetScale = _float4(
			frandom(-m_tCreateData.vFadeInTargetScaleRange.x, m_tCreateData.vFadeInTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.y, m_tCreateData.vFadeInTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.z, m_tCreateData.vFadeInTargetScaleRange.z),
			0.f);

		m_pDatas[i].InstancingData.vFadeInTargetScale += m_tCreateData.vFadeInTargetScale;
		if (m_pDatas[i].InstancingData.vFadeInTargetScale.x <= 0.f)
			m_pDatas[i].InstancingData.vFadeInTargetScale.x = 0.1f;
		if (m_pDatas[i].InstancingData.vFadeInTargetScale.y <= 0.f)
			m_pDatas[i].InstancingData.vFadeInTargetScale.y = 0.1f;
		if (m_pDatas[i].InstancingData.vFadeInTargetScale.z <= 0.f)
			m_pDatas[i].InstancingData.vFadeInTargetScale.z = 0.1f;

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION)
		{
			//m_pDatas[i].InstancingData.vFadeInTargetScale.y = m_pDatas[i].InstancingData.vFadeInTargetScale.x;
			m_pDatas[i].InstancingData.fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA)
		{
			//m_pDatas[i].InstancingData.vFadeInTargetScale.y = m_pDatas[i].InstancingData.vFadeInTargetScale.x;
			m_pDatas[i].InstancingData.fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);

			m_pDatas[i].InstancingData.fDissolveEndTime = m_iWidthSize * m_iHeightSize * m_fDuration; // 
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
		{
			//m_pDatas[i].InstancingData.vFadeInTargetScale.y = m_pDatas[i].InstancingData.vFadeInTargetScale.x;
			m_pDatas[i].InstancingData.fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);

			m_pDatas[i].InstancingData.fDissolveEndTime = m_iWidthSize * m_iHeightSize * m_fDuration; // 
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR)
		{
			//m_pDatas[i].InstancingData.vFadeInTargetScale.y = m_pDatas[i].InstancingData.vFadeInTargetScale.x;
			m_pDatas[i].InstancingData.fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);

			m_pDatas[i].InstancingData.fDissolveEndTime = m_iWidthSize * m_iHeightSize * m_fDuration; // 
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
		{
			m_pDatas[i].InstancingData.fDuration = m_fDuration; 

			if(0 < m_fDurationRange)
				m_pDatas[i].InstancingData.fDuration = random(0, m_fDurationRange);
		}
		//

		m_pDatas[i].InstancingData.vFadeOutTargetScale = _float4(
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.x, m_tCreateData.vFadeOutTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.y, m_tCreateData.vFadeOutTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.z, m_tCreateData.vFadeOutTargetScaleRange.z),
			0.f);

		m_pDatas[i].InstancingData.vFadeOutTargetScale += m_tCreateData.vFadeOutTargetScale;
		if (m_pDatas[i].InstancingData.vFadeOutTargetScale.x <= 0.f)
			m_pDatas[i].InstancingData.vFadeOutTargetScale.x = 0.1f;
		if (m_pDatas[i].InstancingData.vFadeOutTargetScale.y <= 0.f)
			m_pDatas[i].InstancingData.vFadeOutTargetScale.y = 0.1f;
		if (m_pDatas[i].InstancingData.vFadeOutTargetScale.z <= 0.f)
			m_pDatas[i].InstancingData.vFadeOutTargetScale.z = 0.1f;


		m_pDatas[i].InstancingData.vTurnDir = _float4(
			0.f,
			0.f,
			0.f,
			0.f);


		/*m_pDatas[i].RectInstance.vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pDatas[i].RectInstance.vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pDatas[i].RectInstance.vLook = _float4(0.f, 0.f, 1.f, 0.f);*/





		//초기화용 매트릭스 저장
		* ((_float4*)(&(m_pDatas[i].InstancingData.StartMatrix.m[0]))) = m_pDatas[i].RectInstance.vRight;
		*((_float4*)(&(m_pDatas[i].InstancingData.StartMatrix.m[1]))) = m_pDatas[i].RectInstance.vUp;
		*((_float4*)(&(m_pDatas[i].InstancingData.StartMatrix.m[2]))) = m_pDatas[i].RectInstance.vLook;
		*((_float4*)(&(m_pDatas[i].InstancingData.StartMatrix.m[3]))) = m_pDatas[i].RectInstance.vTranslation;



		//Jump
		m_pDatas[i].InstancingData.fPrevY = m_pDatas[i].RectInstance.vTranslation.y;
		m_pDatas[i].InstancingData.fOriginY = m_pDatas[i].RectInstance.vTranslation.y;
		m_pDatas[i].InstancingData.fAcc = 0.f;
		m_pDatas[i].InstancingData.fPlusAcc = 1.5f;


		m_pDatas[i].InstancingData.bAlive = true;
	}

	if (m_bEffectFlag & EFFECT_REFBONE)
	{
		if (m_pFollowTarget)
		{
			m_pRefBone = GET_COMPONENT_FROM(m_pFollowTarget, CModel)->Find_HierarchyNode(m_strBoneName.c_str());
		}
	}
	


	return S_OK;
}

HRESULT CRectEffects::Re_Initialize()
{
	Release();

	static_cast<CRect_Instance*>(Get_Component<CMesh>().front())->ReMake_Instance(m_tCreateData.iNumInstance);

	m_pDatas = new DATAS[m_tCreateData.iNumInstance];
	ZeroMemory(m_pDatas, sizeof(DATAS) * m_tCreateData.iNumInstance);

	m_pFinalRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
	ZeroMemory(m_pFinalRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);

	return Initialize();
}

HRESULT CRectEffects::Start()
{
	CGameObject::Start();
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, "g_vFlag");
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, "g_vGlowFlag");

	return S_OK;
}

void CRectEffects::Release()
{
	SAFE_DELETE_ARRAY(m_pDatas);
	SAFE_DELETE_ARRAY(m_pFinalRectInstances);
}

void CRectEffects::My_Tick()
{
	_float fTimeDelta = fDT(0);
	_float4	vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	_float4 vLook;

	m_fLoopTimeAcc += fTimeDelta;

	vLook = vCamLook * -1.f;

	_float4 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);

	vUp.Normalize();
	_float4 vRight = vUp.Cross(vLook.Normalize());

	vUp = vLook.Cross(vRight.Normalize());
	vUp.Normalize();


	for (_uint i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		if (!m_pDatas[i].InstancingData.bAlive)
			continue;

		m_pDatas[i].InstancingData.fTimeAcc += fTimeDelta;
		m_pDatas[i].InstancingData.fMovingAcc += fTimeDelta;

		//3. FADE


		if (!Fade_Lerp(i))
			continue;




		if (m_iPassType != VTXRECTINSTANCE_PASS_ANIMATION && m_iPassType != VTXRECTINSTANCE_PASS_ANIMATIONALPHA &&
			m_iPassType != VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE && m_iPassType != VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR &&
			m_iPassType != VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
			m_pDatas[i].RectInstance.vColor = m_pDatas[i].InstancingData.vColor;
		else
			m_pDatas[i].RectInstance.vColor.w = m_pDatas[i].InstancingData.vColor.w;



		_float4 vOriginPos;
		vOriginPos = m_pDatas[i].RectInstance.vTranslation;


		//FadeIn시작되어야 이동시작
		if (m_pDatas[i].InstancingData.eCurFadeType != FADEINREADY)
		{
			//1. 속도 변화.
			m_pDatas[i].InstancingData.fSpeed += m_pDatas[i].InstancingData.fSpeedChangeSpeed * fTimeDelta;

			if (m_bZeroSpeedDisable)
			{
				if (m_pDatas[i].InstancingData.fOriginSpeed < 0.f)
				{
					if (m_pDatas[i].InstancingData.fSpeed >= 0.f)
					{

						Dead_Instance(i);
						continue;
					}
				}
				else if (m_pDatas[i].InstancingData.fSpeed <= 0.f)
				{

					Dead_Instance(i);
					continue;
				}
			}

			vOriginPos += m_pDatas[i].InstancingData.vDir * m_pDatas[i].InstancingData.fSpeed * fTimeDelta;

			vOriginPos = Switch_CurveType(vOriginPos, i, fTimeDelta);




			//2. 자유낙하
			_float fFreeFallY = 0.f;
			_float fFreeFallPower = 0.f;

			m_pDatas[i].InstancingData.fAcc += m_pDatas[i].InstancingData.fPlusAcc * fTimeDelta;

			fFreeFallY = m_pDatas[i].InstancingData.fOriginY + ((m_pDatas[i].InstancingData.fJumpPower * m_pDatas[i].InstancingData.fAcc) - (m_pDatas[i].InstancingData.fGravity * 0.5f * m_pDatas[i].InstancingData.fAcc * m_pDatas[i].InstancingData.fAcc));

			//이동량 = 현재 Y - 이전 Y
			fFreeFallPower = fFreeFallY - m_pDatas[i].InstancingData.fPrevY;

			m_pDatas[i].InstancingData.fPrevY = fFreeFallY;

			vOriginPos.y += fFreeFallPower;

			if (vOriginPos.y < m_pDatas[i].InstancingData.fCurGroundY)
			{
				Dead_Instance(i);
				continue;
			}
		}

		//dir의 x값을 회전양으로 써

		//회전은 z축 회전만
		if (m_bBillBoard)
		{
			m_pDatas[i].InstancingData.vTurnDir.x += m_pDatas[i].InstancingData.fTurnSpeed * fTimeDelta;

			_float4 vRotRight, vRotUp, vRotLook;

			vRotRight = vRight;
			vRotUp = vUp;
			vRotLook = vLook;

			if (m_pDatas[i].InstancingData.fTurnSpeed > 0.f)
			{
				_float4x4 matRot;

				matRot = XMMatrixRotationAxis(vCamLook.XMLoad(), m_pDatas[i].InstancingData.vTurnDir.x);

				vRotLook = vLook.MultiplyNormal(matRot);
				vRotRight = vRight.MultiplyNormal(matRot);
				vRotUp = vUp.MultiplyNormal(matRot);
			}

			if (CURVE_ROTATION == m_eCurveType)
			{

				vRight = m_matTrans.XMLoad().r[0];
				vUp = m_matTrans.XMLoad().r[1];

				_float4x4 matRot;
				_float4 vAxis = m_matTrans.XMLoad().r[2];
				matRot = XMMatrixRotationAxis(vAxis.XMLoad(), ToRadian(m_tCreateData.fCurveAngle));

				//matRot *= m_matTrans;

				vRotLook = vLook.MultiplyNormal(matRot);
				vRotRight = vRight.MultiplyNormal(matRot);
				vRotUp = vUp.MultiplyNormal(matRot);

			}


			//m_pDatas[i].InstancingData.vScale = _float4(0.1f, 0.1f, 0.1f, 1.f);
			m_pDatas[i].RectInstance.vRight = vRotRight.Normalize() * m_pDatas[i].InstancingData.vScale.x;
			m_pDatas[i].RectInstance.vUp = vRotUp.Normalize() * m_pDatas[i].InstancingData.vScale.y;
			m_pDatas[i].RectInstance.vLook = vRotLook.Normalize() * m_pDatas[i].InstancingData.vScale.z;
		}
		else
		{
			_float4 vRotRight, vRotUp, vRotLook;

			vRotRight = m_pDatas[i].RectInstance.vRight;
			vRotUp = m_pDatas[i].RectInstance.vUp;
			vRotLook = m_pDatas[i].RectInstance.vLook;

			if (m_pDatas[i].InstancingData.fTurnSpeed > 0.f)
			{
				_float4x4 matRot;

				matRot = XMMatrixRotationAxis(vRotRight.Normalize().XMLoad(), m_pDatas[i].InstancingData.fTurnSpeed * fTimeDelta);

				vRotLook = vRotLook.MultiplyNormal(matRot);
				vRotRight = vRotRight.MultiplyNormal(matRot);
				vRotUp = vRotUp.MultiplyNormal(matRot);
			}

			m_pDatas[i].RectInstance.vRight = vRotRight.Normalize() * m_pDatas[i].InstancingData.vScale.x;
			m_pDatas[i].RectInstance.vUp = vRotUp.Normalize() * m_pDatas[i].InstancingData.vScale.y;
			m_pDatas[i].RectInstance.vLook = vRotLook.Normalize() * m_pDatas[i].InstancingData.vScale.z;
		}
		m_pDatas[i].RectInstance.vTranslation = vOriginPos;

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
			m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR)
			Update_Animation(i);

		if (m_iPassType == VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
			Select_UVTexture(i);

	}


	if (m_iNumDead >= m_tCreateData.iNumInstance)
	{
		DISABLE_GAMEOBJECT(this);
	}




}

void CRectEffects::My_LateTick()
{
	if (m_bSorting)
	{
		_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
		vCamPos *= 1000.f;

		//섞고난 RectInstance가 몇번째 인덱스로 가있는지 알 수 있엉?

		sort(m_pDatas, m_pDatas + (m_tCreateData.iNumInstance - 1), [vCamPos](DATAS& p1, DATAS& p2)
			{
				//노말라이즈한 룩이랑
				_float4 vNormalLook;
				vNormalLook = p1.RectInstance.vLook;
				vNormalLook.Normalize();

				//카메라위치에서 내위치 뺸 벡터를
				_float4 vVector;
				vVector = (XMLoadFloat4(&vCamPos) - XMLoadFloat4(&p1.RectInstance.vTranslation));

				//내적
				_float fDist1 = vNormalLook.Dot(vVector);

				//노말라이즈한 룩이랑
				vNormalLook = p2.RectInstance.vLook;
				vNormalLook.Normalize();

				//카메라위치에서 내위치 뺸 벡터를
				vVector = (XMLoadFloat4(&vCamPos) - XMLoadFloat4(&p2.RectInstance.vTranslation));

				//내적
				_float fDist2 = vNormalLook.Dot(vVector);


				p1.fDistance = fDist1;
				p2.fDistance = fDist2;
				
				return fDist1 > fDist2;
			});
	}

	
	for (_uint i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		m_pFinalRectInstances[i] = m_pDatas[i].RectInstance;
	}


	if (m_bEffectFlag & EFFECT_REFBONE)
	{
		if (m_pRefBone)
		{
			if(m_bKeepSticked)
				Stick_RefBone();
		}
	}


	static_cast<CRect_Instance*>(GET_COMPONENT(CMesh))->ReMap_Instances(m_pFinalRectInstances);

	if (m_bEffectFlag & EFFECT_FOLLOWTARGET)
	{
		m_pTransform->Set_World(WORLD_POS, m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS));
		m_pTransform->Make_WorldMatrix();
	}

}

void CRectEffects::OnEnable()
{
	CEffect::OnEnable();

	m_iNumDead = 0;
	m_fTimeAcc = 0.f;
	m_fLoopTimeAcc = 0.f;
	//시작위치



	for (_uint i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		Reset_Instance(i);
	}

	Stick_RefBone();
}

_bool CRectEffects::Fade_Lerp(_uint iIndex)
{
	switch (m_pDatas[iIndex].InstancingData.eCurFadeType)
	{
	case INSTANCING_DATA::FADEINREADY:
		if (m_pDatas[iIndex].InstancingData.fTimeAcc >= m_pDatas[iIndex].InstancingData.fFadeInStartTime)
		{
			m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;
			m_pDatas[iIndex].InstancingData.eCurFadeType = INSTANCING_DATA::FADEIN;
			m_pDatas[iIndex].InstancingData.vOriginScale = m_pDatas[iIndex].InstancingData.vScale = m_pDatas[iIndex].InstancingData.vScale;
			m_pDatas[iIndex].InstancingData.fOriginAlpha = 0.f;


		}
		else
		{
			return false;

		}


		break;

	case INSTANCING_DATA::FADEIN:
		if (m_pDatas[iIndex].InstancingData.fTimeAcc >= m_pDatas[iIndex].InstancingData.fFadeInTime)
		{
			m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;
			m_pDatas[iIndex].InstancingData.eCurFadeType = INSTANCING_DATA::FADEOUTREADY;
			m_pDatas[iIndex].InstancingData.vScale = m_pDatas[iIndex].InstancingData.vFadeInTargetScale;
			m_pDatas[iIndex].InstancingData.vColor.w = m_pDatas[iIndex].InstancingData.fTargetAlpha;
		}
		else
		{
			_float fRatio = m_pDatas[iIndex].InstancingData.fTimeAcc / m_pDatas[iIndex].InstancingData.fFadeInTime;

			if (m_bSoft)
				fRatio *= fRatio;

			m_pDatas[iIndex].InstancingData.vScale =
				XMVectorLerp(m_pDatas[iIndex].InstancingData.vOriginScale.XMLoad(), m_pDatas[iIndex].InstancingData.vFadeInTargetScale.XMLoad(), fRatio);

			m_pDatas[iIndex].InstancingData.vColor.w = CFunctor::Lerp_Float(
				0.f, m_pDatas[iIndex].InstancingData.fTargetAlpha, fRatio);

		}

		break;

	case INSTANCING_DATA::FADEOUTREADY:
		if (m_pDatas[iIndex].InstancingData.fTimeAcc >= m_pDatas[iIndex].InstancingData.fFadeOutStartTime)
		{
			m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;
			m_pDatas[iIndex].InstancingData.eCurFadeType = INSTANCING_DATA::FADEOUT;
			m_pDatas[iIndex].InstancingData.vOriginScale = m_pDatas[iIndex].InstancingData.vScale = m_pDatas[iIndex].InstancingData.vFadeInTargetScale;
			m_pDatas[iIndex].InstancingData.fOriginAlpha = m_pDatas[iIndex].InstancingData.vColor.w = m_pDatas[iIndex].InstancingData.fTargetAlpha;
		}

		break;

	case INSTANCING_DATA::FADEOUT:
		if (m_pDatas[iIndex].InstancingData.fTimeAcc >= m_pDatas[iIndex].InstancingData.fFadeOutTime)
		{
			m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;
			Dead_Instance(iIndex);
			return false;
		}
		else
		{
			_float fRatio = 0.f;
			if (0 < m_pDatas[iIndex].InstancingData.fDissolveEndTime) //animation dissolve 인 경우
			{
				fRatio = m_pDatas[iIndex].InstancingData.fTimeAcc / (m_pDatas[iIndex].InstancingData.fDissolveEndTime - (m_pDatas[iIndex].InstancingData.fFadeInTime + m_pDatas[iIndex].InstancingData.fFadeOutStartTime));
				fRatio = fabs(fRatio);
			}
			else
			{
				fRatio = m_pDatas[iIndex].InstancingData.fTimeAcc / m_pDatas[iIndex].InstancingData.fFadeOutTime;
			}

			if (m_bSoft)
				fRatio = sqrtf(fRatio);

			m_pDatas[iIndex].InstancingData.vScale =
				XMVectorLerp(m_pDatas[iIndex].InstancingData.vOriginScale.XMLoad(), m_pDatas[iIndex].InstancingData.vFadeOutTargetScale.XMLoad(), fRatio);

			m_pDatas[iIndex].InstancingData.vColor.w = m_pDatas[iIndex].InstancingData.fTargetAlpha * (1.f - fRatio);

		}

		break;

	default:
		break;
	}

	return true;
}

void CRectEffects::Dead_Instance(_uint iIndex)
{
	if (m_bLoop)
	{
		if (m_fLoopTime == 0)
			Reset_Instance(iIndex);
		else if (m_fLoopTimeAcc <= m_fLoopTime)
			Reset_Instance(iIndex);
		else if (m_fLoopTimeAcc > m_fLoopTime)
		{
			//m_fLoopTimeAcc = 0.f;
			//m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;
			m_pDatas[iIndex].InstancingData.bAlive = false;
			m_iNumDead++;
			m_pDatas[iIndex].RectInstance.vColor.w = 0.f;
			m_pDatas[iIndex].InstancingData.fSpeed = m_pDatas[iIndex].InstancingData.fOriginSpeed;
		}
	}
	else
	{


		m_pDatas[iIndex].InstancingData.bAlive = false;
		m_iNumDead++;
		m_pDatas[iIndex].RectInstance.vColor.w = 0.f;
		m_pDatas[iIndex].InstancingData.fSpeed = m_pDatas[iIndex].InstancingData.fOriginSpeed;
	}
	//m_pDatas[iIndex].InstancingData.bAlive = true;
	//m_iNumDead++;
	//m_pDatas[iIndex].RectInstance.vColor.w = 0.f;
	//m_pDatas[iIndex].InstancingData.fSpeed = m_pDatas[iIndex].InstancingData.fOriginSpeed;
	//작아지게해서 안보이게
}

void CRectEffects::Set_NewStartPos(_uint iIndex)
{
	//offset 잡기

	_float4 vStartPos = m_pDatas[iIndex].InstancingData.vStartPureLocalPos;
	_float4 vStartDir = m_pDatas[iIndex].InstancingData.vStartPureLocalDir;
	_float4 vStartRight = m_pDatas[iIndex].InstancingData.vStartPureLocalRight;

	if (m_tCreateData.iOffsetPositionCount > 0)
	{
		_uint iCurIndex = iIndex / (m_tCreateData.iNumInstance / m_tCreateData.iOffsetPositionCount);
		vStartPos += m_tCreateData.pOffsetPositions[iCurIndex];
	}

	/*	if (m_bFollowParticle)
		{
			m_pFollowTarget->
		}*/

	vStartPos = vStartPos.MultiplyCoord(m_matTrans);
	vStartDir = vStartDir.MultiplyNormal(m_matTrans).Normalize();
	vStartRight = vStartRight.MultiplyNormal(m_matTrans).Normalize();
	m_pDatas[iIndex].RectInstance.vTranslation = vStartPos;
	m_pDatas[iIndex].InstancingData.vDir = vStartDir;
	m_pDatas[iIndex].InstancingData.vRight = vStartRight;

	//회전시켜놓기

	//Up은 dir이고
	//Right는 0 1 0 임
	//

/*	_float4 vUp = m_pDatas[iIndex].InstancingData.vDir;
	vUp.z = 0.f;
	vUp.Normalize();
	_float4 vRight = { 0.f, 1.f, 0.f, 0.f };
	_float4 vLook = vRight.Cross(vUp).Normalize();
	vRight = vUp.Cross(vLook).Normalize();

	m_pDatas[iIndex].RectInstance.vRight = vRight.MultiplyNormal(m_matTrans).Normalize();
	m_pDatas[iIndex].RectInstance.vUp = vUp.MultiplyNormal(m_matTrans).Normalize();
	m_pDatas[iIndex].RectInstance.vLook = vLook.MultiplyNormal(m_matTrans).Normalize();*/
	if (!(m_bLoop && m_bBillBoard))
	{
		_float4 vLook = m_pDatas[iIndex].InstancingData.vDir;

		m_pDatas[iIndex].RectInstance.vUp = vLook.Normalize();

		_float4 vUp = { 0.f, 1.f, 0.f };
		if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
			(vLook.y > -1.1f && vLook.y < -0.9f)
			)
			vUp = _float4(0.f, 0.f, 1.f, 0.f);

		vUp.Normalize();
		_float4 vRight = vUp.Cross(vLook);
		m_pDatas[iIndex].RectInstance.vRight = vRight.Normalize();

		vUp = vLook.Cross(vRight);
		m_pDatas[iIndex].RectInstance.vLook = vUp.Normalize();
	}



}

HRESULT CRectEffects::SetUp_RectEffects(ifstream* pReadFile)
{
	string	strMaskMapPath, strColorMapPath;
	strMaskMapPath = CUtility_File::Read_Text(pReadFile);
	strColorMapPath = CUtility_File::Read_Text(pReadFile);

	m_wstrMaskMapPath = CFunctor::To_Wstring(strMaskMapPath);
	m_wstrColorMapPath = CFunctor::To_Wstring(strColorMapPath);


	pReadFile->read((char*)&m_vEffectFlag, sizeof(_float4));
	pReadFile->read((char*)&m_vGlowFlag, sizeof(_float4));
	pReadFile->read((char*)&m_iPassType, sizeof(_uint));


	pReadFile->read((char*)&m_bBillBoard, sizeof(_bool));
	pReadFile->read((char*)&m_bSoft, sizeof(_bool));
	pReadFile->read((char*)&m_bZeroSpeedDisable, sizeof(_bool));
	pReadFile->read((char*)&m_bLoop, sizeof(_bool));
	pReadFile->read((char*)&m_fLoopTime, sizeof(_float));
	pReadFile->read((char*)&m_eCurveType, sizeof(CURVE_TYPE));

	pReadFile->read((char*)&m_bEffectFlag, sizeof(_byte));
	pReadFile->read((char*)&m_vOffsetPos, sizeof(_float4));
	m_strBoneName = CUtility_File::Read_Text(pReadFile);

	pReadFile->read((char*)&m_fDiscardPower, sizeof(_float));
	pReadFile->read((char*)&m_bKeepSticked, sizeof(_bool));

	pReadFile->read((char*)&m_vPlusColor, sizeof(_float4));
	pReadFile->read((char*)&m_fColorPower, sizeof(_float));

	pReadFile->read((char*)&m_bSorting, sizeof(_bool));

	pReadFile->read((char*)&m_tCreateData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));
	if (m_tCreateData.iOffsetPositionCount > 0)
	{
		m_tCreateData.pOffsetPositions = new _float4[m_tCreateData.iOffsetPositionCount];
		pReadFile->read((char*)m_tCreateData.pOffsetPositions, sizeof(_float4) * m_tCreateData.iOffsetPositionCount);

	}

	return Initialize_Prototype();
}

HRESULT CRectEffects::SetUp_RectEffects_Anim(ifstream* pReadFile)
{
	string	strMaskMapPath, strColorMapPath, strNormalMapPath;
	strMaskMapPath = CUtility_File::Read_Text(pReadFile);
	strColorMapPath = CUtility_File::Read_Text(pReadFile);
	strNormalMapPath = CUtility_File::Read_Text(pReadFile);

	m_wstrMaskMapPath = CFunctor::To_Wstring(strMaskMapPath);
	m_wstrColorMapPath = CFunctor::To_Wstring(strColorMapPath);
	m_wstrNoiseMapPath = CFunctor::To_Wstring(strNormalMapPath);


	pReadFile->read((char*)&m_vEffectFlag, sizeof(_float4));
	pReadFile->read((char*)&m_vGlowFlag, sizeof(_float4));
	pReadFile->read((char*)&m_iPassType, sizeof(_uint));


	pReadFile->read((char*)&m_bBillBoard, sizeof(_bool));
	pReadFile->read((char*)&m_bSoft, sizeof(_bool));
	pReadFile->read((char*)&m_bZeroSpeedDisable, sizeof(_bool));
	pReadFile->read((char*)&m_bLoop, sizeof(_bool));
	pReadFile->read((char*)&m_fLoopTime, sizeof(_float));
	pReadFile->read((char*)&m_bBlackBackGround, sizeof(_bool));
	pReadFile->read((char*)&m_bPlayOnce, sizeof(_bool));

	pReadFile->read((char*)&m_iWidthSize, sizeof(_uint));
	pReadFile->read((char*)&m_iHeightSize, sizeof(_uint));
	pReadFile->read((char*)&m_fDuration, sizeof(_float));
	pReadFile->read((char*)&m_fDurationRange, sizeof(_float));
	pReadFile->read((char*)&m_fDissolvePower, sizeof(_float));

	pReadFile->read((char*)&m_eCurveType, sizeof(CURVE_TYPE));
	
	pReadFile->read((char*)&m_bEffectFlag, sizeof(_byte));
	pReadFile->read((char*)&m_vOffsetPos, sizeof(_float4));
	m_strBoneName = CUtility_File::Read_Text(pReadFile);

	pReadFile->read((char*)&m_fDiscardPower, sizeof(_float));
	pReadFile->read((char*)&m_bKeepSticked, sizeof(_bool));

	pReadFile->read((char*)&m_vPlusColor, sizeof(_float4));
	pReadFile->read((char*)&m_fColorPower, sizeof(_float));

	pReadFile->read((char*)&m_bSorting, sizeof(_bool));

	pReadFile->read((char*)&m_tCreateData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));

	if (m_tCreateData.iOffsetPositionCount > 0)
	{
		m_tCreateData.pOffsetPositions = new _float4[m_tCreateData.iOffsetPositionCount];
		pReadFile->read((char*)m_tCreateData.pOffsetPositions, sizeof(_float4) * m_tCreateData.iOffsetPositionCount);

	}

	//m_bSorting = true;

	return Initialize_Prototype();
}

void CRectEffects::Update_Animation(_uint iIndex)
{
	m_pDatas[iIndex].InstancingData.vTurnDir.y += fDT(0);

	//UV넘기는 코드

	while (m_pDatas[iIndex].InstancingData.vTurnDir.y > m_pDatas[iIndex].InstancingData.fDuration * 
		((m_pDatas[iIndex].RectInstance.vColor.x + 1) + m_iHeightSize * m_pDatas[iIndex].RectInstance.vColor.y))
	{
		m_pDatas[iIndex].RectInstance.vColor.x += 1.f;
		if (m_pDatas[iIndex].RectInstance.vColor.x >= m_iWidthSize)
		{
			m_pDatas[iIndex].RectInstance.vColor.x = 0.f;
			m_pDatas[iIndex].RectInstance.vColor.y += 1.f;
			if (m_pDatas[iIndex].RectInstance.vColor.y >= m_iHeightSize)
			{
				//여기 들어왔다 : 한바퀴돌아서 1순한거임
				m_pDatas[iIndex].InstancingData.vTurnDir.y = 0.f;
				m_pDatas[iIndex].RectInstance.vColor.x = m_iWidthSize - 1;
				m_pDatas[iIndex].RectInstance.vColor.y = m_iHeightSize - 1;

				if (m_bPlayOnce)
					Dead_Instance(iIndex);
				else
				{
					m_pDatas[iIndex].RectInstance.vColor.x = 0.f;
					m_pDatas[iIndex].RectInstance.vColor.y = 0.f;
				}
			}
		}
	}
}

void CRectEffects::Select_UVTexture(_uint iIndex)
{
	_uint iSelect = (_uint)m_pDatas[iIndex].InstancingData.fDuration;
	m_pDatas[iIndex].RectInstance.vColor.x = iSelect % m_iWidthSize;
	m_pDatas[iIndex].RectInstance.vColor.y = iSelect / m_iHeightSize;
}

void CRectEffects::Stick_RefBone()
{
	if (!m_pRefBone)
		return;

	_float4 vPos = m_vOffsetPos;
	_float4x4 matBone = m_pRefBone->Get_BoneMatrix();


	vPos = vPos.MultiplyCoord(matBone);
	m_pTransform->Set_World(WORLD_POS, vPos);

	m_pTransform->Make_WorldMatrix();
}



void CRectEffects::Reset_Instance(_uint iIndex)
{
	

	m_pDatas[iIndex].RectInstance.vRight = *((_float4*)(&(m_pDatas[iIndex].InstancingData.StartMatrix.m[0])));
	m_pDatas[iIndex].RectInstance.vUp = *((_float4*)(&(m_pDatas[iIndex].InstancingData.StartMatrix.m[1])));
	m_pDatas[iIndex].RectInstance.vLook = *((_float4*)(&(m_pDatas[iIndex].InstancingData.StartMatrix.m[2])));

	Set_NewStartPos(iIndex);

	m_pDatas[iIndex].InstancingData.vColor.w = 0.f;
	m_pDatas[iIndex].RectInstance.vColor.w = 0.f;

	m_pDatas[iIndex].InstancingData.vScale = m_pDatas[iIndex].InstancingData.vStartScale;

	m_pDatas[iIndex].InstancingData.eCurFadeType = INSTANCING_DATA::FADEINREADY;

	m_pDatas[iIndex].InstancingData.fTimeAcc = 0.f;


	m_pDatas[iIndex].InstancingData.vTurnDir.x = 0.f;
	m_pDatas[iIndex].InstancingData.vTurnDir.y = 0.f;
	m_pDatas[iIndex].InstancingData.vTurnDir.z = 0.f;

	if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
		m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHACOLOR)
	{
		m_pDatas[iIndex].RectInstance.vColor.x = 0.f;
		m_pDatas[iIndex].RectInstance.vColor.y = 0.f;
	}
	if (m_iPassType == VTXRECTINSTANCE_PASS_UVTEXTURESELECT)
	{
		_uint iSelect = (_uint)m_pDatas[iIndex].InstancingData.fDuration;
		m_pDatas[iIndex].RectInstance.vColor.x = iSelect % m_iWidthSize;
		m_pDatas[iIndex].RectInstance.vColor.y = iSelect / m_iHeightSize;
	}

	//Jump
	m_pDatas[iIndex].InstancingData.fPrevY = m_pDatas[iIndex].RectInstance.vTranslation.y;
	m_pDatas[iIndex].InstancingData.fOriginY = m_pDatas[iIndex].RectInstance.vTranslation.y;
	m_pDatas[iIndex].InstancingData.fAcc = 0.f;
	m_pDatas[iIndex].InstancingData.fMovingAcc = 0.f;
	m_pDatas[iIndex].InstancingData.bAlive = true;


}

_float4 CRectEffects::Switch_CurveType(_float4 vPos, _uint iIdx, _float fTimeDelta)
{
	_float fY;
	_float fX;
	_float fSpeed;

	switch (m_eCurveType)
	{
	case Client::CURVE_LINEAR:
	case Client::CURVE_ROTATION:
		break;
	case Client::CURVE_SIN:
		fY = m_pDatas[iIdx].InstancingData.fCurvePower * sinf(m_pDatas[iIdx].InstancingData.fCurveFrequency * m_pDatas[iIdx].InstancingData.fMovingAcc); // a sin(bx)

		vPos.x += fY * m_pDatas[iIdx].InstancingData.vRight.x * fTimeDelta;
		vPos.y += fY * m_pDatas[iIdx].InstancingData.vRight.y * fTimeDelta;
		vPos.z += fY * m_pDatas[iIdx].InstancingData.vRight.z * fTimeDelta;
		break;
	case Client::CURVE_SPIRAL:
		fSpeed = m_pDatas[iIdx].InstancingData.fCurveFrequency * m_pDatas[iIdx].InstancingData.fMovingAcc;

		fX = m_pDatas[iIdx].InstancingData.fCurvePower *
			sinf(fSpeed * PI * 0.5f);

		fY = m_pDatas[iIdx].InstancingData.fCurvePower *
			cosf(-1.f * fSpeed * PI * 0.5f);

		vPos.x += fX * m_pDatas[iIdx].InstancingData.vDir.x * fTimeDelta;
		vPos.y += fX * m_pDatas[iIdx].InstancingData.vDir.y * fTimeDelta;
		vPos.z += fX * m_pDatas[iIdx].InstancingData.vDir.z * fTimeDelta;

		vPos.x += fY * m_pDatas[iIdx].InstancingData.vRight.x * fTimeDelta;
		vPos.y += fY * m_pDatas[iIdx].InstancingData.vRight.y * fTimeDelta;
		vPos.z += fY * m_pDatas[iIdx].InstancingData.vRight.z * fTimeDelta;

		break;
	default:
		break;
	}


	return vPos;
}