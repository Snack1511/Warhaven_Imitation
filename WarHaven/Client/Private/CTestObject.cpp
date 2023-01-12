#include "stdafx.h"
#include "CTestObject.h"

#include "UsefulHeaders.h"

#include "CMesh_Cube.h"

CTestObject::CTestObject()
{
}

CTestObject::~CTestObject()
{
}

HRESULT CTestObject::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	//CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/meshes/testlancermesh_10_anim.fbx");
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/lancer/SKEL_Lancer_Base_R.fbx");
	pAnimator->Initialize();

	if (!pAnimator)
		return E_FAIL;

	Add_Component(pAnimator);



	_float4x4 matIdentity;
	matIdentity.Identity();

	wstring wstrFilePath = L"../bin/resources/meshes/characters/lancer/Lancer.fbx";

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrFilePath.c_str(),
		DEFAULT_TRANS_MATRIX
	);

	//pModel->Add_Model(wstrFilePath.c_str(), 1);
	//pModel->Add_Model(L"../bin/resources/meshes/testlancermesh_10.fbx", 1);
	pModel->Add_Model(L"../bin/resources/meshes/characters/Lancer/body/SK_Lancer0001_Body_A00_10.fbx", 1);
	//pModel->Add_Model(L"../bin/resources/meshes/characters/Lancer/head/SK_Lancer0000_Face_A00_20.fbx", 2);
	pModel->Add_Model(L"../bin/resources/meshes/lancerhead.fbx", 2);
	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);


	return S_OK;
}

HRESULT CTestObject::Initialize()
{
	return S_OK;
}

HRESULT CTestObject::Start()
{
	__super::Start();

	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 9);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 9, 0.1f);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 9, 2.f);

	return S_OK;
}
