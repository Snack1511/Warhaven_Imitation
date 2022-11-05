#include "..\Public\Collision_Manager.h"

#include "GameInstance.h"
#include "CCollider_Box.h"
#include "CCollider_Sphere.h"
#include "GameObject.h"

#include "Object_Manager.h"

IMPLEMENT_SINGLETON(CCollision_Manager)


CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{

	return S_OK;
}

void CCollision_Manager::Tick()
{
	for (_uint iRow = 0; iRow < (_uint)COL_END; ++iRow)
	{
		for (_uint iCol = iRow; iCol < (_uint)COL_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				Collider_GroupUpdate((const _uint&)iRow, (const _uint&)iCol);
			}
		}
	}

	for (_uint i = 0; i < COL_END; ++i)
	{
		for (auto iter = m_pColliderList[i].begin(); iter != m_pColliderList[i].end();)
		{
			if (!(*iter)->Is_Valid())
			{
				iter = m_pColliderList[i].erase(iter);
			}
			else
				++iter;
		}
	}




}
void CCollision_Manager::Clear_All()
{
	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pColliderList[i].clear();
	}
}
#ifdef _DEBUG

HRESULT CCollision_Manager::Render()
{
	if (KEY(F6, TAP))
	{
		m_bRender = !m_bRender;
	}

	if (!m_bRender)
		return S_OK;

	for (_uint i = 0; i < COL_END; ++i)
	{
		for (auto& pCollider : m_pColliderList[i])
		{
			if (FAILED(pCollider->Render()))
				return E_FAIL;
		}
	}
	
	return S_OK;
}
#endif

_bool CCollision_Manager::Is_OBBtoSphereCollision(CCollider* _pOBB, CCollider* _pSphere)
{
	COL_INFO_BOX LeftInfo = ((CCollider_Box*)_pOBB)->Get_ColInfo();
	COL_INFO_SPHERE RightInfo = ((CCollider_Sphere*)_pSphere)->Get_ColInfo();

	_float4 CenterDiff = RightInfo.vFinalPos - LeftInfo.vFinalPos;
	_float Dist = fabsf(CenterDiff.Length());

	_float LeftRadius = (LeftInfo.vWorldVertex[0] - LeftInfo.vFinalPos).Length();
	_float RightRadius = RightInfo.fRadius;

	if (Dist > LeftRadius + RightRadius)
	{
		return false;
	}

	// �Ʒ��� OBB �� �浹 �ڵ� ����


	return _bool();
}

_bool CCollision_Manager::Check_BeforeOBB(CCollider* _pLeft, CCollider* _pRight)
{


	return _bool();
}

HRESULT CCollision_Manager::Is_InIndex(const _uint& iIdx)
{
	if (iIdx < 0 || iIdx >= COL_END)
	{
		Call_MsgBox(L"Out of Range in COL_TYPE : CCollision_Manager ");
		return E_FAIL;
	}

	return S_OK;

}

_float4 CCollision_Manager::Compute_ColPosition(CCollider* _pLeft, CCollider* _pRight)
{
	return (static_cast<CCollider_Sphere*>(_pLeft)->Get_ColInfo().vFinalPos +
		static_cast<CCollider_Sphere*>(_pRight)->Get_ColInfo().vFinalPos)
		* 0.5f;
}

map<_ulonglong, bool>::iterator CCollision_Manager::Find_PrevColInfo(const _uint& _iLeftID, const _uint& _iRightID)
{
	map<_ulonglong, bool>::iterator iter;

	COLLIDER_ID ID;
	ID.Left_ID = _iLeftID;		// 4����Ʈ
	ID.Right_ID = _iRightID;		// 4����Ʈ
												//���ļ� ���� 8����Ʈ�� ���� �Ȱ�ħ.

	iter = m_mapColInfo.find(ID.ID);

	//�浹 ������ �ƿ� �̵�� ������ ���
	if (m_mapColInfo.end() == iter)
	{
		m_mapColInfo.insert(make_pair(ID.ID, false)); // �ƿ� ������� �ȵ��ִ� ��Ȳ�̹Ƿ� �ʿ� �߰�
		iter = m_mapColInfo.find(ID.ID); // ���� �� �ٽ� ���ͷ� �ް�
	}

	return iter;
}

HRESULT CCollision_Manager::Regist_Collider(CCollider* pCollider, const _uint& _iGroupIndex)
{
	if (FAILED(Is_InIndex(_iGroupIndex)))
		return E_FAIL;

	m_pColliderList[_iGroupIndex].push_back(pCollider);

	return S_OK;
}

HRESULT CCollision_Manager::Check_Group(const _uint& _eLeft, const _uint& _eRight)
{
	if (FAILED(Is_InIndex(_eLeft)) || FAILED(Is_InIndex(_eRight)))
		return E_FAIL;

	_uint iRow = (_uint)_eLeft;
	_uint iCol = (_uint)_eRight;

	if (iRow > iCol)
	{
		iRow = (_uint)_eRight;
		iCol = (_uint)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
		m_arrCheck[iRow] &= ~(1 << iCol);
	else
		m_arrCheck[iRow] |= (1 << iCol);

	return S_OK;
}

void CCollision_Manager::Collider_GroupUpdate(const _uint& _eLeft, const _uint& _eRight)
{
	list<CCollider*>& LeftList = m_pColliderList[_eLeft];
	list<CCollider*>& RightList = m_pColliderList[_eRight];

	map<_ulonglong, _bool>::iterator iter;

	for (auto LeftIter = LeftList.begin(); LeftIter != LeftList.end(); ++LeftIter)
	{
		CGameObject* pLeftOwner = (*LeftIter)->Get_Owner();

		for (auto RightIter = RightList.begin(); RightIter != RightList.end(); ++RightIter)
		{
			CGameObject* pRightOwner = (*RightIter)->Get_Owner();

			if ((*LeftIter) == (*RightIter)) // �� �ڽŰ� �浹 ����
			{
				continue;
			}

			iter = Find_PrevColInfo((*LeftIter)->Get_ID(), (*RightIter)->Get_ID());

			if (iter->second) // ������ �浹���� ��Ȳ
			{
				if (!(*LeftIter)->Is_Valid() || !(*RightIter)->Is_Valid()) // ���� �ϳ��� ��ȿ���� ���� ���
				{
					pLeftOwner->CallBack_CollisionExit(pRightOwner, _eRight);
					pRightOwner->CallBack_CollisionExit(pLeftOwner, _eLeft);
					(*LeftIter)->Set_Col(false);
					(*RightIter)->Set_Col(false);

					iter->second = false;
				}
				else // �� �� ��ȿ�ϱ� ��
				{
					if (Is_Collision((*LeftIter), (*RightIter))) // true�� stay
					{
						pLeftOwner->CallBack_CollisionStay(pRightOwner, _eRight);
						pRightOwner->CallBack_CollisionStay(pLeftOwner, _eLeft);
					}
					else
					{
						pLeftOwner->CallBack_CollisionExit(pRightOwner, _eRight);
						pRightOwner->CallBack_CollisionExit(pLeftOwner, _eLeft);
						(*LeftIter)->Set_Col(false);
						(*RightIter)->Set_Col(false);

						iter->second = false;
					}
				}
			}
			else // ���� �����ӿ� �浹 x
			{
				if (!(*LeftIter)->Is_Valid() || !(*RightIter)->Is_Valid()) // ���� �ϳ��� ��ȿ���� ���� ���
					continue;
				else // �� �� ��ȿ
				{
					if (Is_Collision((*LeftIter), (*RightIter))) // true�� Enter
					{
						_float4 vColPosition = Compute_ColPosition((*LeftIter), (*RightIter));

						pLeftOwner->CallBack_CollisionEnter(pRightOwner, _eRight, vColPosition);
						pRightOwner->CallBack_CollisionEnter(pLeftOwner, _eLeft, vColPosition);
						(*LeftIter)->Set_Col(true);
						(*RightIter)->Set_Col(true);

						iter->second = true;
					}
				}
			}
		}
	}
}

bool CCollision_Manager::Is_Collision(CCollider* _pLeft, CCollider* _pRight)
{
	if (_pLeft->Get_ColType() == CT_SPHERE && _pRight->Get_ColType() == CT_SPHERE)
	{
		return Is_SphereCollision(_pLeft, _pRight);
	}
	else if (_pLeft->Get_ColType() == CT_SPHERE && _pRight->Get_ColType() == CT_BOX)
	{
		return Is_OBBtoSphereCollision(_pRight, _pLeft);
	}
	else if (_pLeft->Get_ColType() == CT_BOX && _pRight->Get_ColType() == CT_SPHERE)
	{
		return Is_OBBtoSphereCollision(_pLeft, _pRight);
	}
	else
	{
		return Is_OBBCollision(_pLeft, _pRight);
	}
}

bool CCollision_Manager::Is_OBBCollision(CCollider* _pLeft, CCollider* _pRight)
{
	//OBBINFO LeftBox = _pLeft->Get_ObbInfo();
	//OBBINFO RightBox = _pRight->Get_ObbInfo();

	//D3DXVECTOR3 Axis_c1[3] = {
	//	LeftBox.vWorldVertex[1] - LeftBox.vWorldVertex[0],
	//	LeftBox.vWorldVertex[3] - LeftBox.vWorldVertex[0],
	//	LeftBox.vWorldVertex[4] - LeftBox.vWorldVertex[0] };

	//D3DXVECTOR3 Axis_c2[3] = {
	//	RightBox.vWorldVertex[1] - RightBox.vWorldVertex[0],
	//	RightBox.vWorldVertex[3] - RightBox.vWorldVertex[0],
	//	RightBox.vWorldVertex[4] - RightBox.vWorldVertex[0] };

	//// 2. ���� �������� �˻� 
	//for (int i = 0; i < 3; ++i)
	//{
	//	// ������ �Ǵ� �� ����
	//	D3DXVECTOR3 Axis_norm;
	//	D3DXVec3Normalize(&Axis_norm, &Axis_c1[i]);

	//	// 2 - 1. �࿡���� ���� A�� B ����
	//	map<float, UINT> mValue;
	//	UINT minIndex, maxIndex;

	//	//���� ª�������� ������ ã�Ƴ���.
	//	for (int v = 0; v < sizeof(RightBox.vWorldVertex) / sizeof(D3DXVECTOR3); ++v)
	//	{
	//		mValue.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[v])), v));
	//	}


	//	// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ

	//	map<float, UINT>::iterator iter;
	//	iter = mValue.begin();

	//	for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
	//	--iter;

	//	minIndex = mValue.begin()->second;
	//	maxIndex = iter->second;


	//	// padding
	//	maxIndex = maxIndex;

	//	// 1, 3, 4 ������ ���� ŭ
	//	FLOAT A, B, Dist;
	//	D3DXVECTOR3 vec;
	//	switch (i)
	//	{
	//	case 0:
	//		vec = LeftBox.vWorldVertex[1] - LeftBox.vWorldVertex[0];
	//		break;

	//	case 1:
	//		vec = LeftBox.vWorldVertex[3] - LeftBox.vWorldVertex[0];
	//		break;

	//	case 2:
	//		vec = LeftBox.vWorldVertex[4] - LeftBox.vWorldVertex[0];
	//		break;
	//	}

	//	A = fabsf(D3DXVec3Length(&vec));

	//	D3DXVECTOR3 max = RightBox.vWorldVertex[maxIndex];
	//	D3DXVECTOR3 min = RightBox.vWorldVertex[minIndex];

	//	FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[maxIndex]));
	//	FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[minIndex]));

	//	B = fabs(B1 - B2);

	//	A = A * 0.5F;
	//	B = B * 0.5F;

	//	// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

	//	B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
	//	B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
	//	Dist = fabsf(B1 - B2);

	//	// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
	//	if (Dist > A + B)
	//		return false;
	//}

	//// 2. ���� �������� �˻� 
	//for (int i = 0; i < 3; ++i)
	//{
	//	// ������ �Ǵ� �� ����
	//	D3DXVECTOR3 Axis_norm;
	//	D3DXVec3Normalize(&Axis_norm, &Axis_c2[i]);

	//	// 2 - 1. �࿡���� ���� A�� B ����
	//	map<float, UINT> mValue;
	//	UINT minIndex, maxIndex;

	//	//���� ª�������� ������ ã�Ƴ���.
	//	for (int v = 0; v < sizeof(LeftBox.vWorldVertex) / sizeof(_float4); ++v)
	//	{
	//		mValue.insert(make_pair((D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[v])), v));
	//	}


	//	// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ
	//	map<float, UINT>::iterator iter;
	//	iter = mValue.begin();

	//	for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
	//	--iter;

	//	minIndex = mValue.begin()->second;
	//	maxIndex = iter->second;

	//	// 1, 3, 4 ������ ���� ŭ
	//	FLOAT A, B, Dist;
	//	D3DXVECTOR3 vec;
	//	switch (i)
	//	{
	//	case 0:
	//		vec = RightBox.vWorldVertex[1] - RightBox.vWorldVertex[0];
	//		break;

	//	case 1:
	//		vec = RightBox.vWorldVertex[3] - RightBox.vWorldVertex[0];
	//		break;

	//	case 2:
	//		vec = RightBox.vWorldVertex[4] - RightBox.vWorldVertex[0];
	//		break;
	//	}

	//	A = fabsf(D3DXVec3Length(&vec));

	//	FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[maxIndex]));
	//	FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[minIndex]));

	//	B = fabs(B1 - B2);

	//	A = A * 0.5F;
	//	B = B * 0.5F;

	//	// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

	//	B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
	//	B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
	//	Dist = fabsf(B1 - B2);

	//	// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
	//	if (Dist > A + B)
	//		return false;
	//}

	return true;
}

bool CCollision_Manager::Is_SphereCollision(CCollider* _pLeft, CCollider* _pRight)
{
	COL_INFO_SPHERE LeftInfo = ((CCollider_Sphere*)_pLeft)->Get_ColInfo();
	COL_INFO_SPHERE RightInfo = ((CCollider_Sphere*)_pRight)->Get_ColInfo();

	_float4 CenterDiff = RightInfo.vFinalPos - LeftInfo.vFinalPos;
	_float Dist = fabsf(CenterDiff.Length());

	_float LeftRadius = LeftInfo.fRadius;
	_float RightRadius = RightInfo.fRadius;

	if (Dist > LeftRadius + RightRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}
