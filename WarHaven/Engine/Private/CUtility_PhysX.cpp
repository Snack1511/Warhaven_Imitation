#include "CUtility_PhysX.h"

PxVec3 CUtility_PhysX::To_PxVec3(_float4 vVector)
{
	return PxVec3(vVector.x, vVector.y, vVector.z);
}

PxExtendedVec3 CUtility_PhysX::To_PxExtendedVec3(_float4 vVector)
{
	return PxExtendedVec3(vVector.x, vVector.y, vVector.z);
}

_float4x4 CUtility_PhysX::To_Matrix(PxTransform& pxTransform)
{
	XMMATRIX	QuaternionMatrix;

	_float4 vQuaternion, vPos;
	
	memcpy(&vQuaternion, &pxTransform.q, sizeof(_float4));

	vPos.x = pxTransform.p.x;
	vPos.y = pxTransform.p.y;
	vPos.z = pxTransform.p.z;
	vPos.w = 1.f;

	QuaternionMatrix = XMMatrixRotationQuaternion(vQuaternion.XMLoad());

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	vRight = XMVector3TransformNormal(vRight, QuaternionMatrix);
	vUp = XMVector3TransformNormal(vUp, QuaternionMatrix);
	vLook = XMVector3TransformNormal(vLook, QuaternionMatrix);

	QuaternionMatrix.r[0] = XMVector3Normalize(vRight);
	QuaternionMatrix.r[1] = XMVector3Normalize( vUp);
	QuaternionMatrix.r[2] = XMVector3Normalize(vLook);
	QuaternionMatrix.r[3] = vPos.XMLoad();


	return _float4x4(QuaternionMatrix);
}

_float4 CUtility_PhysX::To_Vector(const PxVec3& pxVec3)
{
	return _float4(pxVec3.x, pxVec3.y, pxVec3.z, 1.f);
}

_float4 CUtility_PhysX::To_Vector(const PxExtendedVec3& pxExtendedVec3)
{
	return _float4(pxExtendedVec3.x, pxExtendedVec3.y, pxExtendedVec3.z, 1.f);
}
