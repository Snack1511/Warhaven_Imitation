#include "CMesh_Terrain.h"

#include "GameInstance.h"

CMesh_Terrain::CMesh_Terrain(_uint iGroupIdx)
	: CMesh(iGroupIdx)
{
}

CMesh_Terrain::~CMesh_Terrain()
{
	SAFE_DELETE_ARRAY(m_pVerticesColor);
	SAFE_DELETE_ARRAY(m_pPhysXVerticesPos);
}

CMesh_Terrain* CMesh_Terrain::Create(_uint iGroupIdx, const _tchar* pHeightMapFilePath)
{
	CMesh_Terrain* pInstance = new CMesh_Terrain(iGroupIdx);

	if (FAILED(pInstance->SetUp_HeightMap(pHeightMapFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_HeightMap : CMesh_Terrain");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CMesh_Terrain");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CMesh_Terrain* CMesh_Terrain::Create(_uint iGroupIdx, _uint iNumVerticesX, _uint iNumVerticesY)
{
	CMesh_Terrain* pInstance = new CMesh_Terrain(iGroupIdx);

	if (FAILED(pInstance->SetUp_Terrain(iNumVerticesX, iNumVerticesY)))
	{
		Call_MsgBox(L"Failed to SetUp_Terrain : CMesh_Terrain");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CMesh_Terrain");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CMesh_Terrain* CMesh_Terrain::Create(_uint iGroupIdx, _uint iNumTilesX, _uint iNumTilesZ, _float fSize)
{
	CMesh_Terrain* pInstance = new CMesh_Terrain(iGroupIdx);

	if (FAILED(pInstance->SetUp_Terrain(iNumTilesX, iNumTilesZ, fSize)))
	{
		Call_MsgBox(L"Failed to SetUp_Terrain : CMesh_Terrain");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CMesh_Terrain");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CMesh_Terrain::Map_Vertex(_uint iIndex, _float4 vPosition, _float4 vColor)
{
	if (iIndex >= m_iNumVertices)
		return;

	m_pVerticesPos[iIndex] = _float3(vPosition.XMLoad().m128_f32);

	D3D11_MAPPED_SUBRESOURCE		SubResource;


	DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	((VTXNORTEX*)SubResource.pData)[iIndex].vPosition = m_pVerticesPos[iIndex];
	((VTXNORTEX*)SubResource.pData)[iIndex].vColor = vColor;

	DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
}

void CMesh_Terrain::ReMap_Vertices()
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		((VTXNORTEX*)SubResource.pData)[i].vPosition = m_pVerticesPos[i];
		((VTXNORTEX*)SubResource.pData)[i].vColor = m_pVerticesColor[i];
	}

	DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);


}

void CMesh_Terrain::Update_VertsNormal()
{
	_float3* ArrNorm = new _float3[m_iNumPrimitive];
	ZeroMemory(ArrNorm, sizeof(_float3) * m_iNumPrimitive);

	//_ulong		dwNumFaces = 0;
	for (_uint i = 0; i < m_iNumPrimitive; i += 2)
	{
		_vector		vSour, vDest, vNormal;
		_uint3* pIndices = &((_uint3*)m_pIndices)[i];

		//한 면당 정점끼리의 방향 계산 및 이를 토대로 면의 법선벡터 계산
		vSour = XMLoadFloat3(&m_pVerticesPos[(*pIndices)._2]) - XMLoadFloat3(&m_pVerticesPos[(*pIndices)._1]);
		vDest = XMLoadFloat3(&m_pVerticesPos[(*pIndices)._3]) - XMLoadFloat3(&m_pVerticesPos[(*pIndices)._2]);
		vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

		//기존 정점의 노멀에다가 덧샘
		XMStoreFloat3(&ArrNorm[(*pIndices)._1],
			XMLoadFloat3(&ArrNorm[(*pIndices)._1]) + vNormal);
		XMStoreFloat3(&ArrNorm[(*pIndices)._2],
			XMLoadFloat3(&ArrNorm[(*pIndices)._2]) + vNormal);
		XMStoreFloat3(&ArrNorm[(*pIndices)._3],
			XMLoadFloat3(&ArrNorm[(*pIndices)._3]) + vNormal);

		vSour = XMLoadFloat3(&m_pVerticesPos[(*(pIndices + 1))._2]) - XMLoadFloat3(&m_pVerticesPos[(*(pIndices + 1))._1]);
		vDest = XMLoadFloat3(&m_pVerticesPos[(*(pIndices + 1))._3]) - XMLoadFloat3(&m_pVerticesPos[(*(pIndices + 1))._2]);
		vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

		XMStoreFloat3(&ArrNorm[(*(pIndices + 1))._1],
			XMLoadFloat3(&ArrNorm[(*(pIndices + 1))._1]) + vNormal);
		XMStoreFloat3(&ArrNorm[(*(pIndices + 1))._2],
			XMLoadFloat3(&ArrNorm[(*(pIndices + 1))._2]) + vNormal);
		XMStoreFloat3(&ArrNorm[(*(pIndices + 1))._3],
			XMLoadFloat3(&ArrNorm[(*(pIndices + 1))._3]) + vNormal);
	}

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
		XMStoreFloat3(&ArrNorm[i], XMVector3Normalize(XMLoadFloat3(&ArrNorm[i])));


	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		((VTXNORTEX*)SubResource.pData)[i].vNormal = ArrNorm[i];
	}

	DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

	Safe_Delete_Array(ArrNorm);
}

void CMesh_Terrain::Memory_Save()
{
#ifdef MEMORY_SAVE
	SAFE_DELETE(m_pVerticesPos);
	SAFE_DELETE(m_pVerticesColor);
	SAFE_DELETE(m_pIndices);
#endif
}

void CMesh_Terrain::ReadyforPhysX()
{
	//VerticesPos 를 다시 세팅해주야함
	m_iPhysXNumVertices = m_iNumPrimitive * 3;

	m_pPhysXVerticesPos = new _float3[m_iPhysXNumVertices];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint3 Indices = Get_Indices(i);

		m_pPhysXVerticesPos[i*3] = m_pVerticesPos[Indices._1];
		m_pPhysXVerticesPos[i * 3 +1] = m_pVerticesPos[Indices._2];
		m_pPhysXVerticesPos[i * 3 +2] = m_pVerticesPos[Indices._3];
	}

	
}

void CMesh_Terrain::ClearPhysX()
{
	SAFE_DELETE_ARRAY(m_pPhysXVerticesPos);
}

HRESULT CMesh_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMesh_Terrain::Initialize()
{
	return S_OK;
}

void CMesh_Terrain::Start()
{
	

}

HRESULT CMesh_Terrain::SetUp_HeightMap(const _tchar* pHeightMapFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	BITMAPINFOHEADER			ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	_ulong* pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXNORTEX);


	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;



	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, (pPixel[iIndex] & 0x000000ff) / 10.f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}




	Safe_Delete_Array(pPixel);
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[1];
			pIndices[dwNumFaces]._3 = iIndices[2];

			_vector		vSour, vDest, vNormal;

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[2];
			pIndices[dwNumFaces]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh_Terrain::SetUp_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXNORTEX);

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;


	m_pVerticesPos = new _float3[m_iNumVertices];
	m_pVerticesColor = new _float4[m_iNumVertices];
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
			pVertices[iIndex].vColor = m_pVerticesColor[iIndex] = _float4(1.f, 0.f, 0.f, 1.f);
		}
	}

#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[1];
			pIndices[dwNumFaces]._3 = iIndices[2];

			_vector		vSour, vDest, vNormal;

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;

			pIndices[dwNumFaces]._1 = iIndices[0];
			pIndices[dwNumFaces]._2 = iIndices[2];
			pIndices[dwNumFaces]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFaces]._3].vNormal) + vNormal);
			++dwNumFaces;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;
	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);
	return S_OK;
}


HRESULT CMesh_Terrain::SetUp_Terrain(_uint iNumTilesX, _uint iNumTilesZ, _float fSize)
{
	m_iNumVerticesX = iNumTilesX * 2;
	m_iNumVerticesZ = iNumTilesZ * 2;


#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXNORTEX);

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iNumVertexBuffers = 1;


	m_pVerticesPos = new _float3[m_iNumVertices];
	m_pVerticesColor = new _float4[m_iNumVertices];
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < iNumTilesZ; ++i)
	{
		for (_uint j = 0; j < iNumTilesX; ++j)
		{
			_float3 vCenterPos = _float3((_float)j * fSize, 0.f, (_float)i * fSize);
			_uint	iIndexLT = (i * 2 * iNumTilesX) + (j * 2) + iNumTilesX;
			_uint	iIndexLB = (i * 2 * iNumTilesX) + (j * 2);
			_uint	iIndexRT = iIndexLT + 1;
			_uint	iIndexRB = iIndexLB + 1;

			_float3 vLTPadding = _float3(-fSize * 0.5f, 0, fSize * 0.5f);
			_float3 vLBPadding = _float3(-fSize * 0.5f, 0, -fSize * 0.5f);

			_float3 vRTPadding = _float3(fSize * 0.5f, 0, fSize * 0.5f);
			_float3 vRBPadding = _float3(fSize * 0.5f, 0, -fSize * 0.5f);

			//인덱스마다 4개씩 찍어줘야함
			
			_vector xCenter = XMLoadFloat3(&vCenterPos);
			
			_vector xLTPadding = XMLoadFloat3(&vLTPadding);
			_vector xLBPadding = XMLoadFloat3(&vLBPadding);

			_vector xRTPadding = XMLoadFloat3(&vRTPadding);
			_vector xRBPadding = XMLoadFloat3(&vRBPadding);

			_vector xLTVertex = xCenter + xLTPadding;
			_vector xLBVertex = xCenter + xLBPadding;
			_vector xRTVertex = xCenter + xRTPadding;
			_vector xRBVertex = xCenter + xRBPadding;

			XMStoreFloat3(&pVertices[iIndexLT].vPosition, xLTVertex);
			XMStoreFloat3(&pVertices[iIndexLB].vPosition, xLBVertex);
			XMStoreFloat3(&pVertices[iIndexRT].vPosition, xRTVertex);
			XMStoreFloat3(&pVertices[iIndexRB].vPosition, xRBVertex);

			m_pVerticesPos[iIndexLT] = pVertices[iIndexLT].vPosition;
			m_pVerticesPos[iIndexLB] = pVertices[iIndexLB].vPosition;
			m_pVerticesPos[iIndexRT] = pVertices[iIndexRT].vPosition;
			m_pVerticesPos[iIndexRB] = pVertices[iIndexRB].vPosition;


			//pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)j * fSize, 0.f, (_float)i * fSize);
			//pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
			//pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
			_float4 InitColor = _float4(0.f, 1.f, 0.f, 1.f);
			pVertices[iIndexLT].vColor = InitColor;
			pVertices[iIndexLB].vColor = InitColor;
			pVertices[iIndexRT].vColor = InitColor;
			pVertices[iIndexRB].vColor = InitColor;

			m_pVerticesColor[iIndexLT] = pVertices[iIndexLT].vColor;
			m_pVerticesColor[iIndexLB] = pVertices[iIndexLB].vColor;
			m_pVerticesColor[iIndexRT] = pVertices[iIndexRT].vColor;
			m_pVerticesColor[iIndexRB] = pVertices[iIndexRB].vColor;

		}
	}

#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = iNumTilesX * iNumTilesZ * 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;

	for (_uint i = 0; i < iNumTilesZ; ++i)
	{
		for (_uint j = 0; j < iNumTilesX; ++j)
		{
			_uint	iIndex = (i * iNumTilesX * 2) + j * 2;

			_uint	iIndexLT = iIndex + iNumTilesX;
			_uint	iIndexLB = iIndex;
			_uint	iIndexRT = iIndexLT + 1;
			_uint	iIndexRB = iIndexLB + 1;

			pIndices[dwNumFaces]._1 = iIndexLB;
			pIndices[dwNumFaces]._2 = iIndexLT;
			pIndices[dwNumFaces]._3 = iIndexRT;


			++dwNumFaces;

			pIndices[dwNumFaces]._1 = iIndexLB;
			pIndices[dwNumFaces]._2 = iIndexRT;
			pIndices[dwNumFaces]._3 = iIndexRB;


			++dwNumFaces;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;
	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);
	return S_OK;
}