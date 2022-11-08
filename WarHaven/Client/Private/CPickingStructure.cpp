#include "stdafx.h"
#include "CPickingStructure.h"
#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CNavigation.h"
#include "CCell.h"
#include "MeshContainer.h"
CPickingStructure::CPickingStructure()
{
}

CPickingStructure::~CPickingStructure()
{
}

HRESULT CPickingStructure::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        assert(0);

   _float4x4
        TransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f)
        * XMMatrixRotationY(XMConvertToRadians(270.0f));

    CModel* pModel = CModel::Create(0, TYPE_NONANIM, TEXT("../Bin/Resources/Meshes/Map/Structure/TestObject/Map_BootCamp_Asset_0_HLOD_0_BP_Module_Tower_7.FBX"), TransformMatrix);

    Add_Component(pModel);


    return S_OK;
}

HRESULT CPickingStructure::Initialize()
{
    if (FAILED(__super::Initialize()))
        assert(0);

    //m_pModelCom->Set_NewModel(wstring(TEXT("../Bin/Resources/Meshes/Map/Structure/TestObject/Map_BootCamp_Asset_0_HLOD_0_BP_Module_Tower_7.FBX")));
    return S_OK;
}

HRESULT CPickingStructure::Start()
{
    __super::Start();

    //m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f, 0.f));
    //m_pModelCom->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);
    return S_OK;
}

void CPickingStructure::OnEnable()
{
    __super::OnEnable();
}

void CPickingStructure::OnDisable()
{
    __super::OnDisable();
}


void CPickingStructure::My_Tick()
{
}

void CPickingStructure::My_LateTick()
{
}
