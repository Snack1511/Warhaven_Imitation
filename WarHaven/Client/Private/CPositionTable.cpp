#include "CTable.h"
#include "CPositionTable.h"

#include "UsefulHeaders.h"
#include "CUtility_File.h"

CPositionTable::CPositionTable()
{
}

CPositionTable::~CPositionTable()
{
	Release();
}

CPositionTable* CPositionTable::Create()
{
	CPositionTable* pTable = new CPositionTable();

	if (FAILED(pTable->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CPositionTable");
		SAFE_DELETE(pTable);
	}

	return pTable;
}

HRESULT CPositionTable::Initialize()
{
	return S_OK;
}

void CPositionTable::Release()
{
}

_float4 CPositionTable::Find_Position(string strPositionKey)
{
	auto iter = m_mapPosition.find(Convert_ToHash(strPositionKey));

	if (iter == m_mapPosition.end())
		return ZERO_VECTOR;

	return iter->second;
}

HRESULT CPositionTable::Load_Position(string strFileKey)
{
	string savePath;
	savePath = "../bin/Data/GameSystem/Positions/";
	savePath += strFileKey;
	savePath += ".bin";


	ifstream	readFile(savePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"로드 경로 없음");
		return E_FAIL;
	}

	_uint iNumPositions = 0;

	readFile.read((char*)&iNumPositions, sizeof(_uint));

	for (_uint i = 0; i < iNumPositions; ++i)
	{
		string strTemp;
		_float4 vTemp;

		strTemp = CUtility_File::Read_Text(&readFile);
		readFile.read((char*)&vTemp, sizeof(_float4));

		Add_Position(strTemp, vTemp);
	}

	readFile.close();
}

void CPositionTable::Add_Position(string strPositionKey, _float4 vPosition)
{
	if (!Find_Position(strPositionKey).Is_Zero())
		assert(0);

	m_mapPosition.emplace(Convert_ToHash(strPositionKey), vPosition);
}
