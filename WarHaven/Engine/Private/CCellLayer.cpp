#include "CCellLayer.h"
#include "CCell.h"
#include "CUtility_Cell.h"
#include "CNode.h"
#ifdef _DEBUG
	#include "GameInstance.h"
	#include "Render_Manager.h"
	#include "CShader.h"
#endif
#include <thread>
CCellLayer::CCellLayer()
{
}

CCellLayer::~CCellLayer()
{
	for (auto& Cell : m_Cells)
		SAFE_DELETE(Cell);
	for (auto& Node : m_Nodes)
		SAFE_DELETE(Node);
	SAFE_DELETE_ARRAY(m_NeighborLayerKeys);
	SAFE_DELETE_ARRAY(m_NeighborIndex);
#ifdef _DEBUG
#ifdef DEBUGRENDER
	_ulong dwCnt = m_pIB.Reset();
	dwCnt = m_pVB.Reset();
	m_pVBInstance.Reset();
#endif
#endif // _DEBUG
	m_ThreadArr.clear();
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
#ifdef DEBUGRENDER
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
		Call_MsgBox(L"Failed to SetUp_Index : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Shader()))
	{
		Call_MsgBox(L"Failed to SetUp_Shader : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#endif
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

CCellLayer* CCellLayer::Create(wstring strFolderPath, wstring strFolderName)
{
	CCellLayer* pInstance = new CCellLayer();
	pInstance->m_strDebugName = strFolderName;
	if (FAILED(pInstance->SetUp_Cells(strFolderPath)))
	{
		Call_MsgBox(L"Failed to SetUp_Cells : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	//if (FAILED(pInstance->SetUp_Nodes()))
	//{
	//	Call_MsgBox(L"Failed to SetUp_Nodes : CCellLayer");
	//	SAFE_DELETE(pInstance);
	//}	
	//if (FAILED(pInstance->SetUp_Visibility()))
	//{
	//	Call_MsgBox(L"Failed to SetUp_Visibility : CCellLayer");
	//	SAFE_DELETE(pInstance);
	//}
#ifdef _DEBUG
#ifdef DEBUGRENDER
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
		Call_MsgBox(L"Failed to SetUp_Index : CCellLayer");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_Shader()))
	{
		Call_MsgBox(L"Failed to SetUp_Shader : CCellLayer");
		SAFE_DELETE(pInstance);
	}
#endif
#endif
	return pInstance;
}

CCellLayer* CCellLayer::Clone_Layer(_float MinHeight)
{
	CCellLayer* pCellLayer = CCellLayer::Create(m_iXNums, m_iZNums, m_fTileSize, m_vCenterPosition, MinHeight, MinHeight+1.f);

	for (_uint i = 0; i < m_Cells.size(); ++i) 
	{
		pCellLayer->m_Cells[i]->Set_Flags(m_Cells[i]->Get_Attribute());
	}
	pCellLayer->Reset_Neighbor();
	return pCellLayer;
}

void CCellLayer::Save(wstring strForlderPath, wstring strForlderName)
{
	wstring SavePath = strForlderPath;
	//SavePath += strForlderName;
	//SavePath += L"/";
	if (!filesystem::exists(SavePath))
	{
		filesystem::create_directory(SavePath);
	}

	wstring strInfoPath = SavePath;
	strInfoPath += strForlderName;
	strInfoPath += L"_Info.bin";
	Save_Info(strInfoPath);

	wstring strAttributePath = SavePath;
	strAttributePath += strForlderName;
	strAttributePath += L"_Attribute.bin";
	Save_Attribute(strAttributePath);

	wstring strNeighborKeyPath = SavePath;
	strNeighborKeyPath += strForlderName;
	strNeighborKeyPath += L"_Keys.bin";
	Save_NeighborKeys(strNeighborKeyPath);

	wstring strNeighborIndicesPath = SavePath;
	strNeighborIndicesPath += strForlderName;
	strNeighborIndicesPath += L"_Index.bin";
	Save_NeighborIndices(strNeighborIndicesPath);

}

void CCellLayer::Save_Info(wstring strInfoPath)
{
	ofstream writeFile;
	writeFile.open(strInfoPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save_Info : CCellLayer");
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
	writeFile.close();
}

void CCellLayer::Save_Attribute(wstring strAttributePath)
{
	ofstream writeFile;
	writeFile.open(strAttributePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save_NeighborIndices : CCellLayer");
		writeFile.close();
		return;
	}
	_uint Size = m_iXNums * m_iZNums * 2;
	_byte* ArrAttribute = new _byte[Size];
	_uint Index = 0;
	for (auto& Cell : m_Cells)
	{
		ArrAttribute[Index] = Cell->Get_Attribute();
		Index++;
	}
	writeFile.write((char*)&Size, sizeof(_uint));
	writeFile.write((char*)ArrAttribute, sizeof(_byte)* Size);
	
	writeFile.close();

	SAFE_DELETE_ARRAY(ArrAttribute);
}

void CCellLayer::Save_NeighborIndices(wstring strNeighborIndicesPath)
{
	ofstream writeFile;
	writeFile.open(strNeighborIndicesPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save_NeighborIndices : CCellLayer");
		writeFile.close();
		return;
	}

	_uint Size = m_iXNums * m_iZNums * 2 * 3;
	_int* Indices = new _int[Size];
	_uint Index = 0;
	writeFile.write((char*)&Size, sizeof(_uint));
	for (auto& Cell : m_Cells)
	{
		Indices[Index++] 
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_AB)) 
			? -1 
			: Cell->Get_NeighborCell(CCell::LINE_AB)->Get_Index();

		Indices[Index++] 
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_BC)) 
			? -1 
			: Cell->Get_NeighborCell(CCell::LINE_BC)->Get_Index();

		Indices[Index++]
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_CA)) 
			? -1
			: Cell->Get_NeighborCell(CCell::LINE_CA)->Get_Index();

		int a = 0;
		writeFile.write((char*)&Indices[Index - 3], sizeof(_int));
		writeFile.write((char*)&Indices[Index - 2], sizeof(_int));
		writeFile.write((char*)&Indices[Index - 1], sizeof(_int));

	}

	writeFile.close();
	SAFE_DELETE_ARRAY(Indices);
}

void CCellLayer::Save_NeighborKeys(wstring strNeighborKeysPath)
{
	ofstream writeFile;
	writeFile.open(strNeighborKeysPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save_NeighborKeys : CCellLayer");
		writeFile.close();
		return;
	}

	_uint Size = m_iXNums * m_iZNums * 2 * 3;
	_float* Keys = new _float[Size];
	_uint Index = 0;
	for (auto& Cell : m_Cells)
	{
		Keys[Index++]
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_AB))
			? 9999999.f
			: Cell->Get_NeighborCell(CCell::LINE_AB)->Get_LayerKey();

		Keys[Index++]
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_BC))
			? 9999999.f
			: Cell->Get_NeighborCell(CCell::LINE_BC)->Get_LayerKey();

		Keys[Index++]
			= (nullptr == Cell->Get_NeighborCell(CCell::LINE_CA))
			? 9999999.f
			: Cell->Get_NeighborCell(CCell::LINE_CA)->Get_LayerKey();

	}
	writeFile.write((char*)&Size, sizeof(_uint));
	writeFile.write((char*)Keys, sizeof(_float) * Size);

	writeFile.close();
	SAFE_DELETE_ARRAY(Keys);
}

void CCellLayer::Save_Visiblity(wstring strNeighborKeysPath)
{
	if (m_LinkList.empty())
		return;

	ofstream writeFile;
	writeFile.open(strNeighborKeysPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to Save_Visiblity : CCellLayer");
		writeFile.close();
		return;
	}

	_uint Size = m_LinkList.size();
	writeFile.write((char*)&Size, sizeof(_uint));
	for (auto& value : m_LinkList)
	{
		writeFile.write((char*)&(value.first->Get_Positon()), sizeof(_float4));
		writeFile.write((char*)&(value.second->Get_Positon()), sizeof(_float4));
	}
}

void CCellLayer::Reset_Neighbor()
{
	m_CellList.clear();

	_uint Index = 0;
	for (auto& Cell : m_Cells)
	{
		CCell* pCell[3] = {};
		if (Index % 2 == 0)
		{
			pCell[0] = ((Index) % (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - 1];//왼쪽 끝
			pCell[1] = ((Index / (m_iXNums * 2)) == m_iZNums - 1) ? nullptr : m_Cells[Index + (m_iXNums * 2)];//맨 위
			pCell[2] = m_Cells[Index + 1];
		}
		else
		{
			pCell[0] = m_Cells[Index - 1];//왼쪽 끝
			pCell[1] = ((Index) % (m_iXNums * 2) == (m_iXNums * 2) - 1) ? nullptr : m_Cells[Index + 1];//오른쪽 끝
			pCell[2] = (Index / (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - (m_iXNums * 2)];//맨 아래
		}
		Cell->Set_Neighbor(CCell::LINE_AB, pCell[0]);
		Cell->Set_Neighbor(CCell::LINE_BC, pCell[1]);
		Cell->Set_Neighbor(CCell::LINE_CA, pCell[2]);
		Index++;
	}

	SetUp_CellList();

}

//로컬 기준 0, 0 이 좌하단
HRESULT CCellLayer::SetUp_Cells(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _int iAttribute)
{
	m_vCenterPosition = vCenterPosition;
	m_iXNums = XNums;
	m_iZNums = ZNums;
	m_fTileSize = fTileSize;

	CCell* pLeftTri = nullptr;
	CCell* pRightTri = nullptr;

	_float4 vHalfCenter = _float4(vCenterPosition.x * 0.5f, vCenterPosition.y * 0.5f, vCenterPosition.z * 0.5f, 1.f);
	//셀 생성
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
			pCell[0] = ((Index) % (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - 1];//왼쪽 끝
			pCell[1] = ((Index / (m_iXNums * 2)) == m_iZNums - 1) ? nullptr : m_Cells[Index + (m_iXNums*2)];//맨 위
			pCell[2] = m_Cells[Index + 1];
		}
		else
		{
			pCell[0] = m_Cells[Index - 1];//왼쪽 끝
			pCell[1] = ((Index) % (m_iXNums * 2) == (m_iXNums * 2) - 1) ? nullptr : m_Cells[Index + 1];//오른쪽 끝
			pCell[2] = (Index / (m_iXNums * 2) == 0) ? nullptr : m_Cells[Index - (m_iXNums*2)];//맨 아래
		}
		Cell->Set_Neighbor(CCell::LINE_AB, pCell[0]);
		Cell->Set_Neighbor(CCell::LINE_BC, pCell[1]);
		Cell->Set_Neighbor(CCell::LINE_CA, pCell[2]);
		Index++;
	}
	//셀 정보 설정
	//셀 정보 토대로 Cell리스트 저장
	//Blocked셀이면 각 정점을 노드로 만든 후 m_Nodes에 저장

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
	wstring strLoadPath = strFilePath;
	strLoadPath += L"/";
	m_strFolderPath = strLoadPath;
	//strLoadPath

	wstring strLoadInfoPath = strLoadPath;
	strLoadInfoPath += m_strDebugName;
	strLoadInfoPath += L"_Info.bin";
	Load_LayerInfo(strLoadInfoPath);

	wstring strLoadAttributePath = strLoadPath;
	strLoadAttributePath += m_strDebugName;
	strLoadAttributePath += L"_Attribute.bin";
	Load_LayerAttribute(strLoadAttributePath);

	wstring strLoadIndexPath = strLoadPath;
	strLoadIndexPath += m_strDebugName;
	strLoadIndexPath += L"_Index.bin";
	Load_LayerNeighborIndices(strLoadIndexPath);

	wstring strLoadKeysPath = strLoadPath;
	strLoadKeysPath += m_strDebugName;
	strLoadKeysPath += L"_Keys.bin";
	Load_LayerNeighborKeys(strLoadKeysPath);

	if (nullptr == m_NeighborLayerKeys)
	{
		//m_NeighborLayerKeys생성 안됨
		assert(0);
	}
	if (nullptr == m_NeighborIndex)
	{
		//m_NeighborIndex생성 안됨
		assert(0);
	}

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
		//중간에 m_NeighborLayerKeys얘네가 없어진거;;
		assert(0);
		Call_MsgBox(L"큰일남;; : CCellLayer");
	}
	if (nullptr == m_NeighborIndex)
	{
		//중간에 m_NeighborIndex얘네가 없어진거;;
		assert(0);
		Call_MsgBox(L"큰일남;; : CCellLayer");
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

		if (Cell->Check_Attribute(CELL_GROUND))
			int a = 0;

		for (_uint i = 0; i < CCell::LINE_END; ++i)
		{
			auto iter = Layers.find(Keys[i]);
			if (iter != Layers.end()) 
			{
				CCell* pNeighborCell = iter->second->Get_Cell(Indies[i]);
				if (nullptr == pNeighborCell)
					continue;
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

	SetUp_CellList();

	
	SAFE_DELETE_ARRAY(m_NeighborLayerKeys);
	SAFE_DELETE_ARRAY(m_NeighborIndex);
	return S_OK;
}

HRESULT CCellLayer::Load_LayerInfo(wstring strFilePath)
{
	ifstream readFile;
	string strFile;
	strFile.assign(strFilePath.begin(), strFilePath.end());
	readFile.open(strFile, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to Load");
		readFile.close();
		return E_FAIL;
	}

	_uint NameSize;
	char szName[MAXCHAR] = "";
	readFile.read((char*)&NameSize, sizeof(_uint));
	readFile.read(szName, sizeof(char) * NameSize);

	readFile.read((char*)&m_vCenterPosition, sizeof(_float4));
	readFile.read((char*)&m_iXNums, sizeof(_uint));
	readFile.read((char*)&m_iZNums, sizeof(_uint));
	readFile.read((char*)&m_fTileSize, sizeof(_float));
	readFile.read((char*)&m_fLayerHeightMin, sizeof(_float));
	readFile.read((char*)&m_fLayerHeightMax, sizeof(_float));

	readFile.close();
	
	return S_OK;
}

HRESULT CCellLayer::Load_LayerAttribute(wstring strFilePath)
{
	ifstream readFile;
	string strFile;
	strFile.assign(strFilePath.begin(), strFilePath.end());
	readFile.open(strFile, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to Load");
		readFile.close();
		return E_FAIL;
	}

	_uint Size;
	readFile.read((char*)&Size, sizeof(_uint));

	_byte* ArrAttribute = new _byte[Size];
	readFile.read((char*)ArrAttribute, sizeof(_byte) * Size);

	if (FAILED(Make_Cells(ArrAttribute)))
	{
		assert(0);
	}
	
	SAFE_DELETE_ARRAY(ArrAttribute);

	readFile.close();

	return S_OK;
}

HRESULT CCellLayer::Load_LayerNeighborKeys(wstring strFilePath)
{
	ifstream readFile;
	string strFile;
	strFile.assign(strFilePath.begin(), strFilePath.end());
	readFile.open(strFile, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to Load");
		readFile.close();
		return E_FAIL;
	}

	_uint Size = 0;
	readFile.read((char*)&Size, sizeof(_uint));

	_float* Keys = new _float[Size];
	for (_uint i = 0; i < Size; ++i) 
	{
		_float fKey = 5845234.f;
		readFile.read((char*)&fKey, sizeof(_float));
		Keys[i] = fKey;
	}
	readFile.close();

	m_NeighborLayerKeys = Keys;
	return S_OK;
}

HRESULT CCellLayer::Load_LayerNeighborIndices(wstring strFilePath)
{
	ifstream readFile;
	string strFile;
	strFile.assign(strFilePath.begin(), strFilePath.end());
	readFile.open(strFile, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to Load");
		readFile.close();
		return E_FAIL;
	}

	_uint Size = 0;
	readFile.read((char*)&Size, sizeof(_uint));

	_int* Indices = new _int[Size];
	for (_uint i = 0; i < Size; ++i) {
		_int Index = 0;
		readFile.read((char*)&Index, sizeof(_int));
		Indices[i] = Index;
	}

	readFile.close();

	m_NeighborIndex = Indices;
	return S_OK;
}

HRESULT CCellLayer::Load_Visibility(wstring strFilePath)
{
	ifstream readFile(strFilePath, ios::binary);

	if (!readFile.is_open())
	{
		assert(0);
	}


	//list<pair<_float4, _float4>> LinkList;
	_uint Size = 0;
	readFile.read((char*)&Size, sizeof(_uint));
	for (_uint i = 0; i < Size; ++i)
	{
		_float4 vFirst, vSecond;
		readFile.read((char*)&vFirst, sizeof(_float4));
		readFile.read((char*)&vSecond, sizeof(_float4));
		m_LinkPosList.push_back(make_pair(vFirst, vSecond));
	}

	for (auto& value : m_LinkPosList)
	{
		CNode* StartNode = Get_Node(value.first);
		CNode* EndNode = Get_Node(value.second);
		if (StartNode == nullptr || EndNode == nullptr)
			assert(0);
		Add_Link(StartNode, EndNode);
	}
	
	return S_OK;
}

HRESULT CCellLayer::Load_Visibility()
{
	wstring strFilePath = m_strFolderPath;
	strFilePath += m_strDebugName;
	strFilePath += L"_Visibility.bin";
	return Load_Visibility(strFilePath);
}

HRESULT CCellLayer::SetUp_Nodes()
{
	//auto Iter = m_CellList.find(CELL_BLOCKED);
	//auto GroundIter = m_CellList.find(CELL_GROUND);
	//auto WallIter = m_CellList.find(CELL_WALL);
	//auto GrassIter = m_CellList.find(CELL_GRASS);
	//auto STAIRIter = m_CellList.find(CELL_STAIR);
	//if (Iter == m_CellList.end())
	//	return S_OK;

	for (auto& CellList : m_CellList)
	{
		if (CellList.first == CELL_BLOCKED)
			continue;
		for (auto& Cell : CellList.second)
		{
			for (_uint i = 0; i < CCell::LINE_END; ++i)
			{
				CCell* pNeighbor = Cell->Get_NeighborCell(CCell::LINE(i));
				if (nullptr == pNeighbor)
					continue;
				if (pNeighbor->Check_Attribute(CELL_BLOCKED))
				{
					CCell::LinePair Pair = Cell->Get_LinePoints(CCell::LINE(i));
					m_BlockedIncludePosition.push_back(Pair.first);
					m_BlockedIncludePosition.push_back(Pair.second);
					m_BlockedIncludePosition.unique();
				}
			}
		}
	}

	//if (GroundIter != m_CellList.end())
	//{
	//	for (auto& Cell : GroundIter->second)
	//	{
	//		for (_uint i = 0; i < CCell::LINE_END; ++i)
	//		{
	//			CCell* pNeighbor = Cell->Get_NeighborCell(CCell::LINE(i));
	//			if (nullptr == pNeighbor)
	//				continue;
	//			if (pNeighbor->Check_Attribute(CELL_BLOCKED))
	//			{
	//				CCell::LinePair Pair = Cell->Get_LinePoints(CCell::LINE(i));
	//				m_BlockedIncludePosition.push_back(Pair.first);
	//				m_BlockedIncludePosition.push_back(Pair.second);
	//				m_BlockedIncludePosition.unique();
	//			}
	//		}
	//	}
	//}
	//if (STAIRIter != m_CellList.end())
	//{
	//	for (auto& Cell : STAIRIter->second)
	//	{
	//		for (_uint i = 0; i < CCell::LINE_END; ++i)
	//		{
	//			CCell* pNeighbor = Cell->Get_NeighborCell(CCell::LINE(i));
	//			if (nullptr == pNeighbor)
	//				continue;
	//			if (pNeighbor->Check_Attribute(CELL_BLOCKED))
	//			{
	//				CCell::LinePair Pair = Cell->Get_LinePoints(CCell::LINE(i));
	//				m_BlockedIncludePosition.push_back(Pair.first);
	//				m_BlockedIncludePosition.push_back(Pair.second);
	//				m_BlockedIncludePosition.unique();
	//			}
	//		}
	//	}
	//}

	//for (auto& Cell : Iter->second)
	//{
	//	for (_uint i = 0; i < CCell::LINE_END; ++i)
	//	{
	//		CCell* pNeighbor = Cell->Get_NeighborCell(CCell::LINE(i));
	//		if (nullptr != pNeighbor)
	//		{
	//			if (pNeighbor->Check_Attribute(CELL_GROUND) || pNeighbor->Check_Attribute(CELL_STAIR))
	//			{
	//				CCell::LinePair Pair = Cell->Get_LinePoints(CCell::LINE(i));
	//				m_BlockedIncludePosition.push_back(Pair.first);
	//				m_BlockedIncludePosition.push_back(Pair.second);
	//			}
	//		}
	//	}
	//}
	//m_BlockedIncludePosition.unique();

	for (auto SourIter = m_BlockedIncludePosition.begin(); SourIter != m_BlockedIncludePosition.end(); ++SourIter)
	{
		auto DestIter = SourIter;
		for (; DestIter != m_BlockedIncludePosition.end(); )
		{
			if (DestIter == SourIter)
			{
				++DestIter;
				continue;
			}

			if (*SourIter == *DestIter)
			{
				DestIter = m_BlockedIncludePosition.erase(DestIter);
			}
			else
			{
				++DestIter;
			}
		}

	}
	for (auto& sour : m_BlockedIncludePosition)
	{
		for (auto& dest : m_BlockedIncludePosition)
		{
			if (sour == dest)
				int a = 0;
		}
	}

	for (auto& value : m_BlockedIncludePosition)
	{
		CNode* pNode = CNode::Create(value);
		m_Nodes.push_back(pNode);
	}
	return S_OK;
}

HRESULT CCellLayer::SetUp_Visibility()
{

	//Blocked매시의 정점들을 통해 가시성 검사
	//해당 정점들을 기준으로 노드 설정 및 이웃 설정
	//A*테스트..

	//정점을 연결하는 모든 순서쌍 생성
	m_LinkList.clear();
	Make_Dump(string("StartTimeLog"), string("Start_VisiblityCheck"));

	list<pair<CNode*, CNode*>>::iterator LinkCmpIter = m_LinkList.begin();
	for (auto SourIter = m_Nodes.begin(); SourIter != m_Nodes.end(); ++SourIter)
	{
		_uint Index = 0;
		for (auto DestIter = /*m_Nodes.begin()*/SourIter; DestIter != m_Nodes.end(); ++DestIter)
		{
			Index++;
			if (*SourIter == *DestIter)
				continue;
			std::pair<CNode*, CNode*> link = std::make_pair(*SourIter, *DestIter);
			//std::pair<CNode*, CNode*> link = std::make_pair((*SourIter), (*DestIter));
			//std::pair<CNode*, CNode*> Invlink = std::make_pair((*DestIter), (*SourIter));

			//std::list<std::pair<CNode*, CNode*>>::iterator LinkIter =
			//	find_if(LinkList.begin(), LinkList.end(),
			//		[&link, &Invlink](std::list<std::pair<CNode*, CNode*>>::value_type& Value)
			//		{
			//			if ((link.first == Value.first && link.second == Value.second)
			//				|| (Invlink.first == Value.first && Invlink.second == Value.second))
			//				return true;
			//			else return false;
			//		});

			//if (LinkList.end() == LinkIter)
			//{
			//	LinkList.push_back(link);
			//}
			m_LinkList.push_back(link);
		}
		//int a = 0;
		if (LinkCmpIter == m_LinkList.end())
		{
			LinkCmpIter = m_LinkList.begin();
		}
		Compare_Link(m_LinkList, LinkCmpIter);
		list<pair<CNode*, CNode*>>::reverse_iterator RIter = m_LinkList.rbegin();
		if(!m_LinkList.empty())
			RIter++;
		LinkCmpIter = RIter.base();
		//int b = 0;
	}
	Make_Dump(string("EndTimeLog"), string("End_VisiblityCheck"));

	//Save_Visiblity(L"../Bin/Data/GameSystem/TempVisibility.bin");

	for (auto& value : m_LinkList)
	{
		Add_Link(value.first, value.second);
	}

	return S_OK;
}
HRESULT CCellLayer::SetUp_Visibility_UseThread()
{
	//Blocked매시의 정점들을 통해 가시성 검사
//해당 정점들을 기준으로 노드 설정 및 이웃 설정
//A*테스트..

//정점을 연결하는 모든 순서쌍 생성
	Make_Dump(string("StartTimeLog"), string("Start_VisiblityCheck"));
	list<pair<CNode*, CNode*>> LinkList;
	for (auto SourIter = m_Nodes.begin(); SourIter != m_Nodes.end(); ++SourIter)
	{
		for (auto DestIter = /*m_Nodes.begin()*/SourIter; DestIter != m_Nodes.end(); ++DestIter)
		{
			if (*SourIter == *DestIter)
				continue;
			std::pair<CNode*, CNode*> link = std::make_pair(*SourIter, *DestIter);
			LinkList.push_back(link);
		}
	}


	_uint iUsableThreadCount = thread::hardware_concurrency();
	_uint LinkNums = ceil(_float(LinkList.size()) / iUsableThreadCount);
	
	vector<list<pair<CNode*, CNode*>>>SeperateLinkArr;
	//list<pair<CNode*, CNode*>> SeperateLink;
	SeperateLinkArr.push_back(list<pair<CNode*, CNode*>>());
	for (auto Link : LinkList)
	{
		list<pair<CNode*, CNode*>>& SaperateList = SeperateLinkArr.back();
		SaperateList.push_back(Link);
		if (SaperateList.size() >= LinkNums)
		{
			SeperateLinkArr.push_back(list<pair<CNode*, CNode*>>());
		}

		
	}


	for (_uint Index = 0; Index < _uint(SeperateLinkArr.size()); ++Index)
	{
		m_ThreadArr.push_back(
			make_shared<thread>(&CCellLayer::Func_Compare_UseThread, this, &SeperateLinkArr[Index])
		);
	}

	for (auto& Thread : m_ThreadArr)
	{
		if (Thread.get()->joinable())
			Thread.get()->join();
	}

	for (auto Link : SeperateLinkArr)
	{
		for (auto Pair : Link) 
		{
			m_LinkList.push_back(Pair);
		}


	}
	Make_Dump(string("EndTimeLog"), string("End_VisiblityCheck"));
	for (auto& value : m_LinkList)
	{
		Add_Link(value.first, value.second);
	}

	return S_OK;
}
void CCellLayer::Compare_Link(list<pair<CNode*, CNode*>>& LinkList, list<pair<CNode*, CNode*>>::iterator CmpIter)
{
	/*
	1. 같은 직선 이고, 이웃이 Blocked일때
	2. 같은 직선 위 이고, 직선이 다른 Block된 셀을 가로지를 때
	3. 이웃이 없을 때
*/
	list<pair<CNode*, CNode*>>::iterator LinkIter = CmpIter;//LinkList.begin();
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
		//모든 Blocked셀들로 검사
		for (auto& Cell : m_CellList[CELL_BLOCKED])
		{
			if (!Check_NearPoint(vLinkStart, vLinkEnd, Cell->Get_Position()))
				continue;

			//Blocked셀의 Line들을 검사
			std::list<_int> BlockLine;
			for (_uint i = 0; i < CCell::LINE_END; ++i)
			{
				//이웃 인덱스 가져오기
				CCell* pNeighborCell = Cell->Get_NeighborCell(CCell::LINE(i));
				std::pair<_float4, _float4> LinePoints = Cell->Get_LinePoints(CCell::LINE(i));
				_float2 v2CheckStart = _float2(LinePoints.first.x, LinePoints.first.z);
				_float2 v2CheckEnd = _float2(LinePoints.second.x, LinePoints.second.z);

				//이웃이 없을 때 --> 같은 선이면 제거
				if (nullptr == pNeighborCell)
				{
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}
					continue;
				}

				//Blocked된 이웃이 존재하는 선이 링크에 포함될 때
				if (pNeighborCell->IsBlocked())
				{
					//셀의 외각선이 가시성링크를 포함할 때
					if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
					{
						bRemove = true;
						break;
					}

				}
			}
			if (bRemove)
				break;

			//Blocked와 교차하는 선일 때
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
}
HRESULT CCellLayer::Make_Cells(_byte* ArrAttribute)
{
	CCell* pLeftTri = nullptr;
	CCell* pRightTri = nullptr;

	_float4 vHalfCenter = _float4(m_vCenterPosition.x * 0.5f, m_vCenterPosition.y * 0.5f, m_vCenterPosition.z * 0.5f, 1.f);
	//셀 생성
	for (_uint i = 0; i < m_iZNums; ++i)
	{
		for (_uint j = 0; j < m_iXNums; ++j)
		{
			_uint iIndex = ((i * m_iXNums) + j) * 2;

			_float3 vVertexPos[4] =
			{
				_float3(_float(j) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i) * m_fTileSize - vHalfCenter.z),//LB
				_float3(_float(j) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * m_fTileSize - vHalfCenter.z),//LT
				_float3(_float(j + 1) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i + 1) * m_fTileSize - vHalfCenter.z),//RT
				_float3(_float(j + 1) * m_fTileSize - vHalfCenter.x, m_fLayerHeightMin, _float(i) * m_fTileSize - vHalfCenter.z)//RB
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


			//Add_CellList(CELL_GROUND, pLeftTri);
			//Add_CellList(CELL_WALL, pLeftTri);
			//Add_CellList(CELL_GRASS, pLeftTri);
			//Add_CellList(CELL_STAIR, pLeftTri);
			//Add_CellList(CELL_BLOCKED, pLeftTri);

			//Add_CellList(CELL_GROUND, pRightTri);
			//Add_CellList(CELL_GRASS, pRightTri);
			//Add_CellList(CELL_WALL, pRightTri);
			//Add_CellList(CELL_STAIR, pRightTri);
			//Add_CellList(CELL_BLOCKED, pRightTri);


		}
	}
	SetUp_CellList();
	return S_OK;
}
HRESULT CCellLayer::SetUp_CellList()
{
	for (auto& Cell : m_Cells)
	{
		Add_CellList(CELL_GROUND, Cell);
		Add_CellList(CELL_WALL, Cell);
		Add_CellList(CELL_GRASS, Cell);
		Add_CellList(CELL_STAIR, Cell);
		Add_CellList(CELL_BLOCKED, Cell);
	}

	/*auto Iter = m_CellList.find(CELL_BLOCKED);
	if (Iter == m_CellList.end())
		return S_OK;
	for (auto CellIter = Iter->second.begin(); CellIter != Iter->second.end();)
	{
		_bool bRemove = true;
		for (_uint i = 0; i < CCell::LINE_END; ++i)
		{
			CCell* pNeighborCell = (*CellIter)->Get_NeighborCell(CCell::LINE(i));
			if (nullptr == pNeighborCell)
			{
				bRemove = false;
				break;
			}
			if (!pNeighborCell->Check_Attribute(CELL_BLOCKED)) {
				bRemove = false;
				break;
			}
		}
		if (bRemove)
			CellIter = Iter->second.erase(CellIter);
		else
			CellIter++;
	}
	*/
	return S_OK;
}
#ifdef _DEBUG
#ifdef DEBUGRENDER

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

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유요한 값이 담겨잇는 형태로 얻어오낟. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어오낟. */
	DEVICE_CONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		// *(((VTXINSTANCE*)SubResource.pData) + i)
		((VTXTRIINSTANCE*)SubResource.pData)[i].vPosition[0].y = m_fLayerHeightMin;
		((VTXTRIINSTANCE*)SubResource.pData)[i].vPosition[1].y = m_fLayerHeightMin;
		((VTXTRIINSTANCE*)SubResource.pData)[i].vPosition[2].y = m_fLayerHeightMin;
		((VTXTRIINSTANCE*)SubResource.pData)[i].vColor = Get_Color(m_Cells[i]);
	}

	DEVICE_CONTEXT->Unmap(m_pVBInstance.Get(), 0);

	CRender_Manager::Get_Instance()->Callback_DebugRender
		+= bind(&CCellLayer::DebugRendering, this);
}

void CCellLayer::DebugRendering()
{
	//쉐이더 비긴
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

	m_pDebugShader->Set_RawValue("g_ViewMatrix", &matView, sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_float4x4));
	m_pDebugShader->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4));
	m_pDebugShader->Begin(1);

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
		m_pVBInstance.Get()
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	DEVICE_CONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICE_CONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICE_CONTEXT->IASetPrimitiveTopology(m_eToplogy);

	DEVICE_CONTEXT->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	//for (auto& Cell : m_Cells)
	//{
	//	_float4 vColor = Get_Color(Cell);

	//	m_pDebugShader->Set_RawValue("g_ViewMatrix", &matView, sizeof(_float4x4));
	//	m_pDebugShader->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_float4x4));
	//	m_pDebugShader->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4));
	//	m_pDebugShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
	//	m_pDebugShader->Begin(0);
	//	Cell->DebugRendering();
	//}
}
_float4 CCellLayer::Get_Color(CCell* pCell)
{
	_float4 vColor;

	if (pCell->Check_Attribute(CELL_GROUND))
	{
		vColor = _float4(0.f, 1.f, 0.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_WALL))
	{
		vColor = _float4(0.f, 0.f, 1.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_GRASS))
	{
		vColor = _float4(1.f, 1.f, 0.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_STAIR))
	{
		vColor = _float4(1.f, 0.f, 1.f, 1.f);
	}
	else if (pCell->Check_Attribute(CELL_BLOCKED))
	{
		vColor = _float4(1.f, 0.f, 0.f, 1.f);
	}
	return vColor;
}
#endif
#endif 
HRESULT CCellLayer::SetUp_StairList(map<_float, CCellLayer*>& Layers)
{
	m_StairList.clear();
	for (auto Layer : Layers)
	{
		if (Layer.second == this)
			continue;

		auto StairIter = m_CellList.find(CELL_STAIR);
		list<CCell*> StairList;
		for (auto Cell : StairIter->second)
		{
			_int Index = Cell->Get_Index();
			CCell* pCell = Layer.second->Get_Cell(Index);
			if (pCell && pCell->Check_Attribute(CELL_STAIR))
				StairList.push_back(Cell);
		}

		m_StairList.emplace(Layer.first, StairList);
	}

	return S_OK;
}
// _DEBUG
_bool CCellLayer::Is_Access(_float LayerKey)
{
	auto Iter = m_LayerRoute.find(LayerKey);
	if (Iter == m_LayerRoute.end())
		return false;
	else return true;
}
list<_float> CCellLayer::Get_LayerRoute(_float LayerKey)
{
	auto Iter = m_LayerRoute.find(LayerKey);
	return Iter->second;
}
HRESULT CCellLayer::SetUp_LayerRoute(map<_float, CCellLayer*>& Layers)
{
	//나를 제외한 모든 레이어에 대해 검사
	list<_float>CircularTest;
	CircularTest.push_back(m_fLayerHeightMin);
	for (auto Layer : Layers)
	{
		if (Layer.second == this)
			continue;
		list<_float> LayerRoute;
		LayerRoute.push_back(m_fLayerHeightMin);
		
		if (Layer.second->Check_MovableLayer(Layer.first, Layers, LayerRoute, CircularTest))
		{
			m_LayerRoute.emplace(Layer.first, LayerRoute);
		}
	}

	return S_OK;
}
_bool CCellLayer::Check_Circular(list<_float>& CircularCheckList)
{
	for (auto Value : CircularCheckList)
	{
		if (Value == m_fLayerHeightMin)
			return true;
	}
	return false;
}
_bool CCellLayer::Check_MovableLayer(_float fTargetLayerKey, map<_float, CCellLayer*>& Layers, list<_float>& LayerRoute, list<_float>CircularCheckList)
{
	if (Check_Circular(CircularCheckList)) //순환하면 못찾은거 --> 순환여부는 함수 만들던가 하자
	{
		return false;
	}

	if (fTargetLayerKey == m_fLayerHeightMin)//순환하지 않고 도달하면 찾은거
	{
		LayerRoute.push_back(m_fLayerHeightMin);
		return true;
	}
	else
	{
		
		list<_float> thisCircular = CircularCheckList;
		thisCircular.push_back(m_fLayerHeightMin);
		thisCircular.sort();
		list<list<_float>> LayerListArr;
		for (auto StairList : m_StairList)
		{
			auto LayerIter = Layers.find(StairList.first);
			if (Layers.end() == LayerIter)
				assert(0);//못찾으면 사고임 서순 확인ㄱㄱ

			list<_float> LayerList;

			LayerList.push_back(m_fLayerHeightMin);
			if (LayerIter->second->Check_MovableLayer(fTargetLayerKey, Layers, LayerList, thisCircular))
			{
				LayerListArr.push_back(LayerList);
			}
			//else
			//{
			//	LayerRoute.clear();
			//	return false;
			//}
		}
		LayerListArr.sort([](auto& Sour, auto& Dest) {
			if (Sour.size() > Dest.size())
				return true;
			else return false;
			});
		list<_float> Route = LayerListArr.front();

		if (Route.back() != fTargetLayerKey)
			assert(0);//이론상 여기 걸리면 안됨 뭔가 큰 문제가 있는거

		LayerRoute.merge(Route);
		return true;
	}
}
_bool CCellLayer::Check_BlockedVisibility(_float2 v2LinkStart, _float2 v2LinkEnd, _float2 v2CheckStart, _float2 v2CheckEnd)
{
	//같은 선분 상에 존재할 때
	if (CUtility_Cell::Is_IncludeLine(v2LinkStart, v2LinkEnd, v2CheckStart)
		&& CUtility_Cell::Is_IncludeLine(v2LinkStart, v2LinkEnd, v2CheckEnd))
	{
		//두 점 공유
		//Blocked Link 같은 점을 공유할 때
		if (CUtility_Cell::Is_SamePoint(v2LinkStart, v2CheckStart) && CUtility_Cell::Is_SamePoint(v2LinkEnd, v2CheckEnd))
			return true;
		if (CUtility_Cell::Is_SamePoint(v2LinkStart, v2CheckEnd) && CUtility_Cell::Is_SamePoint(v2LinkEnd, v2CheckStart))
			return true;

		//Blocked Link 한 점을 공유
		//Start Start공유
		if (CUtility_Cell::Is_SamePoint(v2CheckStart, v2LinkStart))
		{
			//두 점 사이에 있으면 제거
			if (CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckEnd))
				return true;
			//밖에 있지만 같은 방향이면 제거
			if (CUtility_Cell::Is_SameDir(v2LinkStart, v2LinkEnd, v2CheckEnd))
				return true;

		}
		//End Start공유
		if (CUtility_Cell::Is_SamePoint(v2CheckEnd, v2LinkStart))
		{
			//두 점 사이에 있으면 제거
			if (CUtility_Cell::Is_BetweenLine(v2LinkStart, v2LinkEnd, v2CheckStart))
				return true;
			//밖에 있지만 같은 방향이면 제거
			if (CUtility_Cell::Is_SameDir(v2LinkStart, v2LinkEnd, v2CheckStart))
				return true;

		}
		//Start End공유
		if (CUtility_Cell::Is_SamePoint(v2CheckStart, v2LinkEnd))
		{
			//두 점 사이에 있으면 제거
			if (CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckEnd))
				return true;
			//밖에 있지만 같은 방향이면 제거
			if (CUtility_Cell::Is_SameDir(v2LinkEnd, v2LinkStart, v2CheckEnd))
				return true;

		}
		//End End공유
		if (CUtility_Cell::Is_SamePoint(v2CheckEnd, v2LinkEnd))
		{
			//두 점 사이에 있으면 제거
			if (CUtility_Cell::Is_BetweenLine(v2LinkEnd, v2LinkStart, v2CheckStart))
				return true;
			//밖에 있지만 같은 방향이면 제거
			if (CUtility_Cell::Is_SameDir(v2LinkEnd, v2LinkStart, v2CheckStart))
				return true;

		}//End공유

		//공유되는 점X
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

_bool CCellLayer::Check_NearPoint(_float4 vLineStart, _float4 vLineEnd, _float4 vPosition)
{
	_bool bNear = CUtility_Cell::Is_NearPoint(_float2(vLineStart.x, vLineStart.z),
		_float2(vLineEnd.x, vLineEnd.z),
		_float2(vPosition.x, vPosition.z),
		m_fTileSize * 3.f
	);

	return bNear;
}

CCellLayer::CellList CCellLayer::Get_BestRoute(CNode* pStartNode, CNode* pEndNode, _bool& bOutIsFind, list<_float4>& OutNodeList)
{
	if (nullptr == pStartNode || nullptr == pEndNode)
		return CellList();

	//여기 오래걸림
	//if(FAILED(Ready_SearchingChild()))
	//	return CellList();

	CellList BestCellList;

	NODES BestRoute;

	NODES OpenList;
	NODES CloseList;

	//pStartNode의 휴리스틱 비용 업데이트
	pStartNode->Set_HCost(pEndNode);

	//StartNode의 가시성 설정
	//여기 오래걸림
	Set_Visiblity(pStartNode);

	//EndNode의 가시성 설정
	Set_Visiblity(pEndNode);


	//Start-End의 가시성 검사및 연결
	_bool bRemove = false;
	_float4 vLinkStart = pStartNode->Get_Positon();
	_float4 vLinkEnd = pEndNode->Get_Positon();

	_float2 v2LinkStart = _float2(vLinkStart.x, vLinkStart.z);
	_float2 v2LinkEnd = _float2(vLinkEnd.x, vLinkEnd.z);
	//A. 연산량 주의 : 모든 Blocked셀을 확인함
	for (auto& Cell : m_CellList[CELL_BLOCKED])
	{
		//Blocked셀의 Line들을 검사
		for (_uint i = 0; i < CCell::LINE_END; ++i)
		{
			//이웃 인덱스 가져오기
			CCell* pNeighborCell = Cell->Get_NeighborCell(CCell::LINE(i));
			std::pair<_float4, _float4> LinePoints = Cell->Get_LinePoints(CCell::LINE(i));
			_float2 v2CheckStart = _float2(LinePoints.first.x, LinePoints.first.z);
			_float2 v2CheckEnd = _float2(LinePoints.second.x, LinePoints.second.z);

			//이웃이 없을 때 --> 같은 선이면 제거
			if (nullptr == pNeighborCell)
			{
				if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
				{
					bRemove = true;
					break;
				}
				continue;
			}

			//Blocked된 이웃이 존재하는 선이 링크에 포함될 때
			if (pNeighborCell->IsBlocked())
			{
				//셀의 외각선이 가시성링크를 포함할 때
				if (Check_BlockedVisibility(v2LinkStart, v2LinkEnd, v2CheckStart, v2CheckEnd))
				{
					bRemove = true;
					break;
				}

			}
		}
		//Blocked와 교차하는 선일 때
		if (Cell->Check_CrossLines(vLinkStart, vLinkEnd))
		{
			bRemove = true;
			break;
		}
	}

	if(!bRemove)
		Add_Link(pStartNode, pEndNode);

	OpenList.push_back(pStartNode);

	if (Bake_OpenCloseList(OpenList, CloseList, pEndNode))
	{
		//맨마지막의 노드로 부터 부모노드로 거슬러 올라가면서 경로 구축
		Bake_BestList(BestRoute, OpenList, CloseList, pEndNode);
		if (BestRoute.size() > 2)
			int a = 0;
		BestCellList = Make_CellList(BestRoute);
		for (auto Node : BestRoute)
		{
			OutNodeList.push_back(Node->Get_Positon());
		}
		Clear_Nodes();
		
	}//찾음
	else
	{
		BestRoute.clear();
		Clear_Nodes();
	}//못찾음
	for (auto& Child : pStartNode->Get_OriginChildList())
	{
		Child->Remove_Child(pStartNode);
	}
	pStartNode->Clear_ChildList();
	for (auto& Child : pEndNode->Get_OriginChildList())
	{
		Child->Remove_Child(pEndNode);
	}
	pEndNode->Clear_ChildList();
	return BestCellList;
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
		//if (Value == nullptr)
		//	return E_FAIL;
		Value->Ready_SearchingChild();
	}
	return S_OK;
}

void CCellLayer::Set_Visiblity(CNode* pNode)
{
	std::list<std::pair<CNode*, CNode*>> LinkList;
	_float fMinLength = 99999999.f;
	CNode* pNearNode = nullptr;
	//for (auto& Dest : m_Nodes)
	//{
	//	if (pNode == Dest)
	//		continue;

	//	_float DestLength = (pNode->Get_Positon() - Dest->Get_Positon()).Length();
	//	if (fMinLength > DestLength)
	//	{
	//		fMinLength = DestLength;
	//		pNearNode = Dest;
	//	}

	//	if (DestLength> 5)
	//	{
	//		std::pair<CNode*, CNode*> link = std::make_pair(pNode, Dest);
	//		LinkList.push_back(link);
	//	}

	//}

	if (LinkList.empty())
	{
		CNode* pNearNode = Get_NearNode(pNode->Get_Positon());
		std::pair<CNode*, CNode*> link = std::make_pair(pNode, pNearNode);
		LinkList.push_back(link);
	}

	//std::list<std::pair<CNode*, CNode*>>::iterator LinkIter = LinkList.begin();
	//Compare_Link(LinkList, LinkList.begin());
	
	for (auto& value : LinkList)
	{
		//Add_SearchLink(value.first, value.second);
		Add_Link(value.first, value.second);
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

CNode* CCellLayer::Get_Node(_float4 vPosition)
{
	auto Iter = find_if(m_Nodes.begin(), m_Nodes.end(), [&vPosition](auto& Value)
		{
			if (Value->Get_Positon().x == vPosition.x && Value->Get_Positon().z == vPosition.z)
				return true;
			else return false;
		});

	if (Iter == m_Nodes.end())
		return nullptr;
	else
		return *Iter;
}

CNode* CCellLayer::Get_NearNode(_float4 vPosition)
{
	CNode* pNearNode = nullptr;
	_float fMinLength = 9999999.f;
	for (auto& Node : m_Nodes)
	{
		_float NodeLength = (vPosition - Node->Get_Positon()).Length();
		if (fMinLength > NodeLength)
		{
			fMinLength = NodeLength;
			pNearNode = Node;
		}
	}

	return pNearNode;
}

void CCellLayer::Replace_Attribute(_byte OriginAttribute, _byte ReplaceAttribute)
{

	for (auto& CellList : m_CellList)
	{
		CellList.second.clear();
	}

	for (auto& Cell : m_Cells)
	{
		if (Cell->Check_Attribute(OriginAttribute))
		{
			Cell->Set_Flags(ReplaceAttribute);
		}
	}

	SetUp_CellList();


}

_bool CCellLayer::Bake_OpenCloseList(NODES& rhsOpenNodeList, NODES& rhsCloseNodeList, CNode* pEndNode)
{
	if (rhsOpenNodeList.empty())
		return false;

	//가장 싼 비용의 노드 가져오기
	CNode* pSelectNode = Pop_CheapestNode(rhsOpenNodeList);

	rhsCloseNodeList.push_back(pSelectNode);
	if (Check_EndNode(rhsOpenNodeList, rhsCloseNodeList, pSelectNode, pEndNode))
		return true;
	return Bake_OpenCloseList(rhsOpenNodeList, rhsCloseNodeList, pEndNode);
}

_bool CCellLayer::Check_EndNode(NODES& rhsOpenList, NODES& rhsCloseList, CNode* pSelectNode, CNode* pEndNode)
{
	// G : 시작점 부터 현재 타일까지의 소요비용 --> 현재 타일의 부모의 G비용에 본인 타일의 비용을 더한다
		// H : 현재 타일에서 목적지까지의 예상 소요비용 --> 
		// F : 총 비용

	_uint Select_GCost = pSelectNode->Get_TotalGCost();
	NODES& ChildList = pSelectNode->Get_OriginChildList();
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

		//자식노드의 검사가 이미 끝났다면..
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
			//현재 자식노드의 GCost와 기존에 들어있던 노드의 GCost비교 후 업데이트
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
	//여기서 CloseList의 마지막 요소 가져옴
	NODES ResultList;
	ResultList.push_back(pEnd);
	CNode* pNode = CloseNodeList.back();

	while (nullptr != pNode)
	{
		ResultList.push_back(pNode);
		pNode = pNode->Get_Parent();
	}
	//이 타이밍에 ResultList는 End부터 Start까지

	//거꾸로 할당
	OutBestList.assign(ResultList.rbegin(), ResultList.rend());
}

CCellLayer::CellList CCellLayer::Make_CellList(NODES& OutBestList)
{
	//두 노드 사이의 OpenCell들을 가져옴
	CellList BestList;
	CNode* pStartNode = nullptr;
	CNode* pEndNode = nullptr;
	auto NodeIter = OutBestList.begin();

	for (auto& Node : OutBestList)
	{
		//pEndNode = Node;
		//Find_NearCell(pStartNode, pEndNode, BestList);
		//pStartNode = pEndNode;
		Find_NearCell(Node, BestList);
	}
	_float4 vEndPos = OutBestList.back()->Get_Positon();
	BestList.sort([&vEndPos](auto Sour, auto Dest) {
		_float SourLength = (Sour->Get_Position() - vEndPos).Length();
		_float DestLength = (Dest->Get_Position() - vEndPos).Length();
		if (SourLength > DestLength)
			return true;
		else if (SourLength == DestLength)
		{
			if (Sour->Get_Index() > Dest->Get_Index())
				return true;
			else return false;
		}
		else return false;
		});
	BestList.unique();
	//if (BestList.empty())
	//	return;

	//auto CellIter = BestList.begin();
	//for (; CellIter != BestList.end();)
	//{
	//	if ((*CellIter)->Check_InCell(pStartNode->Get_Positon()))
	//	{
	//		CellIter = BestList.erase(++CellIter);
	//	}
	//	else if ((*CellIter)->Check_InCell(pEndNode->Get_Positon()))
	//	{
	//		CellIter = BestList.erase(++CellIter);
	//	}
	//	else
	//		++CellIter;
	//}

	//얘네를 return;
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
		if (Cell->Check_Attribute(CELL_BLOCKED))
			continue;

		//if (Cell->Check_CrossLines(vStartPosition, vEndPosition, true))//직선과 셀이 교차하는지 체크
		//{
		//	rhsCellList.push_back(Cell);
		//}

		if(Cell->Check_InCell(vStartPosition))// && Cell->Check_CrossLines(vStartPosition, vEndPosition, true))
			rhsCellList.push_back(Cell);
	}	
	for (auto& Cell : m_Cells)
	{
		if (Cell->Check_Attribute(CELL_BLOCKED))
			continue;


		if (Cell->Check_InCell(vEndPosition))// && Cell->Check_CrossLines(vStartPosition, vEndPosition, true))
			rhsCellList.push_back(Cell);
	}


}

void CCellLayer::Find_NearCell(CNode* pNode, CellList& rhsCellList)
{
	if (nullptr == pNode)
		return;

	_float4 vPosition = pNode->Get_Positon();

	CCell* pCell = Find_Cell(vPosition);
	if (pCell)
	{
		if (pCell->Check_Attribute(CELL_BLOCKED))
		{
			for (auto& Cell : m_Cells)
			{
				if (Cell->Check_Attribute(CELL_BLOCKED))
					continue;

				if (Cell->Check_InCell(vPosition))// && Cell->Check_CrossLines(vStartPosition, vEndPosition, true))
				{
					rhsCellList.push_back(Cell);
					break;
				}
			}
		}
		else
		{
			rhsCellList.push_back(pCell);
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
list<CCell*> CCellLayer::Find_Cell_InRange(_float4 vPosition, _float fRange)
{
	list<CCell*> Return;

	if (fRange <= 1.f)
	{
		CCell* pCell = Find_Cell(vPosition);
		Return.push_back(pCell);
		return Return;
	}

	_float4 vFindingPos = _float4(vPosition.x, vPosition.y, vPosition.z);
	_float4 vHalfCenter = _float4(m_vCenterPosition.x * 0.5f, 0.f, m_vCenterPosition.z * 0.5f, 1.f);
	vFindingPos += vHalfCenter;
	vFindingPos /= m_fTileSize;

	_int iHalfRange = roundf((fRange - 1.f) * 0.5f);
	_int iLimits[4] = { 0 };
	iLimits[0] = (_int(vFindingPos.x) - iHalfRange >= 0) ? _int(vFindingPos.x) - iHalfRange : 0;//L
	iLimits[1] = (_int(vFindingPos.z) + iHalfRange < m_iZNums) ? _int(vFindingPos.z) + iHalfRange : m_iZNums - 1;//T
	iLimits[2] = (_int(vFindingPos.x) + iHalfRange < m_iXNums) ? _int(vFindingPos.x) + iHalfRange : m_iXNums - 1;//R
	iLimits[3] = (_int(vFindingPos.z) - iHalfRange >= 0) ? _int(vFindingPos.z) - iHalfRange : 0;//B

	_int CenterIndex = ((_int(vFindingPos.z)) * m_iXNums + _int(vFindingPos.x));

	_int StartIndex = CenterIndex - iLimits[0] - (iLimits[3] * m_iXNums);
	_int EndIndex = CenterIndex + iLimits[2] + (iLimits[1] * m_iXNums);

	for (_uint i = StartIndex; i <= EndIndex; ++i)
	{
		_bool bLeft = false;
		_bool bRight = false;
		_bool bTop = false;
		_bool bBottom = false;
		if (i % m_iXNums < iLimits[0])
			bLeft = true;
		if (i / m_iXNums > iLimits[1])
			bTop = true;
		if (i % m_iXNums > iLimits[2])
			bRight = true;
		if (i / m_iXNums < iLimits[3])
			bBottom = true;
		if (!bLeft && !bTop && !bRight && !bBottom)
		{
			Return.push_back(m_Cells[i * 2]);
			Return.push_back(m_Cells[(i * 2) + 1]);
		}
	}

	return Return;
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
	//다음 탐색 필요 타일 선택 --> 경로채점 --> F = G + H
	//이후 가장 작은 크기의 FCost를 선택해 Openlist에 넣어줌

	//FCost기준 오름차순 정렬
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

void CCellLayer::Func_Compare_UseThread(list<pair<CNode*, CNode*>>* pList)
{
	Compare_Link((*pList), pList->begin());
}
