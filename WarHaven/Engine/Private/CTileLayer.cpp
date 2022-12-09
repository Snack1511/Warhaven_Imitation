#include "CTileLayer.h"

CTileLayer::CTileLayer()
{
}

CTileLayer::~CTileLayer()
{
	Release();
}

CTileLayer* CTileLayer::Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iLayerIndex)
{
	CTileLayer* pInstance = new CTileLayer;

	if (iNumTilesX == 0 || iNumTilesZ == 0 || fTileSize <= 0.f)
	{
		Call_MsgBox(L" InValid Variables in : CTileLayer ");
		SAFE_DELETE(pInstance);
		return pInstance;
	}

	pInstance->m_iNumTilesX = iNumTilesX;
	pInstance->m_iNumTilesZ = iNumTilesZ;
	pInstance->m_fTileSize = fTileSize;
	pInstance->m_iLayerIndex = iLayerIndex;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L" Failed to Initialize : CTileLayer ");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CTileLayer::Initialize()
{
	/* Ÿ�� ���� */
	
	_uint	m_iTotalTileNum = m_iNumTilesX * m_iNumTilesZ;
	_float4 vCenterPos = ZERO_VECTOR;
    _float fOffset = 0.f;//m_fTileSize * 0.5f;
	for (_uint i = 0; i < m_iNumTilesZ; ++i)
	{
		vCenterPos.z = (m_fTileSize * (_float)i ) + fOffset;

		for (_uint j = 0; j < m_iNumTilesX; ++j)
		{
			vCenterPos.x = (m_fTileSize * (_float)j) + fOffset;

			_uint iCurIndex = j + (i * m_iNumTilesX);
			CTile* pTile = CTile::Create(vCenterPos, iCurIndex, this);
			m_vecTiles.push_back(pTile);

		}
		
	}

    if (FAILED(SetUp_NeighvorTile()))
        assert(0);
	return S_OK;
}

HRESULT CTileLayer::SetUp_NeighvorTile()
{
    for (_int i = 0; i < m_iNumTilesZ; ++i)
    {
        for (_int j = 0; j < m_iNumTilesX; ++j)
        {
            _uint TileIndex = _uint((i * m_iNumTilesX) + j);

            _bool bTop = (i == (m_iNumTilesZ - 1));
            _bool bBottom = (i == 0);
            _bool bLeft = (j == 0);
            _bool bRight = (j == m_iNumTilesX - 1);

            CTile* pCurTile = m_vecTiles[TileIndex];
            CTile* pNeighvorTile = nullptr;

            if (nullptr == pCurTile)
                assert(0);

            //�»� --> N + width - 1
            if (bTop || bLeft)
                pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex + m_iNumTilesX - 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_LeftTop, pNeighvorTile);

            //�� -->  N + width
            if (bTop)
                pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex + m_iNumTilesX;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_Top, pNeighvorTile);

            //��� --> N + width + 1
            if (bTop || bRight)
                pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex + m_iNumTilesX + 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_RightTop, pNeighvorTile);

            //�� --> N + 1
            if (bRight)
                 pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex + 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_Right, pNeighvorTile);

            //���� --> N - width + 1
            if (bBottom || bRight)
                 pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex - m_iNumTilesX + 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_RightBottom, pNeighvorTile);

            //�� --> N - width
            if (bBottom)
                 pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex - m_iNumTilesX;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_Bottom, pNeighvorTile);

            //���� --> N - width -1
            if (bBottom || bLeft)
                pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex - m_iNumTilesX - 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_LeftBottom, pNeighvorTile);

            //�� --> N - 1
            if (bLeft)
                 pNeighvorTile = nullptr;
            else
            {
                _uint NeighborIndex = TileIndex - 1;
                pNeighvorTile = m_vecTiles[NeighborIndex];
            }
            pCurTile->Bake_Neighvor(CTile::eNeighvorFlags_Left, pNeighvorTile);

        }
    }

	return S_OK;
}

HRESULT CTileLayer::Render()
{


	return S_OK;
}

void CTileLayer::Release()
{
	for (auto& elem : m_vecTiles)
		SAFE_DELETE(elem);

	m_vecTiles.clear();
}

_bool CTileLayer::Get_Route_TileBase(_uint iCurrent_TileIdx, _uint iTarget_TileIdx, list<CTile*>& OutBestList)
{
    //�ִܰŸ� ����Ʈ
    list<CTile*> BestRoute;

    CTile* pStartTile = m_vecTiles[iCurrent_TileIdx];
    CTile* pEndTile = m_vecTiles[iTarget_TileIdx];
    NODE StartNode = make_tuple(pStartTile, nullptr, 0, 0, 0);
    //�θ��� ��������Ʈ�� ����
    //������ �� �ִ� ����Ÿ�� ����
    //�θ�Ÿ��(���� �� ���) ����Ʈ���� ����
    //�θ�Ÿ�� ��������Ʈ�� ����
    //���� ���� ����� Ÿ�� ���� �� ������Ͽ� ����
    //�ش� Ÿ���� ���� Ÿ�� ����
    
    //������� �� ������� �����
    NODELIST OpenList;
    NODELIST CloseList;
    OpenList.push_back(StartNode);
    if (Bake_OpenCloseList(OpenList, CloseList, pEndTile))
    {
        Bake_BestList(BestRoute, OpenList, CloseList, pStartTile, pEndTile);
        OutBestList.merge(BestRoute);
        return true;
    }//ã��
    else
    {
        return false;
    }//��ã��

}

_bool CTileLayer::Get_Route_NearStair(_uint iCurrent_TileIdx, _uint& OutStairIndex, list<CTile*>& OutBestList)
{
    OutStairIndex = 0;
    return true;
}

_bool CTileLayer::Bake_OpenCloseList(NODELIST& rhsOpenTileList, NODELIST& rhsCloseTileList, CTile* pEndTile)
{
    if (rhsOpenTileList.empty())
        return false;

    //���� �� ����� ��� ��������
    NODE SelectNode = Pop_CheapestNode(rhsOpenTileList);

    rhsCloseTileList.push_back(SelectNode);
    if (Check_EndTile(rhsOpenTileList, rhsCloseTileList, SelectNode, pEndTile))
        return true;
    return Bake_OpenCloseList(rhsOpenTileList, rhsCloseTileList, pEndTile);
}

void CTileLayer::Bake_BestList(list<CTile*>& OutBestList, NODELIST& rhsOpenTileList, NODELIST& rhsCloseTileList, CTile* pStart, CTile* pEnd)
{
    CTile* pSearchTile = pEnd;
    NODE SearchNode;
    list<CTile*> ResultRoute;
    _bool bFind = false;
    bFind = Pop_FindNode(rhsOpenTileList, pSearchTile, SearchNode);
    ResultRoute.push_back(get<Node_NodeTile>(SearchNode));
    while (bFind)
    { 
        pSearchTile = get<Node_ParentTile>(SearchNode);
        bFind = Pop_FindNode(rhsCloseTileList, pSearchTile, SearchNode);
        ResultRoute.push_back(get<Node_NodeTile>(SearchNode));
    }
    ResultRoute.pop_back();

    //list<CTile*>::iterator ListIter = ResultRoute.end();
    //--ListIter;
    //for (; ListIter != ResultRoute.begin(); --ListIter)
    //    OutBestList.push_back((*ListIter));
    //OutBestList.push_back((*ListIter));
    OutBestList.assign(ResultRoute.rbegin(), ResultRoute.rend());
}

CTileLayer::NODE CTileLayer::Pop_CheapestNode(NODELIST& rhsOpenTileList)
{
    //���� Ž�� �ʿ� Ÿ�� ���� --> ���ä�� --> F = G + H
    //���� ���� ���� ũ���� FCost�� ������ Openlist�� �־���

    //FCost���� �������� ����
    rhsOpenTileList.sort([](NODELIST::value_type& Sour, NODELIST::value_type& Dest) {
        if (get<Node_FCost>(Sour) > get<Node_FCost>(Dest))
            return true;
        else return false;
        });

    NODE TargetNode = rhsOpenTileList.front();
    rhsOpenTileList.pop_front();

    return TargetNode;
}

_bool CTileLayer::Check_EndTile(NODELIST& rhsOpenList, NODELIST& rhsCloseList, NODE& SelectNode, CTile* pEndTile)
{
    // G : ������ ���� ���� Ÿ�ϱ����� �ҿ��� --> ���� Ÿ���� �θ��� G��뿡 ���� Ÿ���� ����� ���Ѵ�
    // H : ���� Ÿ�Ͽ��� ������������ ���� �ҿ��� --> 
    // F : �� ���
    CTile* pSelectTile = get<Node_NodeTile>(SelectNode);
    _uint Select_GCost = get<Node_GCost>(SelectNode);
    CTile* pNeigvorTile = nullptr;
    for (_uint i = 0; i < CTile::eNeighvorFlags_END; ++i)
    {
        CTile* pNeigvorTile = pSelectTile->m_arrAdjTiles[i];
        if (nullptr == pNeigvorTile)
            continue;

        NODELIST::iterator CloseIter = find_if(rhsCloseList.begin(), rhsCloseList.end(), [&pNeigvorTile](NODELIST::value_type Value)
            {
                if (get<Node_NodeTile>(Value) == pNeigvorTile)
                    return true;
                else
                    return false;
            });

        //���� �̿�Ÿ���� �˻簡 �̹� �����ٸ�..
        if (rhsCloseList.end() != CloseIter)
            continue;

        _uint GCost = pNeigvorTile->Get_GCost() + Select_GCost;
        _uint HCost = Get_TileDistance(pNeigvorTile, pEndTile);
        _uint FCost = GCost + HCost;

        NODELIST::iterator OpenIter = find_if(rhsOpenList.begin(), rhsOpenList.end(), [&pNeigvorTile](NODELIST::value_type Value)
            {
                if (get<Node_NodeTile>(Value) == pNeigvorTile)
                    return true;
                else
                    return false;
            });

        if (rhsOpenList.end() == OpenIter)
        {
            NODE Node = make_tuple(pNeigvorTile, pSelectTile, GCost, HCost, FCost);
            rhsOpenList.push_back(Node);
        }
        else
        {
            if (GCost < get<Node_GCost>(*OpenIter))
            {
                get<Node_ParentTile>(*OpenIter) = pSelectTile;
                get<Node_GCost>(*OpenIter) = GCost;
                get<Node_FCost>(*OpenIter) = FCost;
            }
        }
        if (pNeigvorTile == pEndTile)
            return true;
    }
    return false;
}

_bool CTileLayer::Pop_FindNode(NODELIST& TileList, CTile* pSearchTile, NODE& OutTargetNode)
{
    NODELIST::iterator ListIter = find_if(TileList.begin(), TileList.end(), [&pSearchTile](NODELIST::value_type Value)
        {
            if (pSearchTile == get<0>(Value))
                return true;
            else return false;
        });

    if (ListIter == TileList.end())
        return false;
    else
    {
        OutTargetNode = (*ListIter);
        TileList.erase(ListIter);
        return true;
    }
}

_float CTileLayer::Get_TileDistance(CTile* pSourTile, CTile* pDestTile)
{

    _float2 SourPos = _float2(pSourTile->m_iIndex % m_iNumTilesX, pSourTile->m_iIndex / m_iNumTilesX);
    _float2 DestPos = _float2(pDestTile->m_iIndex % m_iNumTilesX, pDestTile->m_iIndex / m_iNumTilesX);

    _float2 DiffXY = _float2(DestPos.x - SourPos.x, DestPos.y - SourPos.y);
    _float Distance = sqrtf(pow(DiffXY.x, 2)+ pow(DiffXY.y, 2));

    return Distance;
}
