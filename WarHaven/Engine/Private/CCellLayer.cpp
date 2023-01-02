#include "CCellLayer.h"
#include "CCell.h"
#include "CUtility_Cell.h"
#include "CNode.h"
#ifdef _DEBUG
	#include "GameInstance.h"
	#include "Render_Manager.h"
	#include "CShader.h"
#endif
CCellLayer::CCellLayer()
{
}

CCellLayer::~CCellLayer()
{
	for (auto& Cell : m_Cells)
		SAFE_DELETE(Cell);
	for (auto& Node : m_Nodes)
		SAFE_DELETE(Node);

#ifdef _DEBUG
	_ulong dwCnt = m_pIB.Reset();
	dwCnt = m_pVB.Reset();
	Safe_Release(m_pVBInstance);
#endif // _DEBUG

}

CCellLayer* CCellLayer::Create(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _float fHeightMin, _float fHeightMax, _int BaseCellAttribute)
{
	CCellLayer* pInstance = new CCellLayer();
	pInstance->m_fLayerHeightMin = fHeightMin;
	pInstance->m_fLayerHeightMax = fHeightMax;

	if (FAILED(pInstance->SetUp_Cells(XNums, ZNums, fTileSize, vCenterPosition, BaseCellAttribute)))
	{
		Call_MsgBox(L"Failed to SetUp_Cells : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#ifdef _DEBUG
	if (FAILED(pInstance->SetUp_Vertex()))
	{
		Call_MsgBox(L"Failed to SetUp_Vertex : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Instancing()))
	{
		Call_MsgBox(L"Failed to SetUp_Instancing : CCellLayer");
		SAFE_DELETE(pInstance);
	}	
	if (FAILED(pInstance->SetUp_Shader()))
	{
		Call_MsgBox(L"Failed to SetUp_Shader : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#endif
	//if (FAILED(pInstance->SetUp_Neighbor()))
	//{
	//	Call_MsgBox(L"Failed to SetUp_Neighbor : CCellLayer");
	//	SAFE_DELETE(pInstance);
	//}
	//if (FAILED(pInstance->SetUp_Visibility()))
	//{
	//	Call_MsgBox(L"Failed to SetUp_Visibility : CCellLayer");
	//	SAFE_DELETE(pInstance);
	//}
	return pInstance;
}

CCellLayer* CCellLayer::Create(wstring strFilePath)
{
	CCellLayer* pInstance = new CCellLayer();
	if (FAILED(pInstance->SetUp_Cells(strFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_Cells : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Nodes()))
	{
		Call_MsgBox(L"Failed to SetUp_Nodes : CCellLayer");
		SAFE_DELETE(pInstance);
	}	
	if (FAILED(pInstance->SetUp_Visibility()))
	{
		Call_MsgBox(L"Failed to SetUp_Visibility : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#ifdef _DEBUG
	if (FAILED(pInstance->SetUp_Vertex()))
	{
		Call_MsgBox(L"Failed to SetUp_Vertex : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Instancing()))
	{
		Call_MsgBox(L"Failed to SetUp_Instancing : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Index()))
	{
		Call_MsgBox(L"Failed to SetUp_Instancing : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Shader()))
	{
		Call_MsgBox(L"Failed to SetUp_Shader : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#endif
	return pInstance;
}

void CCellLayer::Save(wstring strPath)
{
	ofstream writeFile;
	writeFile.open(strPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save : CCellLayer");
		writeFile.close();
		return;
	}
	string Name;
	Name.assign(m_strDebugName.begin(), m_strDebugName.end());
	_uint iNameSize = Name.length() + 1;
	char szName[MAXCHAR] = "";
	strcpy_s(szName, sizeof(char) * iNameSize, Name.c_str());
	strcat_s(szName, "\0");

	writeFile.write((char*)&iNameSize, sizeof(_uint));
	writeFile.write(szName, sizeof(char) * iNameSize);

	writeFile.write((char*)&m_vCenterPosition, sizeof(_float4));
	writeFile.write((char*)&m_iXNums, sizeof(_uint));
	writeFile.write((char*)&m_iZNums, sizeof(_uint));
	writeFile.write((char*)&m_fTileSize, sizeof(_float));
	writeFile.write((char*)&m_fLayerHeightMin, sizeof(_float));
	writeFile.write((char*)&m_fLayerHeightMax, sizeof(_float));

	_uint CellSize = _uint(m_Cells.size());

	_byte* Attributes = new _byte[CellSize];
	_float* NeighborKeys = new _float[CellSize * 3];
	_int* NeighborIndices = new _int[CellSize * 3];
	_uint Index = 0;
	for (auto& Cell : m_Cells) 
	{
		Attributes[Index] = Cell->Get_Attribute();

		CCell* Neighbor[3] =
		{
			 Cell->Get_NeighborCell(CCell::LINE_AB),
			 Cell->Get_NeighborCell(CCell::LINE_BC),
			 Cell->Get_NeighborCell(CCell::LINE_CA)
		};
		_uint NeighborIndex = Index * 3;
		NeighborKeys[NeighborIndex] = (nullptr == Neighbor[0]) ? 999999.f : Neighbor[0]->Get_LayerKey();
		NeighborKeys[NeighborIndex + 1] = (nullptr == Neighbor[1]) ? 999999.f : Neighbor[1]->Get_LayerKey();
		NeighborKeys[NeighborIndex + 2] = (nullptr == Neighbor[2]) ? 999999.f : Neighbor[2]->Get_LayerKey();

		NeighborIndices[NeighborIndex] = (nullptr == Neighbor[0]) ? -1 : Neighbor[0]->Get_Index();
		NeighborIndices[NeighborIndex + 1] = (nullptr == Neighbor[1]) ? -1 : Neighbor[1]->Get_Index();
		NeighborIndices[NeighborIndex + 2] = (nullptr == Neighbor[2]) ? -1 : Neighbor[2]->Get_Index();
		Index++;
		//�̿� ���� �Ҽ� �����̾�Ű����..
		//�̿� �ε�����..
	}

	writeFile.write((char*)Attributes, sizeof(_byte) * CellSize);
	writeFile.write((char*)NeighborKeys, sizeof(_float) * CellSize * 3);
	writeFile.write((char*)NeighborIndices, sizeof(_int) * CellSize * 3);

	writeFile.close();
	SAFE_DELETE_ARRAY(Attributes);
	SAFE_DELETE_ARRAY(NeighborKeys);
	SAFE_DELETE_ARRAY(NeighborIndices);
}

//���� ���� 0, 0 �� ���ϴ�
HRESULT CCellLayer::SetUp_Cells(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _int iAttribute)
{
	m_vCenterPosition = vCenterPosition;
	m_iXNums = XNums;
	m_iZNums = ZNums;
	m_fTileSize = fTileSize;

	CCell* pLeftTri = nullptr;
	CCell* pRightTri = nullptr;

	_float4 vHalfCenter = _float4(vCenterPosition.x * 0.5f, vCenterPosition.y * 0.5f, vCenterPosition.z * 0.5f, 1.f);
	//�� ����
	for (_uint i = 0; i < m_iZNums; ++i)
	{
		for (_uint j = 0; j < m_iXNums; ++j)
		{
			_uint iIndex = ((i * m_iXNums) + j) * 2;

			_float3 vVertexPos[4] =
			{
				_float3(_float(j) * fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i) * fTileSize - vHalfCenter.z),
				_float3(_float(j) * fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * fTileSize - vHalfCenter.z),
				_float3(_float(j + 1) * fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * fTileSize - vHalfCenter.z),
				_float3(_float(j + 1) * fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i) * fTileSize - vHalfCenter.z)
			};

			_float3 LeftPositions[3] =
			{
				vVertexPos[0],
				vVertexPos[1],
				vVertexPos[2],
			};
			_float3 RightPositions[3] =
			{
				vVertexPos[0],
				vVertexPos[2],
				vVertexPos[3],
			};

			pLeftTri = CCell::Create(LeftPositions, iIndex, m_fLayerHeightMin);
			pRightTri = CCell::Create(RightPositions, iIndex + 1, m_fLayerHeightMin);

			pLeftTri->Set_Flags(iAttribute);
			pRightTri->Set_Flags(iAttribute);

			m_Cells.push_back(pLeftTri);
			m_Cells.push_back(pRightTri);
			
		}
	}

	_uint Index = 0;
	for (auto& Cell: m_Cells)
	{
		CCell* pCell[3] = {};
		if(Index % 2 == 0)
		{ 
			pCell[0] = ((Index) % (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - 1];//���� ��
			pCell[1] = ((Index / (m_iXNums * 2)) == m_iZNums - 1) ? nullptr : m_Cells[Index + (m_iXNums*2)];//�� ��
			pCell[2] = m_Cells[Index + 1];
		}
		else
		{
			pCell[0] = m_Cells[Index - 1];//���� ��
			pCell[1] = ((Index) % (m_iXNums * 2) == (m_iXNums * 2) - 1) ? nullptr : m_Cells[Index + 1];//������ ��
			pCell[2] = (Index / (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - (m_iXNums*2)];//�� �Ʒ�
		}
		Cell->Set_Neighbor(CCell::LINE_AB, pCell[0]);
		Cell->Set_Neighbor(CCell::LINE_BC, pCell[1]);
		Cell->Set_Neighbor(CCell::LINE_CA, pCell[2]);
		Index++;
	}
	//�� ���� ����
	//�� ���� ���� Cell����Ʈ ����
	//Blocked���̸� �� ������ ���� ���� �� m_Nodes�� ����

	//for (auto& Cell : m_Cells)
	//{
	//	Add_CellList(CELL_BLOCKED, Cell);
	//	Add_CellList(CELL_STAIR, Cell);
	//	Add_CellList(CELL_GROUND, Cell);
	//}

	return S_OK;
}

HRESULT CCellLayer::SetUp_Cells(wstring strFilePath)
{
	ifstream readFile;
	string strFile;
	strFile.assign(strFilePath.begin(), strFilePath.end());
	readFile.open(strFile);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to Load");
		readFile.close();
		return E_FAIL;
	}

	_uint iNameSize = 0;
	readFile.read((char*)&iNameSize, sizeof(_uint));
	char szName[MAXCHAR] = "";
	readFile.read(szName, sizeof(char)* iNameSize);
	string strName = szName;
	m_strDebugName.assign(strName.begin(), strName.end());

	readFile.read((char*)&m_vCenterPosition, sizeof(_float4));
	readFile.read((char*)&m_iXNums, sizeof(_uint));
	readFile.read((char*)&m_iZNums, sizeof(_uint));
	readFile.read((char*)&m_fTileSize, sizeof(_float));
	readFile.read((char*)&m_fLayerHeightMin, sizeof(_float));
	readFile.read((char*)&m_fLayerHeightMax, sizeof(_float));

	_uint TotalNums = m_iZNums * m_iXNums * 2;
	_byte* ArrAttribute = new _byte[TotalNums];
	m_NeighborLayerKeys = new _float[TotalNums];
	m_NeighborIndex = new _int[TotalNums];


	readFile.read((char*)ArrAttribute, sizeof(_byte) * TotalNums);
	readFile.read((char*)m_NeighborLayerKeys, sizeof(_float) * TotalNums * 3);
	readFile.read((char*)m_NeighborIndex, sizeof(_int) * TotalNums * 3);
	readFile.close();
	
	CCell* pLeftTri = nullptr;
	CCell* pRightTri = nullptr;

	_float4 vHalfCenter = _float4(m_vCenterPosition.x * 0.5f, m_vCenterPosition.y * 0.5f, m_vCenterPosition.z * 0.5f, 1.f);
	//�� ����
	for (_uint i = 0; i < m_iZNums; ++i)
	{
		for (_uint j = 0; j < m_iXNums; ++j)
		{
			_uint iIndex = ((i * m_iXNums) + j) * 2;

			_float3 vVertexPos[4] =
			{
				_float3(_float(j)	  * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i)	  * m_fTileSize - vHalfCenter.z),//LB
				_float3(_float(j)	  * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * m_fTileSize - vHalfCenter.z),//LT
				_float3(_float(j + 1) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * m_fTileSize - vHalfCenter.z),//RT
				_float3(_float(j + 1) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i)	  * m_fTileSize - vHalfCenter.z)//RB
			};

			_float3 LeftPositions[3] =
			{
				vVertexPos[0],
				vVertexPos[1],
				vVertexPos[2],
			};
			_float3 RightPositions[3] =
			{
				vVertexPos[0],
				vVertexPos[2],
				vVertexPos[3],
			};

			pLeftTri = CCell::Create(LeftPositions, iIndex, m_fLayerHeightMin);
			pRightTri = CCell::Create(RightPositions, iIndex + 1, m_fLayerHeightMin);

			pLeftTri->Set_Flags(ArrAttribute[iIndex]);
			pRightTri->Set_Flags(ArrAttribute[iIndex + 1]);

			m_Cells.push_back(pLeftTri);
			m_Cells.push_back(pRightTri);


			Add_CellList(CELL_BLOCKED, pLeftTri);
			Add_CellList(CELL_STAIR, pLeftTri);
			Add_CellList(CELL_GROUND, pLeftTri);

			Add_CellList(CELL_BLOCKED, pRightTri);
			Add_CellList(CELL_STAIR, pRightTri);
			Add_CellList(CELL_GROUND, pRightTri);

			
		}
	}
	if (nullptr == m_NeighborLayerKeys)
	{
		//m_NeighborLayerKeys���� �ȵ�
		assert(0);
	}
	if (nullptr == m_NeighborIndex)
	{
		//m_NeighborIndex���� �ȵ�
		assert(0);
	}
	SAFE_DELETE_ARRAY(ArrAttribute);

	return S_OK;
}

HRESULT CCellLayer::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_A),
				pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_B),
				pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_C),
				pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
				continue;
			}
		}
	}


	return S_OK;
}

HRESULT CCellLayer::SetUp_Neighbor(map<_float, CCellLayer*>& Layers)
{
	if (nullptr == m_NeighborLayerKeys) 
	{
		//�߰��� m_NeighborLayerKeys��װ� ��������;;
		assert(0);
		Call_MsgBox(L"ū�ϳ�;; : CCellLayer");
	}
	if (nullptr == m_NeighborIndex)
	{
		//�߰��� m_NeighborIndex��װ� ��������;;
		assert(0);
		Call_MsgBox(L"ū�ϳ�;; : CCellLayer");
	}

	_uint TotalNums = m_iZNums * m_iXNums * 2;

	_uint Index = 0;
	for (auto& Cell : m_Cells)
	{
		_uint NeighborIndex = Index * 3;

		_float Keys[3] =
		{
			m_NeighborLayerKeys[NeighborIndex],
			m_NeighborLayerKeys[NeighborIndex + 1],
			m_NeighborLayerKeys[NeighborIndex + 2]
		};
		_int Indies[3] =
		{
			m_NeighborIndex[NeighborIndex],
			m_NeighborIndex[NeighborIndex + 1],
			m_NeighborIndex[NeighborIndex + 2]
		};

		for (_uint i = 0; i < CCell::LINE_END; ++i)
		{
			auto iter = Layers.find(Keys[i]);
			if (iter != Layers.end()) 
			{
				CCell* pNeighborCell = iter->second->Get_Cell(Indies[i]);
				if (pNeighborCell->Check_Attribute(CELL_STAIR))
				{
					_float Key = iter->first;
					auto StairIter = m_StairList.find(Key);
					if (StairIter == m_StairList.end())
					{
						CellList StairList;
						StairList.push_back(pNeighborCell);
						m_StairList.emplace(Key, StairList);
					}
					else
					{
						StairIter->second.push_back(pNeighborCell);
					}
				}
				Cell->Set_Neighbor(CCell::LINE(i), pNeighborCell);
			}
			else
			{
				Cell->Set_Neighbor(CCell::LINE(i), nullptr);
			}

		}

		Index++;
	}

	SAFE_DELETE_ARRAY(m_NeighborLayerKeys);
	SAFE_DELETE_ARRAY(m_NeighborIndex);
	return S_OK;
}

HRESULT CCellLayer::SetUp_Nodes()
{
	auto Iter = m_CellList.find(CELL_BLOCKED);
	
	if (Iter == m_CellList.end())
		return S_OK;

	for (auto& Cell : Iter->second)
	{
		for (_uint i = 0; i < CCell::POINT_END; ++i)
		{
			m_BlockedIncludePosition.push_back(Cell->Get_Point(CCell::POINT(i)));
		}
	}
	m_BlockedIncludePosition.unique();
	return S_OK;
}

HRESULT CCellLayer::SetUp_Visibility()
{

	//Blocked�Ž��� �������� ���� ���ü� �˻�
	//�ش� �������� �������� ��� ���� �� �̿� ����
	//A*�׽�Ʈ..

	//������ �����ϴ� ��� ������ ����
	std::list<std::pair<CNode*, CNode*>> LinkList;
	for (auto& Sour : m_Nodes)
	{
		for (auto& Dest : m_Nodes)
		{
			if (Sour == Dest)
				continue;

			std::pair<CNode*, CNode*> link = std::make_pair(Sour, Dest);
			std::pair<CNode*, CNode*> Invlink = std::make_pair(Dest, Sour);

			std::list<std::pair<CNode*, CNode*>>::iterator LinkIter =
				find_if(LinkList.begin(), LinkList.end(),
					[&link, &Invlink](std::list<std::pair<CNode*, CNode*>>::value_type& Value)
					{
						if ((link.first == Value.first && link.second == Value.second)
							|| (Invlink.first == Value.first && Invlink.second == Value.second))
							return true;
						else return false;
					});

			if (LinkList.end() == LinkIter)
			{
				LinkList.push_back(link);
			}
		}
	}

	//���ü� ���� ����
/*
	1. ���� ���� �̰�, �̿��� Blocked�϶�
	2. ���� ���� �� �̰�, ������ �ٸ� Block�� ���� �������� ��
	3. �̿��� ���� ��
*/
//link�� �ٸ� Blocked�� �Žÿ� �����ϴ°�?
	std::list<std::pair<CNode*, CNode*>>::iterator LinkIter = LinkList.begin();
	for (; LinkIter != LinkList.end();)
	{
		_bool bRemove = false;

		_float4 vLinkStart = (*LinkIter).first->Get_Positon();
		_float4 vLinkEnd = (*LinkIter).second->Get_Positon();

		_vector xLinkStart = vLinkStart.XMLoad();
		_vector xLinkEnd = vLinkEnd.XMLoad();

		xLinkStart = XMVectorSetW(xLinkStart, 0.f);
		xLinkEnd = XMVectorSetW(xLinkEnd, 0.f);
		_float2 v2LinkStart = _float2(vLinkStart.x, vLinkStart.z);
		_float2 v2LinkEnd = _float2(vLinkEnd.x, vLinkEnd.z);
		//��� Blocked����� �˻�
		for (auto& Cell : m_CellList[CELL_BLOCKED])
		{
			//Blocked���� Line���� �˻�
			std::list<_int> BlockLine;
			for (_uint i = 0; i < CCell::LINE_END; ++i)
			{
				//�̿� �ε��� ��������
				CCell* pNeighborCell = Cell->Get_NeighborCell(CCell::LINE(i));
				std::pair<_float4, _float4> LinePoints = Cell->Get_LinePoints(CCell::LINE(i));
				_float2 v2CheckStart = _float2(LinePoints.first.x, LinePoints.first.z);
				_float2 v2CheckEnd = _float2(LinePoints.second.x, LinePoints.second.z);

				//�̿��� ���� �� --> ���� ���̸� ����
				if (nullptr == pNeighborCell)
				{
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}
					continue;
				}

				//Blocked�� �̿��� �����ϴ� ���� ��ũ�� ���Ե� ��
				if (pNeighborCell->IsBlocked())
				{
					//���� �ܰ����� ���ü���ũ�� ������ ��
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}

				}
			}
			//Blocked�� �����ϴ� ���� ��
			if (Cell->Check_CrossLines(vLinkStart, vLinkEnd))
			{
				bRemove = true;
				break;
			}
		}


		if (bRemove)
		{
			LinkIter = LinkList.erase(LinkIter);
			bRemove = false;
		}
		else
		{
			LinkIter++;
		}
	}
	//������� ��� ����
	for (auto& value : LinkList)
	{
		Add_Link(value.first, value.second);
	}

	return S_OK;
}
#ifdef _DEBUG

HRESULT CCellLayer::SetUp_Vertex()
{
	m_iStride = sizeof(VTXDEFAULT);
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXDEFAULT* pVertices = new VTXDEFAULT;

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	return S_OK;
}

HRESULT CCellLayer::SetUp_Instancing()
{
	m_iInstanceStride = sizeof(VTXTRIINSTANCE);
	m_iNumInstance = m_iXNums* m_iZNums * 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXTRIINSTANCE* pInstance = new VTXTRIINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		memcpy_s(pInstance[i].vPosition, sizeof(_float3), &m_Cells[i]->Get_Point(CCell::POINT_A), sizeof(_float3));
		memcpy_s(pInstance[i].vPosition+1, sizeof(_float3), &m_Cells[i]->Get_Point(CCell::POINT_B), sizeof(_float3));
		memcpy_s(pInstance[i].vPosition+2, sizeof(_float3), &m_Cells[i]->Get_Point(CCell::POINT_C), sizeof(_float3));
		pInstance[i].vColor = Get_Color(m_Cells[i]);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	return S_OK;
}

HRESULT CCellLayer::SetUp_Index()
{
	m_iIndicesStride = sizeof(_ushort);
	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndices = m_iNumInstance;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	return S_OK;
}

HRESULT CCellLayer::SetUp_Shader()
{
	m_pDebugShader = CShader::Create(TEXT("../Bin/Shaderfiles/Shader_Debugging.hlsl"), VTXDEFAULT_TRI_INSTANCE_DECLARATION::Element, VTXDEFAULT_TRI_INSTANCE_DECLARATION::iNumElements);

	CRender_Manager::Get_Instance()->m_DebuggingShaders_OutCreate.push_back(m_pDebugShader);
	return S_OK;
}

HRESULT CCellLayer::Create_VertexBuffer()
{
	return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVB.GetAddressOf()));
}

HRESULT CCellLayer::Create_IndexBuffer()
{
	return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pIB.GetAddressOf()));
}

void CCellLayer::DebugTick()
{
	CRender_Manager::Get_Instance()->Callback_DebugRender
		+= bind(&CCellLayer::DebugRendering, this);
}

void CCellLayer::DebugRendering()
{
	//���̴� ���
	if (nullptr == m_pDebugShader)
		return;

	_float4x4 matProj = CCamera_Manager::Get_Instance()->Get_Proj();
	_float4x4 matView = CCamera_Manager::Get_Instance()->Get_View();
	_float4x4 matWorld;
	matWorld.Identity();
	//memcpy_s(&matWorld._41, sizeof(_float4), &m_vCenterPosition, sizeof(_float4));
	matWorld._42 = m_fLayerHeightMin;
	matView = matView.Transpose();
	matProj = matProj.Transpose();
	matWorld = matWorld.Transpose();
	for (auto& Cell : m_Cells)
	{
		_float4 vColor = Get_Color(Cell);

		m_pDebugShader->Set_RawValue("g_ViewMatrix", &matView, sizeof(_float4x4));
		m_pDebugShader->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_float4x4));
		m_pDebugShader->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4));
		m_pDebugShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
		m_pDebugShader->Begin(0);
		Cell->DebugRendering();
	}
}
_float4 CCellLayer::Get_Color(CCell* pCell)
{
	_float4 vColor;
	if (pCell->Check_Attribute(CELL_BLOCKED))
	{
		vColor = _float4(1.f, 0.f, 0.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_GROUND))
	{
		vColor = _float4(0.f, 1.f, 0.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_STAIR))
	{
		vColor = _float4(1.f, 0.f, 1.f, 1.f);
	}
	return vColor;
}
#endif // _DEBUG

_bool CCellLayer::Check_BlockedVisibility(_float2 v2LinkStart, _float2 v2LinkEnd, _float2 v2CheckStart, _float2 v2CheckEnd)
{
	//���� ���� �� ������ ��
	if (CUtility_Cell::Is_IncludeLine(v2LinkStart, v2LinkEnd, v2CheckStart)
		&& CUtility_Cell::Is_IncludeLine(v2LinkStart, v2LinkEnd, v2CheckEnd))
	{
		//�� �� ����
		//Blocked Link ���� ���� ������ ��
		if (CUtility_Cell::Is_SamePoint(v2LinkStart, v2CheckStart) && CUtility_Cell::Is_SamePoint(v2LinkEnd, v2CheckEnd))
			return true;
		if (CUtility_Cell::Is_SamePoint(v2LinkStart, v2CheckEnd) && CUtility_Cell::Is_SamePoint(v2LinkEnd, v2CheckStart))
			return true;

		//Blocked Link �� ���� ����
		//Start Start����
		if (CUtility_Cell::Is_SamePoint(v2CheckStart, v2LinkStart))
		{
			//�� �� ���̿� ������ ����
			if (CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckEnd))
				return true;
			//�ۿ� ������ ���� �����̸� ����
			if (CUtility_Cell::Is_SameDir(v2LinkStart, v2LinkEnd, v2CheckEnd))
				return true;

		}
		//End Start����
		if (CUtility_Cell::Is_SamePoint(v2CheckEnd, v2LinkStart))
		{
			//�� �� ���̿� ������ ����
			if (CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckStart))
				return true;
			//�ۿ� ������ ���� �����̸� ����
			if (CUtility_Cell::Is_SameDir(v2LinkStart, v2LinkEnd, v2CheckStart))
				return true;

		}
		//Start End����
		if (CUtility_Cell::Is_SamePoint(v2CheckStart, v2LinkEnd))
		{
			//�� �� ���̿� ������ ����
			if (CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckEnd))
				return true;
			//�ۿ� ������ ���� �����̸� ����
			if (CUtility_Cell::Is_SameDir(v2LinkEnd, v2LinkStart, v2CheckEnd))
				return true;

		}
		//End End����
		if (CUtility_Cell::Is_SamePoint(v2CheckEnd, v2LinkEnd))
		{
			//�� �� ���̿� ������ ����
			if (CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckStart))
				return true;
			//�ۿ� ������ ���� �����̸� ����
			if (CUtility_Cell::Is_SameDir(v2LinkEnd, v2LinkStart, v2CheckStart))
				return true;

		}//End����

		//�����Ǵ� ��X
		if (CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckStart) && CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckEnd))
		{
			return true;
		}
		if (CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckStart) && CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckEnd))
		{
			return true;
		}

	}


	return false;
}

CCellLayer::CellList CCellLayer::Get_BestRoute(CNode* pStartNode, CNode* pEndNode)
{
	if (nullptr == pStartNode || nullptr == pEndNode)
		return CellList();

	if(FAILED(Ready_SearchingChild()))
		return CellList();

	CellList BestCellList;

	NODES BestRoute;

	NODES OpenList;
	NODES CloseList;

	//pStartNode�� �޸���ƽ ��� ������Ʈ
	pStartNode->Set_HCost(pEndNode);

	//StartNode�� ���ü� ����
	Set_Visiblity(pStartNode);

	//EndNode�� ���ü� ����
	Set_Visiblity(pEndNode);


	//Start-End�� ���ü� �˻�� ����
	_bool bRemove = false;
	_float4 vLinkStart = pStartNode->Get_Positon();
	_float4 vLinkEnd = pEndNode->Get_Positon();

	_float2 v2LinkStart = _float2(vLinkStart.x, vLinkStart.z);
	_float2 v2LinkEnd = _float2(vLinkEnd.x, vLinkEnd.z);
	//A. ���귮 ���� : ��� Blocked���� Ȯ����
	for (auto& Cell : m_CellList[CELL_BLOCKED])
	{
		//Blocked���� Line���� �˻�
		for (_uint i = 0; i < CCell::LINE_END; ++i)
		{
			//�̿� �ε��� ��������
			CCell* pNeighborCell = Cell->Get_NeighborCell(CCell::LINE(i));
			std::pair<_float4, _float4> LinePoints = Cell->Get_LinePoints(CCell::LINE(i));
			_float2 v2CheckStart = _float2(LinePoints.first.x, LinePoints.first.z);
			_float2 v2CheckEnd = _float2(LinePoints.second.x, LinePoints.second.z);

			//�̿��� ���� �� --> ���� ���̸� ����
			if (nullptr == pNeighborCell)
			{
				if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
				{
					bRemove = true;
					break;
				}
				continue;
			}

			//Blocked�� �̿��� �����ϴ� ���� ��ũ�� ���Ե� ��
			if (pNeighborCell->IsBlocked())
			{
				//���� �ܰ����� ���ü���ũ�� ������ ��
				if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
				{
					bRemove = true;
					break;
				}

			}
		}
		//Blocked�� �����ϴ� ���� ��
		if (Cell->Check_CrossLines(vLinkStart, vLinkEnd))
		{
			bRemove = true;
			break;
		}
	}

	if(!bRemove)
		Add_SearchLink(pStartNode, pEndNode);

	OpenList.push_back(pStartNode);

	if (Bake_OpenCloseList(OpenList, CloseList, pEndNode))
	{
		//�Ǹ������� ���� ���� �θ���� �Ž��� �ö󰡸鼭 ��� ����
		Bake_BestList(BestRoute, OpenList, CloseList, pEndNode);
		BestCellList = Make_CellList(BestRoute);
		Clear_Nodes();
		return BestCellList;
	}//ã��
	else
	{
		BestRoute.clear();
		Clear_Nodes();
		return  CellList();
	}//��ã��

}

void CCellLayer::Add_Link(CNode* pStart, CNode* pEnd)
{
	pStart->Add_OriginChild(pEnd);
	pEnd->Add_OriginChild(pStart);

}

void CCellLayer::Add_SearchLink(CNode* pStart, CNode* pEnd)
{
	pStart->Add_SearchChild(pEnd);
	pEnd->Add_SearchChild(pStart);
}

HRESULT CCellLayer::Ready_SearchingChild()
{
	for (auto& Value : m_Nodes)
	{
		Value->Ready_SearchingChild();
	}
	return S_OK;
}

void CCellLayer::Set_Visiblity(CNode* pNode)
{
	std::list<std::pair<CNode*, CNode*>> LinkList;
	for (auto& Dest : m_Nodes)
	{
		if (pNode == Dest)
			continue;

		std::pair<CNode*, CNode*> link = std::make_pair(pNode, Dest);
		std::pair<CNode*, CNode*> Invlink = std::make_pair(Dest, pNode);

		std::list<std::pair<CNode*, CNode*>>::iterator LinkIter =
			find_if(LinkList.begin(), LinkList.end(),
				[&link, &Invlink](std::list<std::pair<CNode*, CNode*>>::value_type& Value)
				{
					if ((link.first == Value.first && link.second == Value.second)
						|| (Invlink.first == Value.first && Invlink.second == Value.second))
						return true;
					else return false;
				});

		if (LinkList.end() == LinkIter)
		{
			LinkList.push_back(link);
		}
	}

	std::list<std::pair<CNode*, CNode*>>::iterator LinkIter = LinkList.begin();
	for (; LinkIter != LinkList.end();)
	{
		_bool bRemove = false;

		_float4 vLinkStart = (*LinkIter).first->Get_Positon();
		_float4 vLinkEnd = (*LinkIter).second->Get_Positon();

		_vector xLinkStart = vLinkStart.XMLoad();
		_vector xLinkEnd = vLinkEnd.XMLoad();

		xLinkStart = XMVectorSetW(xLinkStart, 0.f);
		xLinkEnd = XMVectorSetW(xLinkEnd, 0.f);
		_float2 v2LinkStart = _float2(vLinkStart.x, vLinkStart.z);
		_float2 v2LinkEnd = _float2(vLinkEnd.x, vLinkEnd.z);
		//��� Blocked����� �˻�
		for (auto& Cell : m_CellList[CELL_BLOCKED])
		{
			//Blocked���� Line���� �˻�
			std::list<_int> BlockLine;
			for (_uint i = 0; i < CCell::LINE_END; ++i)
			{
				//�̿� �ε��� ��������
				CCell* pNeighborCell = Cell->Get_NeighborCell(CCell::LINE(i));
				std::pair<_float4, _float4> LinePoints = Cell->Get_LinePoints(CCell::LINE(i));
				_float2 v2CheckStart = _float2(LinePoints.first.x, LinePoints.first.z);
				_float2 v2CheckEnd = _float2(LinePoints.second.x, LinePoints.second.z);

				//�̿��� ���� �� --> ���� ���̸� ����
				if (nullptr == pNeighborCell)
				{
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}
					continue;
				}

				//Blocked�� �̿��� �����ϴ� ���� ��ũ�� ���Ե� ��
				if (pNeighborCell->IsBlocked())
				{
					//���� �ܰ����� ���ü���ũ�� ������ ��
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}

				}
			}
			//Blocked�� �����ϴ� ���� ��
			if (Cell->Check_CrossLines(vLinkStart, vLinkEnd))
			{
				bRemove = true;
				break;
			}
		}


		if (bRemove)
		{
			LinkIter = LinkList.erase(LinkIter);
			bRemove = false;
		}
		else
		{
			LinkIter++;
		}
	}
	for (auto& value : LinkList)
	{
		Add_SearchLink(value.first, value.second);
	}

}

CCellLayer::CellList CCellLayer::Get_StairCellList(_float fLayerKey)
{
	auto StairIter = m_StairList.find(fLayerKey);
	if (StairIter == m_StairList.end())
		return CellList();
	else
		return StairIter->second;
}

_bool CCellLayer::Bake_OpenCloseList(NODES& rhsOpenNodeList, NODES& rhsCloseNodeList, CNode* pEndNode)
{
	if (rhsOpenNodeList.empty())
		return false;

	//���� �� ����� ��� ��������
	CNode* pSelectNode = Pop_CheapestNode(rhsOpenNodeList);

	rhsCloseNodeList.push_back(pSelectNode);
	if (Check_EndNode(rhsOpenNodeList, rhsCloseNodeList, pSelectNode, pEndNode))
		return true;
	return Bake_OpenCloseList(rhsOpenNodeList, rhsCloseNodeList, pEndNode);
}

_bool CCellLayer::Check_EndNode(NODES& rhsOpenList, NODES& rhsCloseList, CNode* pSelectNode, CNode* pEndNode)
{
	// G : ������ ���� ���� Ÿ�ϱ����� �ҿ��� --> ���� Ÿ���� �θ��� G��뿡 ���� Ÿ���� ����� ���Ѵ�
		// H : ���� Ÿ�Ͽ��� ������������ ���� �ҿ��� --> 
		// F : �� ���

	_uint Select_GCost = pSelectNode->Get_TotalGCost();
	NODES& ChildList = pSelectNode->Get_SearchChildList();
	for (auto& Child : ChildList)
	{
		if (Child == pEndNode)
			return true;

		NODES::iterator CloseIter = find_if(rhsCloseList.begin(), rhsCloseList.end(), [&Child](NODES::value_type Value)
			{
				if (Value == Child)
					return true;
				else
					return false;
			});

		//�ڽĳ���� �˻簡 �̹� �����ٸ�..
		if (rhsCloseList.end() != CloseIter)
			continue;

		Child->Set_CurGCost(pSelectNode);
		Child->Set_HCost(pEndNode);

		_uint GCost = Child->Get_CurGCost() + Select_GCost;
		_uint HCost = Child->Get_HCost();
		_uint FCost = GCost + HCost;

		NODES::iterator OpenIter = find_if(rhsOpenList.begin(), rhsOpenList.end(), [&Child](NODES::value_type Value)
			{
				if (Value == Child)
					return true;
				else
					return false;
			});

		if (rhsOpenList.end() == OpenIter)
		{
			Child->Set_Parent(pSelectNode);
			Child->Set_TotalGCost(GCost);
			rhsOpenList.push_back(Child);
		}
		else
		{
			//���� �ڽĳ���� GCost�� ������ ����ִ� ����� GCost�� �� ������Ʈ
			if (GCost < (*OpenIter)->Get_TotalGCost())
			{
				(*OpenIter)->Set_Parent(pSelectNode);
				(*OpenIter)->Set_TotalGCost(GCost);
			}
		}
	}
	return false;
}

void CCellLayer::Bake_BestList(NODES& OutBestList, NODES& OpenNodeList, NODES& CloseNodeList, CNode* pEnd)
{
	//���⼭ CloseList�� ������ ��� ������
	NODES ResultList;
	ResultList.push_back(pEnd);
	CNode* pNode = CloseNodeList.back();

	while (nullptr != pNode)
	{
		ResultList.push_back(pNode);
		pNode = pNode->Get_Parent();
	}
	//�� Ÿ�ֿ̹� ResultList�� End���� Start����

	//�Ųٷ� �Ҵ�
	OutBestList.assign(ResultList.rbegin(), ResultList.rend());
}

CCellLayer::CellList CCellLayer::Make_CellList(NODES& OutBestList)
{
	//�� ��� ������ OpenCell���� ������
	CellList BestList;
	CNode* pStartNode = nullptr;
	CNode* pEndNode = nullptr;
	for (auto& Node : OutBestList)
	{
		pEndNode = Node;
		Find_NearCell(pStartNode, pEndNode, BestList);
		pStartNode = pEndNode;
	}
	//��׸� return;
	return BestList;
}

void CCellLayer::Find_NearCell(CNode* pStartNode, CNode* pEndNode, CellList& rhsCellList)
{
	if (nullptr == pStartNode)
		return;
	if (nullptr == pEndNode)
		return;
	_float4 vStartPosition = pStartNode->Get_Positon();
	_float4 vEndPosition = pEndNode->Get_Positon();
	
	for (auto& Cell : m_Cells)
	{
		if (Cell->IsBlocked())
			continue;
		if (Cell->Check_CrossLines(vStartPosition, vEndPosition))//������ ���� �����ϴ��� üũ
		{
			rhsCellList.push_back(Cell);
		}
	}

}

void CCellLayer::Clear_Nodes()
{
	for (auto& Node : m_Nodes)
	{
		Node->Clear_Node();
	}
}

CCell* CCellLayer::Find_Cell(_float4 vPosition)
{
	_float4 vFindingPos = _float4(vPosition.x, vPosition.y, vPosition.z);
	_float4 vHalfCenter = _float4(m_vCenterPosition.x * 0.5f, 0.f, m_vCenterPosition.z * 0.5f, 1.f);
	vFindingPos += vHalfCenter;
	//vFindingPos -= vHalfCenter;
	//_int Index = _int(((vFindingPos.z / m_fTileSize) - vHalfCenter.z * m_iXNums * 2)) + _int((vFindingPos.x / m_fTileSize) - vHalfCenter.x);

	if (_int(vFindingPos.x / m_fTileSize) >= m_iXNums)
		return nullptr;

	if ((_int(vFindingPos.z / m_fTileSize)) >= m_iZNums)
		return nullptr;

	_int Index = ((_int(vFindingPos.z / m_fTileSize)) * m_iXNums + _int(vFindingPos.x/ m_fTileSize)) * 2;
	if (Index < 0)
		return nullptr;

	if (m_Cells.size() <= Index)
		return nullptr;


	CCell* pLeftCell = m_Cells[Index];
	CCell* pRightCell = m_Cells[Index + 1];

	CCell::LinePair Pair = pRightCell->Get_LinePoints(CCell::LINE_AB);
	_float2 Points[3] =
	{
		_float2(Pair.first.x, Pair.first.z),
		_float2(Pair.second.x, Pair.second.z),
		_float2(vPosition.x, vPosition.z)
	};
	_int Dir = CUtility_Cell::ccw(Points[0], Points[1], Points[2]);

	if (Dir < 0)
	{
		return pRightCell;
	}
	else
	{
		return pLeftCell;
	}


}
CCell* CCellLayer::Get_Cell(_int Index)
{
	if (Index < 0 || Index >= _int(m_Cells.size()))
		return nullptr;
	return m_Cells[Index];
}
void CCellLayer::Set_MinHeight(_float Height) 
{
	m_fLayerHeightMin = Height;
	for (auto& Cell : m_Cells)
	{
		Cell->Set_LayerKey(m_fLayerHeightMin);
	}
}
CNode* CCellLayer::Pop_CheapestNode(NODES& rhsOpenList)
{
	//���� Ž�� �ʿ� Ÿ�� ���� --> ���ä�� --> F = G + H
	//���� ���� ���� ũ���� FCost�� ������ Openlist�� �־���

	//FCost���� �������� ����
	rhsOpenList.sort([](NODES::value_type& Sour, NODES::value_type& Dest) {
		if (Sour->Get_FCost() < Dest->Get_FCost())
			return true;
		else return false;
		});

	CNode* TargetNode = rhsOpenList.front();
	rhsOpenList.pop_front();

	return TargetNode;
}

void CCellLayer::Add_CellList(_uint iCellAttribute, CCell* pCell)
{
	if (nullptr == pCell)
		return;
	if(pCell->Check_Attribute(iCellAttribute))
	{
		auto CellListIter = m_CellList.find(iCellAttribute);
		if (CellListIter == m_CellList.end())
		{
			CellList List;
			List.push_back(pCell);
			m_CellList.emplace(iCellAttribute, List);
		}
		else
		{
			CellListIter->second.push_back(pCell);
		}
	}

}
