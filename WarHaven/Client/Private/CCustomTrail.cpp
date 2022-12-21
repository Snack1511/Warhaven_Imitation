#include "stdafx.h"
#include "CCustomTrail.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Texture.h"
#include "Transform.h"

#include "Renderer.h"
#include "CShader.h"


CCustomTrail::CCustomTrail(_uint iGroupIdx)
	: CMesh(iGroupIdx)
{
}

CCustomTrail::~CCustomTrail()
{
}

CCustomTrail* CCustomTrail::Create(_uint iGroupIdx, _uint iTriCnt)
{
	CCustomTrail* pInstance = new CCustomTrail(iGroupIdx);

	pInstance->m_iTriCnt = iTriCnt;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCustomTrail");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize : CCustomTrail");
		return nullptr;
	}

	return pInstance;
}

_uint3 CCustomTrail::Get_Indices(_uint iIndex) const
{
	return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
		((FACEINDICES16*)m_pIndices)[iIndex]._2,
		((FACEINDICES16*)m_pIndices)[iIndex]._3);
}

HRESULT CCustomTrail::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = m_iTriCnt + 2;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = m_iTriCnt;
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

	for (_uint i = 0; i < m_iTriCnt; i += 2)
	{
		pIndices[i]._1 = i + 3;
		pIndices[i]._2 = i + 1;
		pIndices[i]._3 = i + 0;

		pIndices[i + 1]._1 = i + 2;
		pIndices[i + 1]._2 = i + 3;
		pIndices[i + 1]._3 = i + 0;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


	return S_OK;
}

HRESULT CCustomTrail::Initialize()
{

	return S_OK;
}

void CCustomTrail::Start()
{
	CComponent::Start();


}

void CCustomTrail::OnEnable()
{
	CComponent::OnEnable();
}

void CCustomTrail::OnDisable()
{
	CComponent::OnDisable();
}

void CCustomTrail::Tick()
{

}

void CCustomTrail::Late_Tick()
{
}

void CCustomTrail::ReMap_TrailBuffers(list<_float4>& NodesList, _float fWide)
{
	if ((_uint)NodesList.size() < 2)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_float4 vPrevNode = ZERO_VECTOR;
	_uint iVertexIndex = 0;

	for (auto& elem : NodesList)
	{
		/* 트레일 두 점의 방향 */
		_float4 vDir;

		if (elem == NodesList.front())
			vDir = _float4(0.f, 0.f, 1.f, 0.f);
		else
			vDir = (elem - vPrevNode);

		vDir.y = 0.f;
		vDir.Normalize();

		vPrevNode = elem;

		_float4 vRight = _float4(0.f, 1.f, 0.f, 0.f).Cross(vDir);
		vRight.Normalize();

		
		/* 노드마다 버텍스 두개 찍기 */
		_float4 vVertex1 = elem + vRight * fWide;
		_float4 vVertex2 = elem - vRight * fWide;

		memcpy(&((VTXTEX*)SubResource.pData)[iVertexIndex++].vPosition, &vVertex1, sizeof(_float3));
		memcpy(&((VTXTEX*)SubResource.pData)[iVertexIndex++].vPosition, &vVertex2, sizeof(_float3));

		/* 버텍스 갯수 넘어가면 끝 */
		if (iVertexIndex >= m_iNumVertices)
			break;
	}

	for (_uint i = 0; i < iVertexIndex; i += 2)
	{
		((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)iVertexIndex - 2), 1.f };
		((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)iVertexIndex - 2), 0.f };
	}

	/* 남은 정점들은 전부 끝 점으로 맞추기 */
	
	for (_uint i = iVertexIndex; i < m_iNumVertices; i += 2)
	{
		((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iVertexIndex-2].vPosition;
		((VTXTEX*)SubResource.pData)[i+1].vPosition = ((VTXTEX*)SubResource.pData)[iVertexIndex-1].vPosition;
	}

	DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

}
