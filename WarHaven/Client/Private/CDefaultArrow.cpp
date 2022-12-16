#include "stdafx.h"
#include "CDefaultArrow.h"

#include "UsefulHeaders.h"

CDefaultArrow::CDefaultArrow()
{
}
CDefaultArrow::~CDefaultArrow()
{
}

HRESULT CDefaultArrow::Initialize_Protoype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt.fbx")))
        return E_FAIL;

    if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
        return E_FAIL;
    

    return __super::Initialize_Prototype();
}
