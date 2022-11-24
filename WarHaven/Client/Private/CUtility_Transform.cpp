#include "stdafx.h"
#include "CUtility_Transform.h"
#include "GameInstance.h"
#include "Transform.h"
#include "GameObject.h"

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

void CUtility_Transform::Turn_ByAngle(_float4x4& Matrix, _float4 vAxis, _float fAngle)
{
	_matrix Mat = Matrix.XMLoad();
	_float4 vRight = Mat.r[0];
	_float4 vUp = Mat.r[1];
	_float4 vLook = Mat.r[2];

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vAxis.Normalize().XMLoad(), ToRadian(fAngle));

	vLook = vLook.MultiplyNormal(matRot);
	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);

	Mat.r[0] = vRight.XMLoad();
	Mat.r[1] = vUp.XMLoad();
	Mat.r[2] = vLook.XMLoad();

	Matrix = Mat;
}

void CUtility_Transform::Turn_ByAngle_Up(_float4x4& Matrix, _float fAngle)
{
	_matrix Mat = Matrix.XMLoad();
	_float4 vRight = Mat.r[0];
	_float4 vUp = Mat.r[1];
	_float4 vLook = Mat.r[2];

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vUp.Normalize().XMLoad(), ToRadian(fAngle));

	vLook = vLook.MultiplyNormal(matRot);
	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);

	Mat.r[0] = vRight.XMLoad();
	Mat.r[1] = vUp.XMLoad();
	Mat.r[2] = vLook.XMLoad();

	Matrix = Mat;
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

_float4 CUtility_Transform::Get_ProjPos(CTransform* pTransform, _float4 vOffsetPos)
{
	_float4 vPos = pTransform->Get_World(WORLD_POS);
	vPos += vOffsetPos;
	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

	vPos = vPos.MultiplyCoord(matVP);

	vPos.x *= g_iWinCX * 0.5f;
	vPos.y *= g_iWinCY * 0.5f;
	vPos.z = 0.1f;
	vPos.w = 1.f;

	return vPos;
}

_float4 CUtility_Transform::Get_Dir_2D(CTransform* pFrom, CTransform* pTo)
{
	_float4 vDir = (pTo->Get_World(WORLD_POS) - pFrom->Get_World(WORLD_POS));
	vDir.y = 0.f;
	return vDir.Normalize();
}

_float CUtility_Transform::Get_LookRotateAngle(_float4 vLook)
{
	_float4 vOriginLook = vLook;
	vOriginLook.y = 0.f;
	vOriginLook.Normalize();

	_float4 vRealLook = vLook;
	vRealLook.Normalize();


	_float fCosTheta = vOriginLook.Dot(vRealLook);

	if (vRealLook.y < 0.f)
		fCosTheta *= -1.f;

	return fCosTheta;
}

_float CUtility_Transform::Get_FromCameraDistance(CGameObject* pObject)
{
	_float4 vObjPos = pObject->Get_Transform()->Get_World(WORLD_POS);
	_float4 vViewPos = GAMEINSTANCE->Get_ViewPos();

	_float fDisX = vViewPos.x - vObjPos.x;
	_float fDisZ = vViewPos.z - vObjPos.z;

	_float fDis = sqrt(pow(fDisX, 2) + pow(fDisZ, 2));

	return fDis;
}
