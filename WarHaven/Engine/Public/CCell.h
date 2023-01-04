#pragma once
#include "Engine_Defines.h"

#define CELL_BLOCKED	0x01// 0000 0000 0000 0001
#define CELL_WALL		0x02// 0000 0000 0000 0010
#define CELL_GROUND		0x04// 0000 0000 0000 0100
#define CELL_GRASS		0x08// 0000 0000 0000 1000
#define CELL_STAIR		0x10// 0000 0000 0001 0000

BEGIN(Engine)


class ENGINE_DLL CCell
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	typedef pair<_float4, _float4> LinePair;
public:
	CCell();
	virtual ~CCell();

public:
	static CCell* Create(const _float3* pPoints, _int iIndex);
	static CCell* Create(const _float3* pPoints, _int iIndex, _float fMinHeight);


public:
	_vector Get_Point(POINT ePoint)
	{
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_float4 Get_Point_Float4(POINT ePoint);

	_vector Get_Line(LINE eLine) {
		return XMLoadFloat3(&m_vLine[eLine]);
	}

	_int Get_Index() const {
		return m_iMyIndex;
	}

	_float4 Get_Position() { return m_vCenter; }
	_float4	Get_CellNormal() { return m_vCellNormal; }
	_float4	Get_CellRight() { return m_vCellRight; }
	_float	Get_CellAngle() { return m_fAngle; }

	void Set_Neighbor(LINE eLine, CCell* pNeighBorCell) {
		m_pNeighborCells[eLine] = pNeighBorCell;
	}

	void	Set_Flags(_byte bFlag) { m_bCellFlag = bFlag; }
	void	Add_Flags(_byte bFlag) { m_bCellFlag |= bFlag; }
	_bool	IsBlocked() { 

		if (m_bCellFlag & CELL_BLOCKED) 
			return true; 

		return false;
	}

	_bool	IsWall() {
		if (m_bCellFlag & CELL_WALL)
			return true;

		return false;
	}

	_bool	IsGrass() {
		if (m_bCellFlag & CELL_GRASS)
			return true;

		return false;
	}

	_bool	IsGround() {
		if (m_bCellFlag & CELL_GROUND)
			return true;

		return false;
	}

	CCell* Get_NeighborCell(LINE eLine) { return m_pNeighborCells[eLine]; }
	LinePair Get_LinePoints(LINE eLine);
	_float Get_LayerKey() { return m_fLayerKey; }

	_bool Check_Attribute(_uint iAttribute);
	_byte Get_Attribute() { return m_bCellFlag; }
	void Set_LayerKey(_float Height) { m_fLayerKey = Height; };
public:
	// CComponent을(를) 통해 상속됨
	HRESULT Initialize(const _float3* pPoints, _int iIndex, _float fMinHeight);
	void	Make_Matrix();
	_bool Compare_Points(_vector vSourPoint, _vector vDestPoint);
	_bool isIn(_vector vPosition, CCell** pOutNeighborCell, LINE* pOutLine);
	_bool Check_CrossLines(_float4 LineStart, _float4 LineEnd);

#ifdef _DEBUG
//public:
//	void DebugRendering();
//private:
//	HRESULT Create_VertexBuffer();
//	HRESULT Create_IndexBuffer();
//	HRESULT SetUp_Vertices();
//private:
//	D3D11_BUFFER_DESC				m_BufferDesc;
//	D3D11_SUBRESOURCE_DATA			m_SubResourceData;
//	ComPtr<ID3D11Buffer> m_pVB = nullptr;
//	ComPtr<ID3D11Buffer> m_pIB = nullptr;
//	_uint							m_iStride = 0;
//	_uint							m_iNumVertices = 0;
//	_uint							m_iNumPrimitive = 0;
//	_uint							m_iIndicesStride = 0;
//	_uint							m_iNumIndices = 0;
//	_uint							m_iNumVertexBuffers = 0;
//	DXGI_FORMAT						m_eIndexFormat;
//	D3D11_PRIMITIVE_TOPOLOGY		m_eToplogy;
#endif // _DEBUG

private:
	_uint						m_iMyIndex = 0;
	_float3						m_vPoints[POINT_END];
	_float3						m_vLine[LINE_END];
	_float3						m_vNormal[LINE_END];
	CCell*						m_pNeighborCells[LINE_END] = {nullptr, nullptr, nullptr};

	_byte						m_bCellFlag = 0;
	_float4						m_vCellNormal;
	_float4						m_vCellRight;
	_float						m_fAngle = 0.f;

	_float4						m_vCenter;

	_float						m_fLayerKey = 999999.f;
};

END