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
	, m_bFixed(_origin.m_bFixed)
	, m_eCurveType(_origin.m_eCurveType)
{
	if (_origin.m_pRectInstances)
	{
		m_pRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
		memcpy(m_pRectInstances, _origin.m_pRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);
	}

}

CRectEffects::~CRectEffects()
{
	SAFE_DELETE_ARRAY(m_pRectInstances);
}

CRectEffects* CRectEffects::Create(_uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, wstring wstrTexturePath,
	_hashcode _hcCode, _bool bBillBoard, _bool bSorting, _bool bZeroSpeedDisable, _bool bLoop, _bool bFixed)
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
	pInstance->m_bFixed = bFixed;

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
		Call_MsgBox(L"Failed to Initialize_Prototype : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CRectEffects* CRectEffects::Create_Anim(ifstream* pReadFile)
{
	CRectEffects* pInstance = new CRectEffects;

	if (FAILED(pInstance->SetUp_RectEffects_Anim(pReadFile)))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRectEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CRectEffects::Self_Reset(CGameObject* pGameObject, _float4 vStartPos)
{
	//__super::Self_Reset(pGameObject, vStartPos);
	Reset(vStartPos);
	m_pFollowTarget = pGameObject;
	//이거 파티클 빌보드 잘 안대는거 확인중
	m_matTrans = pGameObject->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);


	//if (m_bBillBoard)
	//{
	//	m_matTrans = pGameObject->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);
	//}
	//else
	//	m_matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE | MARTIX_NOTRANS);

}

void CRectEffects::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
		m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
	{
		pShader->Set_RawValue("g_iWidthSize", &m_iWidthSize, sizeof(_uint));
		pShader->Set_RawValue("g_iHeightSize", &m_iHeightSize, sizeof(_uint));
		pShader->Set_RawValue("g_fDissolvePower", &m_fDissolvePower, sizeof(_float));
		pShader->Set_RawValue("g_bBlackBG", &m_bBlackBackGround, sizeof(_float));
	}

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


	m_pRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
	ZeroMemory(m_pRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);

	m_pInstancingDatas = new INSTANCING_DATA[m_tCreateData.iNumInstance];

	m_matTrans.Identity();
	//m_bBillBoard = true;

	m_bEffectFlag = 0;

	return S_OK;
}

HRESULT CRectEffects::Initialize()
{
	m_fTimeAcc = 0.f;
	m_iNumDead = 0;

	GET_COMPONENT(CRenderer)->Set_Pass(m_iPassType);

	for (_uint i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEINREADY;

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

		m_pInstancingDatas[i].vStartPureLocalPos = vStartPos;
		m_pInstancingDatas[i].vStartPureLocalDir = vMoveDir;



		_float4 vUpDir = { 0.f, 1.f, 0.f };
		if ((vMoveDir.y < 1.1f && vMoveDir.y > 0.9f) ||
			(vMoveDir.y > -1.1f && vMoveDir.y < -0.9f)
			)
			vUpDir = _float4(0.f, 0.f, 1.f, 0.f);

		vUpDir.Normalize();
		m_pInstancingDatas[i].vStartPureLocalRight = vUpDir.Cross(vMoveDir);

		_float4x4 matCurveRot;

		_float fStartCurveAngle = m_tCreateData.fCurveAngle + frandom(-m_tCreateData.fCurveAngleRange, m_tCreateData.fCurveAngleRange);

		matCurveRot = XMMatrixRotationAxis(m_pInstancingDatas[i].vStartPureLocalDir.XMLoad(), ToRadian(fStartCurveAngle));
		
		m_pInstancingDatas[i].vStartPureLocalRight = m_pInstancingDatas[i].vStartPureLocalRight.MultiplyNormal(matCurveRot);

		m_pInstancingDatas[i].fCurvePower = m_tCreateData.fCurvePower + frandom(-m_tCreateData.fCurvePowerRange, m_tCreateData.fCurvePowerRange);

		m_pInstancingDatas[i].fCurveFrequency = m_tCreateData.fCurveFrequency + frandom(-m_tCreateData.fCurveFrequencyRange, m_tCreateData.fCurveFrequencyRange);


		Set_NewStartPos(i);

		m_pInstancingDatas[i].fSpeed = m_pInstancingDatas[i].fOriginSpeed = m_tCreateData.fSpeed + frandom(-m_tCreateData.fSpeedRange, m_tCreateData.fSpeedRange);
		m_pInstancingDatas[i].fTurnSpeed = m_tCreateData.fTurnSpeed + frandom(-m_tCreateData.fTurnSpeedRange, m_tCreateData.fTurnSpeedRange);
		m_pInstancingDatas[i].fJumpPower = m_tCreateData.fJumpPower + frandom(-m_tCreateData.fJumpPowerRange, m_tCreateData.fJumpPowerRange);
		m_pInstancingDatas[i].fGravity = m_tCreateData.fGravity + frandom(-m_tCreateData.fGravityRange, m_tCreateData.fGravityRange);
		m_pInstancingDatas[i].fCurGroundY = m_tCreateData.fCurGroundY + frandom(-m_tCreateData.fCurGroundYRange, m_tCreateData.fCurGroundYRange);
		m_pInstancingDatas[i].fCurGroundY = -100.f;
		m_pInstancingDatas[i].fSpeedChangeSpeed = m_tCreateData.fSpeedChangeSpeed + frandom(-m_tCreateData.fSpeedChangeSpeedRange, m_tCreateData.fSpeedChangeSpeedRange);
		m_pInstancingDatas[i].fTargetAlpha = m_tCreateData.fTargetAlpha;

		m_pInstancingDatas[i].fFadeInStartTime = m_tCreateData.fFadeInStartTime + frandom(-m_tCreateData.fFadeInStartTimeRange, m_tCreateData.fFadeInStartTimeRange);
		m_pInstancingDatas[i].fFadeInTime = m_tCreateData.fFadeInTime + frandom(-m_tCreateData.fFadeInTimeRange, m_tCreateData.fFadeInTimeRange);
		m_pInstancingDatas[i].fFadeOutStartTime = m_tCreateData.fFadeOutStartTime + frandom(-m_tCreateData.fFadeOutStartTimeRange, m_tCreateData.fFadeOutStartTimeRange);
		m_pInstancingDatas[i].fFadeOutTime = m_tCreateData.fFadeOutTime + frandom(-m_tCreateData.fFadeOutTimeRange, m_tCreateData.fFadeOutTimeRange);


		if (m_pInstancingDatas[i].fJumpPower > 0.f)
			m_pInstancingDatas[i].vDir.y = 0.f;

		m_pInstancingDatas[i].vColor = _float4(
			frandom(-m_tCreateData.vColorRange.x, m_tCreateData.vColorRange.x),
			frandom(-m_tCreateData.vColorRange.y, m_tCreateData.vColorRange.y),
			frandom(-m_tCreateData.vColorRange.z, m_tCreateData.vColorRange.z),
			0.f);
		m_pInstancingDatas[i].vColor += m_tCreateData.vColor;

		m_pInstancingDatas[i].vDir.Normalize();

		m_pInstancingDatas[i].vScale = _float4(
			frandom(-m_tCreateData.vScaleRange.x, m_tCreateData.vScaleRange.x),
			frandom(-m_tCreateData.vScaleRange.y, m_tCreateData.vScaleRange.y),
			frandom(-m_tCreateData.vScaleRange.z, m_tCreateData.vScaleRange.z),
			0.f);

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
			m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
		{
			m_pInstancingDatas[i].vScale.y = m_pInstancingDatas[i].vScale.x;
		}

		m_pInstancingDatas[i].vScale += m_tCreateData.vScale;

		if (m_pInstancingDatas[i].vScale.x <= 0.f)
			m_pInstancingDatas[i].vScale.x = 0.1f;
		if (m_pInstancingDatas[i].vScale.y <= 0.f)
			m_pInstancingDatas[i].vScale.y = 0.1f;
		if (m_pInstancingDatas[i].vScale.z <= 0.f)
			m_pInstancingDatas[i].vScale.z = 0.1f;

		m_pInstancingDatas[i].vStartScale = m_pInstancingDatas[i].vScale;

		m_pInstancingDatas[i].vFadeInTargetScale = _float4(
			frandom(-m_tCreateData.vFadeInTargetScaleRange.x, m_tCreateData.vFadeInTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.y, m_tCreateData.vFadeInTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.z, m_tCreateData.vFadeInTargetScaleRange.z),
			0.f);

		m_pInstancingDatas[i].vFadeInTargetScale += m_tCreateData.vFadeInTargetScale;
		if (m_pInstancingDatas[i].vFadeInTargetScale.x <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.x = 0.1f;
		if (m_pInstancingDatas[i].vFadeInTargetScale.y <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.y = 0.1f;
		if (m_pInstancingDatas[i].vFadeInTargetScale.z <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.z = 0.1f;

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION)
		{
			m_pInstancingDatas[i].vFadeInTargetScale.y = m_pInstancingDatas[i].vFadeInTargetScale.x;
			m_pInstancingDatas[i].fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA)
		{
			m_pInstancingDatas[i].vFadeInTargetScale.y = m_pInstancingDatas[i].vFadeInTargetScale.x;
			m_pInstancingDatas[i].fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);
			
			m_pInstancingDatas[i].fDissolveEndTime =  m_iWidthSize * m_iHeightSize * m_fDuration; // 
		}
		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
		{		
			m_pInstancingDatas[i].vFadeInTargetScale.y = m_pInstancingDatas[i].vFadeInTargetScale.x;
			m_pInstancingDatas[i].fDuration = m_fDuration + frandom(-m_fDurationRange, m_fDurationRange);

			m_pInstancingDatas[i].fDissolveEndTime = m_iWidthSize * m_iHeightSize * m_pInstancingDatas[i].fDuration; // 
		}
		//

		m_pInstancingDatas[i].vFadeOutTargetScale = _float4(
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.x, m_tCreateData.vFadeOutTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.y, m_tCreateData.vFadeOutTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.z, m_tCreateData.vFadeOutTargetScaleRange.z),
			0.f);

		m_pInstancingDatas[i].vFadeOutTargetScale += m_tCreateData.vFadeOutTargetScale;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.x <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.x = 0.1f;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.y <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.y = 0.1f;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.z <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.z = 0.1f;


		m_pInstancingDatas[i].vTurnDir = _float4(
			0.f,
			0.f,
			0.f,
			0.f);

		
		/*m_pRectInstances[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pRectInstances[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pRectInstances[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);*/


		


		//초기화용 매트릭스 저장
		*((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[0]))) = m_pRectInstances[i].vRight;
		*((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[1]))) = m_pRectInstances[i].vUp;
		*((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[2]))) = m_pRectInstances[i].vLook;
		*((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[3]))) = m_pRectInstances[i].vTranslation;



		//Jump
		m_pInstancingDatas[i].fPrevY = m_pRectInstances[i].vTranslation.y;
		m_pInstancingDatas[i].fOriginY = m_pRectInstances[i].vTranslation.y;
		m_pInstancingDatas[i].fAcc = 0.f;
		m_pInstancingDatas[i].fPlusAcc = 1.5f;


		m_pInstancingDatas[i].bAlive = true;
	}


	return S_OK;
}

HRESULT CRectEffects::Re_Initialize()
{
	SAFE_DELETE_ARRAY(m_pRectInstances);
	SAFE_DELETE_ARRAY(m_pInstancingDatas);
	static_cast<CRect_Instance*>(Get_Component<CMesh>().front())->ReMake_Instance(m_tCreateData.iNumInstance);

	m_pRectInstances = new VTXRECTINSTANCE[m_tCreateData.iNumInstance];
	ZeroMemory(m_pRectInstances, sizeof(VTXRECTINSTANCE) * m_tCreateData.iNumInstance);

	m_pInstancingDatas = new INSTANCING_DATA[m_tCreateData.iNumInstance];

	return Initialize();
}

HRESULT CRectEffects::Start()
{
	CGameObject::Start();
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, "g_vFlag");
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResource, this, placeholders::_1, "g_vGlowFlag");

	return S_OK;
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
		if (!m_pInstancingDatas[i].bAlive)
			continue;

		m_pInstancingDatas[i].fTimeAcc += fTimeDelta;
		m_pInstancingDatas[i].fMovingAcc += fTimeDelta;

		//3. FADE
		if (m_bLoop)
		{
			if (m_bFixed) //고정된 파티클일경우
			{
				if (INSTANCING_DATA::FADEOUTREADY <= m_pInstancingDatas[i].eCurFadeType)
				{
					if (0.f != m_fLoopTime)
					{
						if (m_fLoopTimeAcc > m_fLoopTime)
						{
							if (!Fade_Lerp(i)) //loop time 동안 fadeout 막음
								continue;
						}
					}
				}
				else
				{
					if (!Fade_Lerp(i))
						continue;
				}
			}
			else
			{
				if (!Fade_Lerp(i))
					continue;
			}

		}
		else //루프가 아닌경우 그대로 fade
		{
			if (!Fade_Lerp(i))
				continue;
		}

		

		if (m_iPassType != VTXRECTINSTANCE_PASS_ANIMATION && m_iPassType != VTXRECTINSTANCE_PASS_ANIMATIONALPHA &&
			m_iPassType != VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
			m_pRectInstances[i].vColor = m_pInstancingDatas[i].vColor;
		else
			m_pRectInstances[i].vColor.w = m_pInstancingDatas[i].vColor.w;



		_float4 vOriginPos;
		vOriginPos = m_pRectInstances[i].vTranslation;


		//FadeIn시작되어야 이동시작
		if (m_pInstancingDatas[i].eCurFadeType != FADEINREADY)
		{
			//1. 속도 변화.
			m_pInstancingDatas[i].fSpeed += m_pInstancingDatas[i].fSpeedChangeSpeed * fTimeDelta;

			if (m_bZeroSpeedDisable)
			{
				if (m_pInstancingDatas[i].fOriginSpeed < 0.f)
				{
					if (m_pInstancingDatas[i].fSpeed >= 0.f)
					{

						Dead_Instance(i);
						continue;
					}
				}
				else if (m_pInstancingDatas[i].fSpeed <= 0.f)
				{

					Dead_Instance(i);
					continue;
				}
			}
			
			vOriginPos += m_pInstancingDatas[i].vDir * m_pInstancingDatas[i].fSpeed * fTimeDelta;

			vOriginPos = Switch_CurveType(vOriginPos, i);


			

			//2. 자유낙하
			_float fFreeFallY = 0.f;
			_float fFreeFallPower = 0.f;

			m_pInstancingDatas[i].fAcc += m_pInstancingDatas[i].fPlusAcc * fTimeDelta;

			fFreeFallY = m_pInstancingDatas[i].fOriginY + ((m_pInstancingDatas[i].fJumpPower * m_pInstancingDatas[i].fAcc) - (m_pInstancingDatas[i].fGravity * 0.5f * m_pInstancingDatas[i].fAcc * m_pInstancingDatas[i].fAcc));

			//이동량 = 현재 Y - 이전 Y
			fFreeFallPower = fFreeFallY - m_pInstancingDatas[i].fPrevY;

			m_pInstancingDatas[i].fPrevY = fFreeFallY;

			vOriginPos.y += fFreeFallPower;

			if (vOriginPos.y < m_pInstancingDatas[i].fCurGroundY)
			{
				Dead_Instance(i);
				continue;
			}
		}

		//dir의 x값을 회전양으로 써

		//회전은 z축 회전만
		if (m_bBillBoard)
		{
			m_pInstancingDatas[i].vTurnDir.x += m_pInstancingDatas[i].fTurnSpeed * fTimeDelta;

			_float4 vRotRight, vRotUp, vRotLook;

			vRotRight = vRight;
			vRotUp = vUp;
			vRotLook = vLook;

			if (m_pInstancingDatas[i].fTurnSpeed > 0.f)
			{
				_float4x4 matRot;

				matRot = XMMatrixRotationAxis(vCamLook.XMLoad(), m_pInstancingDatas[i].vTurnDir.x);

				vRotLook = vLook.MultiplyNormal(matRot);
				vRotRight = vRight.MultiplyNormal(matRot);
				vRotUp = vUp.MultiplyNormal(matRot);
			}
			

			//m_pInstancingDatas[i].vScale = _float4(0.1f, 0.1f, 0.1f, 1.f);
			m_pRectInstances[i].vRight = vRotRight.Normalize() * m_pInstancingDatas[i].vScale.x;
			m_pRectInstances[i].vUp = vRotUp.Normalize() * m_pInstancingDatas[i].vScale.y;
			m_pRectInstances[i].vLook = vRotLook.Normalize() * m_pInstancingDatas[i].vScale.z;
		}
		else 
		{
			_float4 vRotRight, vRotUp, vRotLook;

			vRotRight = m_pRectInstances[i].vRight;
			vRotUp = m_pRectInstances[i].vUp;
			vRotLook = m_pRectInstances[i].vLook;

			if (m_pInstancingDatas[i].fTurnSpeed > 0.f)
			{
				_float4x4 matRot;

				matRot = XMMatrixRotationAxis(vRotRight.Normalize().XMLoad(), m_pInstancingDatas[i].fTurnSpeed * fTimeDelta);

				vRotLook = vRotLook.MultiplyNormal(matRot);
				vRotRight = vRotRight.MultiplyNormal(matRot);
				vRotUp = vRotUp.MultiplyNormal(matRot);
			}

			m_pRectInstances[i].vRight = vRotRight.Normalize() * m_pInstancingDatas[i].vScale.x;
			m_pRectInstances[i].vUp = vRotUp.Normalize() * m_pInstancingDatas[i].vScale.y;
			m_pRectInstances[i].vLook = vRotLook.Normalize() * m_pInstancingDatas[i].vScale.z;
		}
		m_pRectInstances[i].vTranslation = vOriginPos;

		if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
			m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
			Update_Animation(i);
		
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
		/*_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();

		sort(m_pRectInstances, m_pRectInstances + (m_tCreateData.iNumInstance - 1), [vCamPos](VTXRECTINSTANCE p1, VTXRECTINSTANCE p2)
			{
				_float fDist1 = XMVector3Length((XMLoadFloat4(&vCamPos) - XMLoadFloat4(&p1.vTranslation))).m128_f32[0];
				_float fDist2 = XMVector3Length((XMLoadFloat4(&vCamPos) - XMLoadFloat4(&p2.vTranslation))).m128_f32[0];

				return fDist1 > fDist2;
			});*/

		//sorting해도 InstancingData를 어케 해야하지
	}
	

	static_cast<CRect_Instance*>(GET_COMPONENT(CMesh))->ReMap_Instances(m_pRectInstances);

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
		m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEINREADY;
		m_pInstancingDatas[i].vColor.w = 0.f;
		m_pRectInstances[i].vColor.w = 0.f;
		Reset_Instance(i);
	}

}

void CRectEffects::Dead_Instance(_uint iIndex)
{
	if (m_bLoop)
	{
		if(m_fLoopTime == 0)
			Reset_Instance(iIndex);
		else if(m_fLoopTimeAcc <= m_fLoopTime)
			Reset_Instance(iIndex);
		else if(m_fLoopTimeAcc > m_fLoopTime)
		{
			//m_fLoopTimeAcc = 0.f;
			//m_pInstancingDatas[iIndex].fTimeAcc = 0.f;
			m_pInstancingDatas[iIndex].bAlive = false;
			m_iNumDead++;
			m_pRectInstances[iIndex].vColor.w = 0.f;
			m_pInstancingDatas[iIndex].fSpeed = m_pInstancingDatas[iIndex].fOriginSpeed;
		}
	}
	else
	{
		m_pInstancingDatas[iIndex].bAlive = false;
		m_iNumDead++;
		m_pRectInstances[iIndex].vColor.w = 0.f;
		m_pInstancingDatas[iIndex].fSpeed = m_pInstancingDatas[iIndex].fOriginSpeed;
	}
	//m_pInstancingDatas[iIndex].bAlive = true;
	//m_iNumDead++;
	//m_pRectInstances[iIndex].vColor.w = 0.f;
	//m_pInstancingDatas[iIndex].fSpeed = m_pInstancingDatas[iIndex].fOriginSpeed;
	//작아지게해서 안보이게
}

void CRectEffects::Set_NewStartPos(_uint iIndex)
{
	//offset 잡기

		_float4 vStartPos = m_pInstancingDatas[iIndex].vStartPureLocalPos;
		_float4 vStartDir = m_pInstancingDatas[iIndex].vStartPureLocalDir;
		_float4 vStartRight = m_pInstancingDatas[iIndex].vStartPureLocalRight;

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
		m_pRectInstances[iIndex].vTranslation = vStartPos;
		m_pInstancingDatas[iIndex].vDir = vStartDir;
		m_pInstancingDatas[iIndex].vRight = vStartRight;

		//회전시켜놓기
		
		//Up은 dir이고
		//Right는 0 1 0 임
		//

	/*	_float4 vUp = m_pInstancingDatas[iIndex].vDir;
		vUp.z = 0.f;
		vUp.Normalize();
		_float4 vRight = { 0.f, 1.f, 0.f, 0.f };
		_float4 vLook = vRight.Cross(vUp).Normalize();
		vRight = vUp.Cross(vLook).Normalize();

		m_pRectInstances[iIndex].vRight = vRight.MultiplyNormal(m_matTrans).Normalize();
		m_pRectInstances[iIndex].vUp = vUp.MultiplyNormal(m_matTrans).Normalize();
		m_pRectInstances[iIndex].vLook = vLook.MultiplyNormal(m_matTrans).Normalize();*/
		if (!m_bLoop)
		{
			_float4 vLook = m_pInstancingDatas[iIndex].vDir;
	
			m_pRectInstances[iIndex].vUp = vLook.Normalize();

			_float4 vUp = { 0.f, 1.f, 0.f };
			if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
				(vLook.y > -1.1f && vLook.y < -0.9f)
				)
				vUp = _float4(0.f, 0.f, 1.f, 0.f);

			vUp.Normalize();
			_float4 vRight = vUp.Cross(vLook);
			m_pRectInstances[iIndex].vRight = vRight.Normalize();

			vUp = vLook.Cross(vRight);
			m_pRectInstances[iIndex].vLook = vUp.Normalize();
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
	pReadFile->read((char*)&m_bFixed, sizeof(_bool));

	pReadFile->read((char*)&m_iWidthSize, sizeof(_uint));
	pReadFile->read((char*)&m_iHeightSize, sizeof(_uint));
	pReadFile->read((char*)&m_fDuration, sizeof(_float));
	pReadFile->read((char*)&m_fDurationRange, sizeof(_float));
	pReadFile->read((char*)&m_fDissolvePower, sizeof(_float));

	pReadFile->read((char*)&m_eCurveType, sizeof(CURVE_TYPE));

	pReadFile->read((char*)&m_tCreateData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));



	if (m_tCreateData.iOffsetPositionCount > 0)
	{
		m_tCreateData.pOffsetPositions = new _float4[m_tCreateData.iOffsetPositionCount];
		pReadFile->read((char*)m_tCreateData.pOffsetPositions, sizeof(_float4) * m_tCreateData.iOffsetPositionCount);

	}

	m_bSorting = true;

	return Initialize_Prototype();
}

void CRectEffects::Update_Animation(_uint iIndex)
{
	m_pInstancingDatas[iIndex].vTurnDir.y += fDT(0);

	//UV넘기는 코드

	if (m_pInstancingDatas[iIndex].vTurnDir.y > m_pInstancingDatas[iIndex].fDuration)
	{
		m_pInstancingDatas[iIndex].vTurnDir.y = 0.f;

		m_pRectInstances[iIndex].vColor.x += 1.f;
		if (m_pRectInstances[iIndex].vColor.x >= m_iWidthSize)
		{
			m_pRectInstances[iIndex].vColor.x = 0.f;
			m_pRectInstances[iIndex].vColor.y += 1.f;
			if (m_pRectInstances[iIndex].vColor.y >= m_iHeightSize)
			{
				//여기 들어왔다 : 한바퀴돌아서 1순한거임
				m_pRectInstances[iIndex].vColor.x = m_iWidthSize - 1;
				m_pRectInstances[iIndex].vColor.y = m_iHeightSize - 1;
				Dead_Instance(iIndex);
			}
		}
	}
}

void CRectEffects::Reset_Instance(_uint iIndex)
{
	/*m_pRectInstances[iIndex].vRight = *((_float4*)(&(m_pInstancingDatas[iIndex].StartMatrix.m[0])));
	m_pRectInstances[iIndex].vUp = *((_float4*)(&(m_pInstancingDatas[iIndex].StartMatrix.m[1])));
	m_pRectInstances[iIndex].vLook = *((_float4*)(&(m_pInstancingDatas[iIndex].StartMatrix.m[2])));*/

	Set_NewStartPos(iIndex);

	m_pInstancingDatas[iIndex].vScale = m_pInstancingDatas[iIndex].vStartScale;

	if (!m_bFixed)
	{
		if (0 < m_fLoopTimeAcc)
		{
			m_pInstancingDatas[iIndex].vColor.w = 0.f;
			m_pRectInstances[iIndex].vColor.w = 0.f;
			m_pInstancingDatas[iIndex].eCurFadeType = INSTANCING_DATA::FADEINREADY;

		}
	}

	if(!m_bLoop)
		m_pInstancingDatas[iIndex].eCurFadeType = INSTANCING_DATA::FADEINREADY;

	m_pInstancingDatas[iIndex].fTimeAcc = 0.f;


	m_pInstancingDatas[iIndex].vTurnDir.x = 0.f;
	m_pInstancingDatas[iIndex].vTurnDir.y = 0.f;
	m_pInstancingDatas[iIndex].vTurnDir.z = 0.f;

	if (m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA ||
		m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONDISSOLVE)
	{
		m_pRectInstances[iIndex].vColor.x = 0.f;
		m_pRectInstances[iIndex].vColor.y = 0.f;
	}

	//Jump
	m_pInstancingDatas[iIndex].fPrevY = m_pRectInstances[iIndex].vTranslation.y;
	m_pInstancingDatas[iIndex].fOriginY = m_pRectInstances[iIndex].vTranslation.y;
	m_pInstancingDatas[iIndex].fAcc = 0.f;

	m_pInstancingDatas[iIndex].bAlive = true;
}

_float4 CRectEffects::Switch_CurveType(_float4 vPos, _uint iIdx)
{
	_float fY;

	switch (m_eCurveType)
	{
	case Client::CURVE_LINEAR:
		break;
	case Client::CURVE_SIN:
		fY = m_pInstancingDatas[iIdx].fCurvePower * sinf(m_pInstancingDatas[iIdx].fCurveFrequency * m_pInstancingDatas[iIdx].fMovingAcc); // a sin(bx)

		vPos.x += fY * m_pInstancingDatas[iIdx].vRight.x;
		vPos.y += fY * m_pInstancingDatas[iIdx].vRight.y;
		vPos.z += fY * m_pInstancingDatas[iIdx].vRight.z;
		break;
	case Client::CURVE_END:
		break;
	default:
		break;
	}
	

	return vPos;
}