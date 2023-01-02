#include "CCell.h"
#include "CUtility_Cell.h"
#include "GameInstance.h"
CCell::CCell()
{
}

CCell::~CCell()
{
#ifdef _DEBUG
	_ulong dwCnt = m_pIB.Reset();
	dwCnt = m_pVB.Reset();
#endif // _DEBUG

}

CCell* CCell::Create(const _float3* pPoints, _int iIndex)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pPoints, iIndex, 999999.f)))
	{
		Call_MsgBox(L"Failed to Initialize : CCell");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CCell* CCell::Create(const _float3* pPoints, _int iIndex, _float fMinHeight)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pPoints, iIndex, fMinHeight)))
	{
		Call_MsgBox(L"Failed to Initialize : CCell");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

_float4 CCell::Get_Point_Float4(POINT ePoint)
{
	_float4 vPos;
	vPos = XMLoadFloat3(&m_vPoints[ePoint]);
	vPos.w = 1.f;

	return vPos;
}

CCell::LinePair CCell::Get_LinePoints(LINE eLine)
{
	_float4 vStart;
	_float4 vEnd;

	switch (eLine)
	{
	case CCell::LINE_AB:
		vStart = XMLoadFloat3(m_vPoints + POINT_A);
		vEnd = XMLoadFloat3(m_vPoints + POINT_B);
		break;
	case CCell::LINE_BC:
		vStart = XMLoadFloat3(m_vPoints + POINT_B);
		vEnd = XMLoadFloat3(m_vPoints + POINT_C);
		break;
	case CCell::LINE_CA:
		vStart = XMLoadFloat3(m_vPoints + POINT_C);
		vEnd = XMLoadFloat3(m_vPoints + POINT_A);
		break;
	}
	vStart.w = 1.f;
	vEnd.w = 1.f;

	return make_pair(vStart, vEnd);
}

_bool CCell::Check_Attribute(_uint iAttribute)
{
	if ((m_bCellFlag & iAttribute) == iAttribute)
		return true;

	return false;
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex, _float fMinHeight)
{
	m_iMyIndex = iIndex;
	m_vCenter = _float4(0.f, 0.f, 0.f, 0.f);

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	for (_uint i = 0; i < POINT_END; ++i)
	{
		m_vCenter.x += m_vPoints[i].x;
		m_vCenter.y += m_vPoints[i].y;
		m_vCenter.z += m_vPoints[i].z;
	}
	m_vCenter.x /= POINT_END;
	m_vCenter.y /= POINT_END;
	m_vCenter.z /= POINT_END;
	m_vCenter.w = 1.f;

	m_fLayerKey = fMinHeight;

#ifdef _DEBUG
	if (FAILED(SetUp_Vertices()))
	{
		assert(0);
	}
#endif // _DEBUG


	return S_OK;

}

void CCell::Make_Matrix()
{
	_float4 vQuaternion;

	//¹ý¼±º¤ÅÍ
	_vector V1, V2;
	V1 = (XMLoadFloat3(&m_vPoints[CCell::POINT_A]) - XMLoadFloat3(&m_vPoints[CCell::POINT_B]));
	V2 = (XMLoadFloat3(&m_vPoints[CCell::POINT_C]) - XMLoadFloat3(&m_vPoints[CCell::POINT_B]));

	m_vCellNormal = XMVector3Cross(V1, V2);
	m_vCellNormal *= -1.f;
	m_vCellNormal.Normalize();


	if (m_iMyIndex % 2 == 1)
	{
		//È¦¼ö°¡ ¾Æ·¡»ï°¢Çü
		m_vCellRight = XMLoadFloat3(&m_vLine[CCell::LINE_BC]);
		m_vCellRight *= -1.f;
		m_vCellRight.Normalize();

	}
	else
	{
		m_vCellRight = XMLoadFloat3(&m_vLine[CCell::LINE_AB]);
		m_vCellRight.Normalize();
	}

	_float4 vDefaultUp = _float4(0.f, 1.f, 0.f, 0.f);
	m_fAngle = m_vCellNormal.Dot(vDefaultUp);
	m_fAngle = XMConvertToDegrees(acosf(m_fAngle));
}

_bool CCell::Compare_Points(_vector vSourPoint, _vector vDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_vector vPosition, CCell** pOutNeighborCell, LINE* pOutLine)
{
	/* ÀÌ ½ìÀÇ ¼¼º¯¿¡ ´ëÇØ¼­ ³ª°¬´ÂÁö ¾È³«¤¶´Ù¤Ó;¤Ã ÆÇ´ÜÇÑ´Ù. */
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		/* ¹Ù±ùÀ¸·Î ³ª°¬À¸¸é °»½Å */
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			*pOutNeighborCell = m_pNeighborCells[i];
			*pOutLine = (LINE)i;

			return false;
		}
	}


	return true;
}
_bool CCell::Check_CrossLines(_float4 LineStart, _float4 LineEnd)
{
	_float2 PA = _float2(m_vPoints[POINT_A].x, m_vPoints[POINT_A].z);
	_float2 PB = _float2(m_vPoints[POINT_B].x, m_vPoints[POINT_B].z);
	_float2 PC = _float2(m_vPoints[POINT_C].x, m_vPoints[POINT_C].z);
	_float2 v2LineStart = _float2(LineStart.x, LineStart.z);
	_float2 v2LineEnd = _float2(LineEnd.x, LineEnd.z);
	if (CUtility_Cell::Is_Cross(PA, PB, v2LineStart, v2LineEnd))
		return true;

	if (CUtility_Cell::Is_Cross(PB, PC, v2LineStart, v2LineEnd))
		return true;

	if (CUtility_Cell::Is_Cross(PC, PA, v2LineStart, v2LineEnd))
		return true;

	return false;
}
#ifdef _DEBUG
void CCell::DebugRendering()
{
	

	ID3D11Buffer* pVertexBuffers[] = {
	m_pVB.Get(),
	};

	_uint		iStrides[] = {
		m_iStride,
	};

	_uint		iOffsets[] = {
		0,
	};

	DEVICE_CONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICE_CONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICE_CONTEXT->IASetPrimitiveTopology(m_eToplogy);

	DEVICE_CONTEXT->DrawIndexed(m_iNumIndices, 0, 0);
}
HRESULT CCell::Create_VertexBuffer()
{
	return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVB.GetAddressOf()));
}
HRESULT CCell::Create_IndexBuffer()
{
	return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pIB.GetAddressOf()));
}
HRESULT CCell::SetUp_Vertices()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXDEFAULT);
	m_iNumVertices = 3;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXDEFAULT* pVertices = new VTXDEFAULT[m_iNumVertices];

	//pVertices[0].vPosition 
	//	= _float3(
	//		m_vPoints[0].x - m_vCenter.x,
	//		m_vPoints[0].y - m_vCenter.y, 
	//		m_vPoints[0].z - m_vCenter.z);

	//pVertices[1].vPosition 
	//	= _float3(
	//		m_vPoints[1].x - m_vCenter.x, 
	//		m_vPoints[1].y - m_vCenter.y,
	//		m_vPoints[1].z - m_vCenter.z);

	//pVertices[2].vPosition 
	//	= _float3(
	//		m_vPoints[2].x - m_vCenter.x,
	//		m_vPoints[2].y - m_vCenter.y, 
	//		m_vPoints[2].z - m_vCenter.z);
	pVertices[0].vPosition
		= _float3(
			m_vPoints[0].x,
			m_vPoints[0].y,
			m_vPoints[0].z);

	pVertices[1].vPosition
		= _float3(
			m_vPoints[1].x,
			m_vPoints[1].y,
			m_vPoints[1].z);

	pVertices[2].vPosition
		= _float3(
			m_vPoints[2].x,
			m_vPoints[2].y,
			m_vPoints[2].z);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 1;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	return S_OK;
}
#endif // _DEBUG


