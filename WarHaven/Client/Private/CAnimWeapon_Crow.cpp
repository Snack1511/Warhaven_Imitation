#include "stdafx.h"
#include "CAnimWeapon_Crow.h"

#include "UsefulHeaders.h"
#include "CUtility_PhysX.h"

#include "HIerarchyNode.h"
#include "CColorController.h"

#include "CCrowBoom.h"

CAnimWeapon_Crow::CAnimWeapon_Crow()
{
}

CAnimWeapon_Crow::~CAnimWeapon_Crow()
{
}

CAnimWeapon_Crow* CAnimWeapon_Crow::Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName, 
	_float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimWeapon_Crow* pInstance = new CAnimWeapon_Crow;

	pInstance->m_pOwnerUnit = pOwnerUnit;

	if (FAILED(pInstance->SetUp_Model(wstrModelFilePath, wstrAnimFilePath, strBoneName, fRadianX, fRadianY, fRadianZ)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CAnimWeapon_Crow");
		return nullptr;
	}
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CAnimWeapon_Crow");
		return nullptr;
	}

	return pInstance;
}


void CAnimWeapon_Crow::On_ChangePhase(ePhyxState eNextPhase)
{
	m_eCurPhase = eNextPhase;
}

void CAnimWeapon_Crow::Shoot_Crow(_float4 vShootPos, _float4 vShootDir)
{
	
	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = vShootPos;

	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	//pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 25.f);
	_float4 vDir = vShootDir;
	vDir *= m_fMaxSpeed;
	//pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	//pActor->addTorque(CUtility_PhysX::To_PxVec3(vDir));
	//m_pActor = pActor;

	m_eCurPhase = eSHOOT;
	m_vStartPosition = vCurPos;

}

void CAnimWeapon_Crow::Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pAnimator->Set_CurAnimIndex(0, iAnimIndex);
	m_pAnimator->Set_InterpolationTime(0, iAnimIndex, fInterpolateTime);
	m_pAnimator->Set_AnimSpeed(0, iAnimIndex, fAnimSpeed);
}


HRESULT CAnimWeapon_Crow::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	m_pCrowBoom = CCrowBoom::Create();
	if (!m_pCrowBoom)
		return E_FAIL;
	m_pCrowBoom->Initialize();


    return S_OK;
}

HRESULT CAnimWeapon_Crow::Initialize()
{
	XMStoreFloat4x4(&m_OwnerBoneOffsetMatrix, XMMatrixIdentity());
	m_OwnerBoneOffsetMatrix.XMLoad().r[3].m128_f32[0] -= 0.5f;
	m_OwnerBoneOffsetMatrix.XMLoad().r[3].m128_f32[1] += 0.1f;
	m_OwnerBoneOffsetMatrix.XMLoad().r[3].m128_f32[2] += 0.5f;
	//__super::Initialize();

    return S_OK;
}

HRESULT CAnimWeapon_Crow::Start()
{
	__super::Start();

	//if (m_pCrowBoom)
	//{
	//	CREATE_GAMEOBJECT(m_pCrowBoom, GROUP_EFFECT);
	//	DISABLE_GAMEOBJECT(m_pCrowBoom);
	//}

	Set_AnimIndex(10, 0.1f, 1.f);

    return S_OK;
}

HRESULT CAnimWeapon_Crow::SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, _float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, wstrAnimFilePath.c_str());
	pAnimator->Initialize();
	if (!pAnimator)
		return E_FAIL;
	Add_Component(pAnimator);
	m_pAnimator = pAnimator;

	_float4x4 matIdentity;
	matIdentity.Identity();

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrModelFilePath,
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(fRadianZ)) * XMMatrixRotationY(XMConvertToRadians(fRadianY)) * XMMatrixRotationX(XMConvertToRadians(fRadianX))
	);
	pModel->Add_Model(wstrModelFilePath, 1);
	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	

	m_pOwnerBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(strBoneName.c_str());
	if (!m_pOwnerBone)
		return E_FAIL;


	/* PhysX */
	//CMeshContainer* pMesh = (pModel->Get_MeshContainers().back().second);

	//FACEINDICES32* pIndices = pMesh->CMesh::Get_Indices();
	//_uint iNumPrimitive = pMesh->Get_NumPrimitive();

	//_uint iNumVertices = pMesh->Get_NumVertices();
	//_float3* pVerticesPos = pMesh->Get_VerticesPos();

	//GAMEINSTANCE->Create_ConvexMesh(
	//	pVerticesPos,
	//	iNumVertices,
	//	pIndices,
	//	iNumPrimitive,
	//	&m_pConvexMesh);

	//GAMEINSTANCE->Create_ConvexMesh(
	//	pVerticesPos,
	//	iNumPrimitive,
	//	pIndices,
	//	iNumVertices,
	//	&m_pConvexMesh);

	//if (!m_pConvexMesh)
	//	return E_FAIL;

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);


	return S_OK;
}

void CAnimWeapon_Crow::Late_Tick()
{

	switch (m_eCurPhase)
	{
	case Client::CAnimWeapon_Crow::eIDLE:
		if (m_pOwnerBone)
		{
			_float4x4		matBone = m_pOwnerBone->Get_BoneMatrix();

			m_pTransform->Get_Transform().matMyWorld = matBone * m_OwnerBoneOffsetMatrix;

			m_pTransform->Make_WorldMatrix();
		}
		break;
	case Client::CAnimWeapon_Crow::eSHOOT:
	{
		/* PhysX 따라가기 */
	//	PxTransform tTransform = m_pActor->getGlobalPose();
	//	_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
	//	m_pTransform->Get_Transform().matMyWorld = matPhysX;

	//	m_pTransform->Make_WorldMatrix();

	//	_float fPower = CUtility_PhysX::To_Vector(m_pActor->getLinearVelocity()).Length();
	//	_float fLength = (m_vStartPosition - m_pTransform->Get_World(WORLD_POS)).Length();

		//if (fPower < 25.f || fLength > m_fMaxDistance)
		{
			m_pCrowBoom->Boom(m_pOwnerUnit->Get_OwnerPlayer(), m_pTransform->Get_World(WORLD_POS));
			On_ChangePhase(eHIT);
		}
			
	}
	break;
	case Client::CAnimWeapon_Crow::eHIT:
		/* 땅에 떨어졌을 떄 */
	{
		/* PhysX 따라가기 */
		//PxTransform tTransform = m_pActor->getGlobalPose();
		//_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
		//m_pTransform->Get_Transform().matMyWorld = matPhysX;

//		m_pTransform->Make_WorldMatrix();

		m_fLoopTimeAcc += fDT(0);
		if (m_fLoopTimeAcc >= m_fMaxLoopTime)
		{
			m_fLoopTimeAcc = 0.f;
			//Safe_release(m_pActor);
			On_ChangePhase(eIDLE);
		}
	}
	break;

	case Client::CAnimWeapon_Crow::eEND:
		break;
	default:
		break;
	}
}

