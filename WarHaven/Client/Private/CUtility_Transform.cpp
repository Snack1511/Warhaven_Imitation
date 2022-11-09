#include "stdafx.h"
#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Transform.h"

void CUtility_Transform::Turn_ByAngle(CTransform* pTransform, _float4 vAxis, _float fAngle)
{
	_float4 vLook = pTransform->Get_MyWorld(WORLD_LOOK);
	_float4 vRight = pTransform->Get_MyWorld(WORLD_RIGHT);
	_float4 vUp = pTransform->Get_MyWorld(WORLD_UP);

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vAxis.XMLoad(), ToRadian(fAngle));

	vLook = vLook.MultiplyNormal(matRot);
	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());
	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
}

void CUtility_Transform::Turn(CTransform* pTransform, _float4 vAxis, _float fSpeed)
{
	_float4 vRight = pTransform->Get_MyWorld(WORLD_RIGHT);
	_float4 vUp = pTransform->Get_MyWorld(WORLD_UP);
	_float4 vLook = pTransform->Get_MyWorld(WORLD_LOOK);

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vAxis.XMLoad(), fSpeed * fDT(0));

	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);
	vLook = vLook.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());
	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
}

void CUtility_Transform::Rotation(CTransform* pTransform, _float4 vAxis, _float fAngle)
{
	_float4 vLook = _float4(0.f, 0.f, 1.f, 0.f);
	_float4 vRight = _float4(1.f, 0.f, 0.f, 0.f);
	_float4 vUp = _float4(0.f, 1.f, 0.f, 0.f);

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vAxis.XMLoad(), ToRadian(fAngle));

	vLook = vLook.MultiplyNormal(matRot);
	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());
	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
}



_float4 CUtility_Transform::LookAt(CTransform* pTransform, _float4 vPos, _bool bNoY)
{
	_float4 vMyPos = pTransform->Get_World(WORLD_POS);
	_float4 vLook = vPos - vMyPos;

	if (bNoY)
		vLook.y = 0.f;

	pTransform->Set_Look(vLook);

	return vLook;
}

_float4 CUtility_Transform::Get_ProjPos(CTransform* pTransform)
{
	_float4 vPos = pTransform->Get_World(WORLD_POS);
	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

	vPos = vPos.MultiplyCoord(matVP);

	vPos.x *= g_iWinCX * 0.5f;
	vPos.y *= g_iWinCY * 0.5f;
	vPos.z = 0.1f;
	vPos.w = 1.f;

	return vPos;
}
