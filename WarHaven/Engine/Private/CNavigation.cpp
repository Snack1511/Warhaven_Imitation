#include "CNavigation.h"

#include "CCell.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "CNode.h"
#include "CCellLayer.h"

CNavigation::CNavigation(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{
}

CNavigation::~CNavigation()
{
	SAFE_DELETE(m_pStartNode);
	SAFE_DELETE(m_pEndNode);
}

CNavigation* CNavigation::Create(_uint iGroupIdx, CCell* pStartCell, CPhysics* pPhysicsCom)
{
	CNavigation* pInstance = new CNavigation(iGroupIdx);

	pInstance->m_pCurCell = pStartCell;
	pInstance->m_pPhysicsCom = pPhysicsCom;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CNavigation");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

_bool CNavigation::Is_InWall()
{
	return m_pCurCell->IsWall();
}

_float4x4 CNavigation::Get_CurWallMat()
{
	_float4 vMyUp = _float4(0.f, 1.f, 0.f, 0.f);

	_float4 vCellNormal;
	if (m_bExamArena)
	{
		vCellNormal = (m_vExamArenaCenter - m_pOwner->Get_Transform()->Get_World(WORLD_POS));
		vCellNormal.y = 0.f;
		vCellNormal.Normalize();
	}
	else
		vCellNormal = m_pCurCell->Get_CellNormal();

	_float4 vTurnAxis = vMyUp.Cross(vCellNormal);
	vTurnAxis.Normalize();
	_float fTurnTheta = acosf(vMyUp.Dot(vCellNormal));

	_float4x4 matRot;
	if (vTurnAxis.Is_Zero())
		matRot.Identity();
	else
		matRot = XMMatrixRotationAxis(vTurnAxis.XMLoad(), fTurnTheta);

	return matRot;
}

_float4 CNavigation::Get_CurWallNormal()
{
	if (!m_pCurWallCell)
		return _float4(0.f, 0.f, 1.f, 1.f);

	return m_pCurWallCell->Get_CellNormal();
}

void CNavigation::Set_StartPosition(_float4 vPosition)
{
	if (m_pStartNode)
		m_pStartNode->Set_NodePosition(vPosition);
}

void CNavigation::Set_EndPosition(_float4 vPosition)
{
	if (m_pEndNode)
		m_pEndNode->Set_NodePosition(vPosition);
}

//골리스트 만들고
//해당리스트가 빌때까지 길찾기 후 셀 위치 넣기
//A. 연산량 주의
list<pair<_float4, CCellLayer*>> CNavigation::Get_Goals(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd, CCellLayer* pStartLayer)
{
	//시작 위치의 셀레이어부터
	//도착 위치의 셀레이어까지
	//각 점의 최단거리의 계단 셀을 도착지리스트에 넣음       
	list<pair<_float4, CCellLayer*>> GoalList;
	//CCellLayer* pStartLayer = nullptr;
	CCellLayer* pEndLayer = nullptr;
	for (auto& Layer : Layers)
	{
		_float vLayerKey = Layer.first;
		_float4 vPosition = _float4(vEnd.x, vLayerKey, vEnd.z);
		if (vLayerKey <= vEnd.y)
		{
			CCell* pCell = Layer.second->Find_Cell(vPosition);
			if (!pCell)//이런게 존재하면 안됨..
				assert(0);
			//현재셀이 Blocked가 아니거나
			if (!pCell->Check_Attribute(CELL_BLOCKED)) 
			{
				pEndLayer = Layer.second;
			}
			else 
			{
				//바로 이웃하는 셀이 Open일때
				for (_int i = 0; i < CCell::LINE_END; ++i)
				{
					CCell* pNeighborCell = pCell->Get_NeighborCell(CCell::LINE(i));
					if(pNeighborCell&& !pNeighborCell->Check_Attribute(CELL_BLOCKED))
						pEndLayer = Layer.second;

				}
			}
		}
	}

	//못찾았으면 높은대서부터 비교..
	auto InvIter = Layers.rbegin();
	for (; InvIter != Layers.rend(); ++InvIter)
	{
		_float Key = InvIter->first;
		if (nullptr == pEndLayer)
		{
			if (Key > vEnd.y)
			{
				pEndLayer = InvIter->second;
			}
		}
		
	}

	//이론상 무조건 하나의 레이어를 가지고 있어야 함
	if (nullptr == pEndLayer)
		assert(0);

	list<CCell*>OutEndCellList;
	_float4 vEndPos = vEnd;
	vEndPos.y = pEndLayer->Get_MinHeight();
	pEndLayer->Find_NearOpenCell(vEnd, OutEndCellList);

	if (OutEndCellList.empty())
		assert(0);

	OutEndCellList.sort([&vEndPos](auto& Sour, auto& Dest)
		{
			_float SourLength = (Sour->Get_Position() - vEndPos).Length();
			_float DestLength = (Dest->Get_Position() - vEndPos).Length();
			if (SourLength < DestLength)
				return true;
			else return false;
		});
	vEndPos = OutEndCellList.front()->Get_Position();

	if (pStartLayer == pEndLayer)
	{
		GoalList.push_back(make_pair(vEndPos, pEndLayer));
		return GoalList;
	}

	CCellLayer* pCurLayer = pStartLayer;
	CCellLayer* pNextLayer = pEndLayer;
	//현재 레이어에서 목표 레이어까지의 거쳐야 할 레이어 정보가 존재하면 들어감
	if (pCurLayer->Is_Access(pNextLayer->Get_MinHeight()))
	{
		//현재 레이어에서 목표 레이어까지의 거쳐야 할 레이어 정보
		list<_float> LayerRoute = pCurLayer->Get_LayerRoute(pNextLayer->Get_MinHeight());
		_float PrevKey = pStartLayer->Get_MinHeight();
		_float4 vStartPos = vStart;
		for (auto Key : LayerRoute)
		{
			if (Key == PrevKey)
				continue;
			auto PrevLayerIter = Layers.find(PrevKey);

			if (Layers.end() == PrevLayerIter)
				continue;

			//현재 레이어와 연결된 이전 레이어의 Stair키
			list<CCell*> StairList = PrevLayerIter->second->Get_StairCellList(Key);
			if (StairList.empty())
				continue;

			StairList.sort([&vStartPos](auto Sour, auto Dest) {
				_float SourLength = (vStartPos - Sour->Get_Position()).Length();
				_float DestLength = (vStartPos - Dest->Get_Position()).Length();
				if (SourLength < DestLength)
					return true;
				else return false;
				});

			//현재 레이어의 Stairlist --> 이전레이어에도 같은 인덱스의 셀 존재
			_float4 vGoal = StairList.front()->Get_Position();
			GoalList.push_back(make_pair(vGoal, PrevLayerIter->second));

			PrevKey = Key;
			vGoal.y = Key;
			vStartPos = vGoal;
		}
		//_float4 vEndPos = pEndLayer->Find_NearOpenCell(vEnd)->Get_Position();
		GoalList.push_back(make_pair(vEndPos, pEndLayer));
	}

	return GoalList;

	//_float pCurLayerKey = pStartLayer->Get_MinHeight();
	//_bool bUnderToOver = (pStartLayer->Get_MinHeight() < pEndLayer->Get_MinHeight());
	//auto CmpIter = Layers.find(pCurLayerKey);
	//_float4 vStartPos = _float4(vStart.x, pCurLayerKey, vStart.z, 1.f);
	//for (; CmpIter != Layers.end(); ++CmpIter)
	//{
	//	if (CmpIter->second == pEndLayer)
	//		break;

	//	vStartPos.y = CmpIter->first;
	//	//두 셀레이어의 Stair셀을 비교할 때 공유되는 지점이 있으면 두 레이어는 해당 계단으로 연결됨
	//	list<CCell*> StairCellList;
	//	for (auto Iter = CmpIter; Iter != Layers.end(); ++Iter)
	//	{
	//		//다른 레이어들중..CmpIter와 연결된 모든 리스트 가져오기
	//		list<CCell*> StairList = Iter->second->Get_StairCellList(CmpIter->first);
	//		//기존 리스트에 병합
	//		StairCellList.merge(StairList);
	//	}
	//	if (!StairCellList.empty()) 
	//	{
	//		//시작위치 가까운 순으로 정렬
	//		StairCellList.sort([&vStartPos](auto& sour, auto& Dest)
	//			{
	//				_float SourLen = (sour->Get_Position() - vStartPos).Length();
	//				_float DestLen = (Dest->Get_Position() - vStartPos).Length();
	//				if (SourLen > DestLen)
	//					return true;
	//				else return false;
	//			});
	//		//가장 가까운 위치 삽입
	//		GoalList.push_back(make_pair((*StairCellList.begin())->Get_Position(), CmpIter->second));
	//	}
	//	if (GoalList.empty())
	//		break;
	//	//CmpIter와 vStartPos변경
	//	vStartPos = GoalList.back().first;
	//}
 //
	//GoalList.push_back(make_pair(vEnd, pEndLayer));
	return GoalList;
}

/*

//시작 위치의 셀레이어부터
	//도착 위치의 셀레이어까지
	//각 점의 최단거리의 계단 셀을 도착지리스트에 넣음
	list<pair<_float4, CCellLayer*>> GoalList;
	CCellLayer* pStartLayer = nullptr;
	CCellLayer* pEndLayer = nullptr;
	for (auto& Layer : Layers)
	{
		_float vLayerKey = Layer.first;

		if (vLayerKey <= vStart.y)
		{
			pStartLayer = Layer.second;
		}
		if (vLayerKey <= vEnd.y)
		{
			pEndLayer = Layer.second;
		}
	}

	if (pStartLayer == nullptr || pEndLayer == nullptr)
		return GoalList;

	if (pStartLayer == pEndLayer)
	{
		GoalList.push_back(make_pair(vEnd, pEndLayer));
		return GoalList;
	}

	_float pCurLayerKey = pStartLayer->Get_MinHeight();
	auto CmpIter = Layers.find(pCurLayerKey);
	_float4 vStartPos = _float4(vStart.x, pCurLayerKey, vStart.z, 1.f);

	
	* EndLayer부터 시작 가장 처음 레이어까지 반복
	* CmpLayer = EndLayer--
	* EndLayer의 Stair --> 얘가 공유되면 연결된 레이어
	*		Stair가져와서 해당 셀의 인덱스와 일치하는 인덱스가 계단인 레이어와 위치 기억
	*		기억된 모든 위치를 CmpPosition과 비교해 짧은순으로 정렬
	*		가장 가까운 위치의 레이어 가져오기
				--> StartLayer에 도달할 때까지 반복
	* 연결된 레이어가 없다면? --> 어떤 레이어와도 연결되지 않은 레이어
	
_float4 vTargetPosition = vEnd;
auto EndLayerIter = Layers.find(pEndLayer->Get_MinHeight());
if (EndLayerIter == Layers.end() || EndLayerIter == Layers.begin())
assert(0);
//일단 무식하게 찾아
for (auto TargetLayerIter = EndLayerIter; TargetLayerIter->second != pStartLayer;)
{
	//계단 리스트 가져와
	list<pair<_float4, CCellLayer*>> LayerList;
	list<CCell*> StairList = TargetLayerIter->second->Get_StairCellList();
	//리스트 돌면서
	for (auto Cell : StairList)
	{
		_int CellIndex = Cell->Get_Index();
		//모든 셀레이어들을 검사해
		for (auto Layer : Layers)//n
		{
			//같은 레이어냐?
			if (TargetLayerIter->second == Layer.second)
			{
				//제껴
				continue;
			}
			CCell* pCell = Layer.second->Get_Cell(CellIndex);
			//연결되 있냐?
			if (nullptr != pCell && pCell->Check_Attribute(CELL_STAIR))
			{
				_float4 vCellPosition = pCell->Get_Position();
				//그럼 기억해
				//근대 시작레이어랑 연결되있냐?
				if (Layer.second == pStartLayer)
				{
					//그럼 이때까지 기록한거 다 날리고 이거만 기억하고 나가
					LayerList.clear();
					LayerList.push_back(make_pair(vCellPosition, Layer.second));
					break;
				}
				LayerList.push_back(make_pair(vCellPosition, Layer.second));

			}
		}
	}

	//vTargetPosition와의 거리기준으로 정렬해
	LayerList.sort([&vTargetPosition](auto Sour, auto Dest)
		{
			Sour.first.y = vTargetPosition.y;
			Dest.first.y = vTargetPosition.y;
			_float SourLength = (vTargetPosition - Sour.first).Length();
			_float DestLength = (vTargetPosition - Dest.first).Length();
			if (SourLength > DestLength)
				return true;
			else return false;
		});

	//front따로 저장해
	GoalList.push_back(LayerList.front());

	//만약 StartLayer가 맞으면
	if (LayerList.front().second == pStartLayer)
	{
		//나가
		break;

	}
	else//아니라면
	{
		//기준 값 업데이트 하고 반복해
		vTargetPosition = LayerList.front().first;
		TargetLayerIter = Layers.find(LayerList.front().second->Get_MinHeight());
	}
}

//거꾸로 되있는거 돌려
GoalList.reverse();

return GoalList;

*/

//A. 연산량 주의

int CNavigation::Func_MakeRoute(list<_float4>* NodeList, map<_float, CCellLayer*>* Layers, _float4 vStart, _float4 vEnd, CNavigation* pNaviCom)
{
	/* Locked 체크 */
	pNaviCom->m_bThreadOn = true;

	while (1)
	{
		_bool bBreak = false;

		for (auto& elem : *Layers)
		{
			if (elem.second->Is_Locked())
				break;

			bBreak = true;

		}

		if (bBreak)
			break;
	}
	
	for (auto& elem : *Layers)
		elem.second->Lock();

	/* 길찾기 */
	*NodeList = pNaviCom->Get_BestRoute(*Layers, vStart, vEnd);

	for (auto& elem : *Layers)
		elem.second->UnLock();

	pNaviCom->m_bThreadOn = false;

	return 0;

	/*list<pair<_float4, CCellLayer*>> GoalList = CNavigation::Get_Goals(*Layers, vStart, vEnd);
	CCellLayer::CellList Routes;

	pNaviCom->m_pStartNode->Set_NodePosition(vStart);

	for (auto value : GoalList)
	{
		if (nullptr == value.second)
			continue;
		pNaviCom->m_pEndNode->Set_NodePosition(value.first);

		_bool bFind = false;
		list<_float4> listTemp;
		CCellLayer::CellList List = value.second->Get_BestRoute(pNaviCom->m_pStartNode, pNaviCom->m_pEndNode, bFind, listTemp);

		for (auto Cell : List)
			Routes.push_back(Cell);

		pNaviCom->m_pStartNode->Clear_Node();
		pNaviCom->m_pEndNode->Clear_Node();

		pNaviCom->m_pStartNode->Set_NodePosition(value.first);
	}



	list<_float4> Return;
	if (!Routes.empty())
	{
		Routes.pop_front();
		if (!Routes.empty())
			Routes.pop_back();

		NodeList->push_back(vStart);
		for (auto Cell : Routes)
		{
			NodeList->push_back(Cell->Get_Position());
		}
		NodeList->push_back(vEnd);
	}*/
}

void CNavigation::Make_Route(list<_float4>* NodeList, map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd)
{
	if (m_bThreadOn)
		return;

	std::future<int> result = std::async(bind(Func_MakeRoute, NodeList, &Layers, vStart, vEnd, this));

	//*NodeList = Get_BestRoute(Layers, vStart, vEnd);
}

list<_float4> CNavigation::Get_BestRoute(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd)
{
	m_DebugRouteNode.clear();
	CCellLayer* pStartCellLayer = nullptr;

	//근처의 열린셀을 기준으로 길찾기
	CCell* pStartCell = Get_NearOpenCell(vStart, Layers, &pStartCellLayer);

	if (nullptr == pStartCellLayer)
		assert(0);//타일 모양 한번 확인해봐야함

	if (pStartCell->Check_Attribute(CELL_BLOCKED))
		assert(0);

	_float4 vStartPos = pStartCell->Get_Position();
	//GoalList의 목적지들은 모두 열린셀의 위치
	list<pair<_float4, CCellLayer*>> GoalList = Get_Goals(Layers, vStartPos, vEnd, pStartCellLayer);
	
	GoalList.push_front(make_pair(vStartPos, pStartCellLayer));

	CCellLayer::CellList Routes;

	m_pStartNode->Set_NodePosition(vStart);
	Routes.push_back(pStartCellLayer->Find_Cell(vStart));
	for (auto value : GoalList)
	{
		if (nullptr == value.second)
			continue;
		m_pEndNode->Set_NodePosition(value.first);

		_bool bFind = false;
		CCellLayer::CellList List = value.second->Get_BestRoute(m_pStartNode, m_pEndNode, bFind, m_DebugRouteNode);

		for (auto Cell : List)
			Routes.push_back(Cell);

		m_pStartNode->Clear_Node();
		m_pEndNode->Clear_Node();

		m_pStartNode->Set_NodePosition(value.first);
	}
	list<_float4> Return;
	//실제 시작위치 넣기

	//if (!Routes.empty()) 
	//{
	//	//중복되는 애들을 없애기 위해
	//	//앞에꺼 제거
	//	//Routes.pop_front();
	//	//뒤에꺼 제거 --> 는 지우지마
	//	//if(!Routes.empty())
	//	//	Routes.pop_back();

	//	//열린 시작셀 넣기
	//	Return.push_back(vStartPos);
	//	for (auto Cell : Routes)
	//	{
	//		Return.push_back(Cell->Get_Position());
	//	}
	//	Return.push_back(vEnd);
	//}
	
	//시작위치와 끝 위치 근처의 열린 셀을 이음
	for (auto Cell : Routes)
	{
		Return.push_back(Cell->Get_Position());
	}

	return Return;
}

CCell* CNavigation::Get_CurCell(_float4 vPosition, map<_float, CCellLayer*>& Layers, CCellLayer** ppOutInCellLayer)
{
	CCell* pReturn = nullptr;
	CCellLayer* pCellLayer = nullptr;

	for (auto& Layer : Layers)
	{
		_float vLayerKey = Layer.first;
		CCell* pCell = Layer.second->Find_Cell(vPosition);
		if (vLayerKey <= vPosition.y)
		{
			pCellLayer = Layer.second;
			pReturn = pCell;
		}

	}

	return pReturn;
}

CCell* CNavigation::Get_NearOpenCell(_float4 vPosition, map<_float, CCellLayer*>& Layers, CCellLayer** ppOutInCellLayer)
{
	CCell* pReturn = nullptr;
	CCellLayer* pCellLayer = nullptr;

	for (auto& Layer : Layers)
	{
		_float vLayerKey = Layer.first;
		CCell* pCell = Layer.second->Find_Cell(vPosition);
		if (vLayerKey <= vPosition.y)
		{
			if (!pCell->Check_Attribute(CELL_BLOCKED))
			{
				pCellLayer = Layer.second;
				pReturn = pCell;
			}
			else
			{
				for (_uint i = 0; i < CCell::LINE_END; ++i)
				{
					CCell* pNeighborCell = pCell->Get_NeighborCell(CCell::LINE(i));
					if (pNeighborCell)
					{
						if (!pNeighborCell->Check_Attribute(CELL_BLOCKED)
							&&!pNeighborCell->Check_Attribute(CELL_STAIR))
						{
							pCellLayer = Layer.second;
							pReturn = pNeighborCell;
							break;
						}
					}
				}
			}
		}

	}

	//pReturn이 NULL이면 타일 모양이 이상한거..
	if (ppOutInCellLayer)
		(*ppOutInCellLayer) = pCellLayer;
	return pReturn;
}

//CNavigation::CELL_TYPE CNavigation::isMove(_vector vPosition, _float4* pOutPos)
//{
//	if (!m_pCurCell)
//		return CNavigation::CELL_END;
//
//	/* m_NaviDesc.m_iCurrentIndex : 현재 객체가 존재하는 쎌의 인덱스. */
//	CCell* pNeighborCell = nullptr;
//	CCell::LINE pOutLine = CCell::LINE_END;
//
//	/*1. 현재 존재하는 셀 안에서만 움직였을때  */
//	if (true == m_pCurCell->isIn(vPosition, &pNeighborCell, &pOutLine))
//	{
//		if (m_pCurCell->IsWall())
//		{
//			*pOutPos = Get_NewPosFromWall(m_pCurCell, pOutLine, vPosition);
//			return WALL;
//		}
//		
//		*pOutPos = Get_NewPosFromCell(m_pCurCell, vPosition);
//		return DEFAULT;
//
//	}
//
//	/* 현재 존재하고 있는 쎌 바깥으로 나갔다. */
//	else
//	{
//		//이웃이 없을 때
//		if (!pNeighborCell)
//		{
//			*pOutPos = Get_NewDirFromCellLine(m_pCurCell, pOutLine, vPosition);
//			return BLOCKED;
//		}
//
//
//
//		/*2. 나간쪽 쎌에 이웃이 존재할때 */
//
//		//벽인지 부터 체크
//		if (pNeighborCell->IsWall())
//		{
//			//벽에 닿은건지, 벽을 넘어간건지 알아야함
//			for (_uint i = 0; i < 3; ++i)
//			{
//				//하나라도 내 점 y가 포인트 y보다 작다면 : 박은거임
//				if (XMVectorGetY(vPosition) < XMVectorGetY(pNeighborCell->Get_Point((CCell::POINT)i)))
//				{
//					*pOutPos = Get_NewPosFromWall(pNeighborCell, pOutLine, vPosition);
//					m_pCurCell = pNeighborCell;
//					return WALL;
//				}
//			}
//		}
//
//		//아니면 벽이었어도 벽 위로 넘은거고
//		while (1)
//		{
//			//나간쪽 셀에 들어있는지 체크
//			if (true == pNeighborCell->isIn(vPosition, &pNeighborCell, &pOutLine))
//				break;
//
//			if (!pNeighborCell)
//				return CELL_END;
//
//		}
//
//		// 막힌곳인지 체크
//		if (pNeighborCell->IsBlocked())
//		{
//			*pOutPos = Get_NewDirFromCellLine(m_pCurCell, pOutLine, vPosition, );
//			return BLOCKED;
//		}
//
//		//안막힌 곳이면
//
//		m_pCurCell = pNeighborCell;
//
//		*pOutPos = Get_NewPosFromCell(m_pCurCell, vPosition);
//		return DEFAULT;
//	}
//
//}

_float4 CNavigation::Enter_Wall()
{
	//m_fExamDistance = (m_pOwner->Get_Transform()->Get_World(WORLD_POS) - m_vExamArenaCenter).Length();

	//if (m_bOnWall)
	{
		CTransform* pTransform = m_pOwner->Get_Transform();
		_float4 vOriginPos = pTransform->Get_World(WORLD_POS);
		_float4 vNewDir = m_pPhysicsCom->Get_Physics().vDir;
		vNewDir.Normalize();
		_float4 vReturnPos = vOriginPos + vNewDir * m_pPhysicsCom->Get_Physics().fSpeed * fDT(0);


		m_pPhysicsCom->Get_Physics().bAir = false;

		_float4 vMyUp = m_pOwner->Get_Transform()->Get_World(WORLD_UP);
		_float4 vCellNormal;
		if (m_bExamArena)
		{
			vCellNormal = (m_vExamArenaCenter - vOriginPos);
			vCellNormal.y = 0.f;
			vCellNormal.Normalize();
		}
		else
			vCellNormal = m_pCurCell->Get_CellNormal();

		_float4 vTurnAxis = vMyUp.Cross(vCellNormal);
		vTurnAxis.Normalize();
		_float fTurnTheta = acosf(vMyUp.Dot(vCellNormal));

		if (!m_bOnWall)
		{
			vReturnPos.y += 0.5f;
		}
		m_bOnWall = true;



		if (vTurnAxis.Is_Zero())
			return vReturnPos;

		_float4x4 matRot;

		matRot = XMMatrixRotationAxis(vTurnAxis.XMLoad(), fTurnTheta);

		_float4		vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
		_float4		vUp = pTransform->Get_MyWorld(WORLD_UP).Normalize();
		_float4		vLook = pTransform->Get_MyWorld(WORLD_LOOK).Normalize();

		vRight = vRight.MultiplyNormal(matRot);
		vUp = vUp.MultiplyNormal(matRot);
		vLook = vLook.MultiplyNormal(matRot);

		pTransform->Set_World(WORLD_LOOK, vLook.Normalize());

		vUp = vCellNormal;
 		pTransform->Set_World(WORLD_UP, vUp.Normalize());
		vRight = vUp.Cross(vLook);
		pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());


		return vReturnPos;
	}

	CTransform* pTransform = m_pOwner->Get_Transform();
	_float4 vOriginPos = pTransform->Get_World(WORLD_POS);
	_float4 vNewDir = m_pPhysicsCom->Get_Physics().vDir;
	vNewDir.Normalize();
	_float4 vReturnPos = vOriginPos + vNewDir * m_pPhysicsCom->Get_Physics().fSpeed * fDT(0);


	m_pPhysicsCom->Get_Physics().bAir = false;
	m_bOnWall = true;

	_float4 vMyUp = m_pOwner->Get_Transform()->Get_World(WORLD_UP);
	_float4 vCellNormal;
	if (m_bExamArena)
	{
		vCellNormal = (m_vExamArenaCenter - vOriginPos);
		vCellNormal.y = 0.f;
		vCellNormal.Normalize();
	}
	else
		vCellNormal = m_pCurCell->Get_CellNormal();

	_float4 vTurnAxis = vMyUp.Cross(vCellNormal);
	vTurnAxis.Normalize();
	_float fTurnTheta = acosf(vMyUp.Dot(vCellNormal));

	if (vTurnAxis.Is_Zero())
		return vReturnPos;

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vTurnAxis.XMLoad(), fTurnTheta);



	_float4		vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
	_float4		vUp = pTransform->Get_MyWorld(WORLD_UP).Normalize();
	_float4		vLook = pTransform->Get_MyWorld(WORLD_LOOK).Normalize();

	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);
	vLook = vLook.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());

	//새로 위치 구하기
	vNewDir = vNewDir.MultiplyNormal(matRot);
	vReturnPos = vOriginPos + vNewDir * m_pPhysicsCom->Get_Physics().fSpeed * fDT(0);

	return vReturnPos;

}

void CNavigation::Exit_Wall()
{
	m_bOnWall = false;

	_float4 vMyUp = m_pOwner->Get_Transform()->Get_World(WORLD_UP);
	_float4 vCellNormal = _float4(0.f, 1.f, 0.f, 0.f);

	_float4 vTurnAxis = vMyUp.Cross(vCellNormal);
	_float fTurnTheta = acosf(vMyUp.Dot(vCellNormal));

	_float4x4 matRot;

	matRot = XMMatrixRotationAxis(vTurnAxis.XMLoad(), fTurnTheta);

	CTransform* pTransform = m_pOwner->Get_Transform();

	_float4		vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
	_float4		vUp = pTransform->Get_MyWorld(WORLD_UP).Normalize();
	_float4		vLook = pTransform->Get_MyWorld(WORLD_LOOK).Normalize();

	vRight = vRight.MultiplyNormal(matRot);
	vUp = vUp.MultiplyNormal(matRot);
	vLook = vLook.MultiplyNormal(matRot);

	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());

}

HRESULT CNavigation::Initialize_Prototype()
{
	m_pStartNode = CNode::Create(_float4());
	m_pEndNode = CNode::Create(_float4());
	return S_OK;
}

HRESULT CNavigation::Initialize()
{
	return S_OK;
}

void CNavigation::Start()
{
	__super::Start();
	//m_pPhysicsCom = GET_COMPONENT_FROM(m_pOwner, CPhysics);

	//if (!m_pPhysicsCom)
	//	Call_MsgBox(L"No Physics In Navi");
}

void CNavigation::Tick()
{
	if (nullptr == m_pPhysicsCom)
		return;
	if (nullptr == m_pCurCell)
		return;
	m_bOrigin = false;
	_float fSpeed = m_pPhysicsCom->Calculate_Speed();
	_float fFallPower = m_pPhysicsCom->Calculate_FreeFall();
	_float4 vCurPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vNewPos, vOutPlanePos, vFinalPos, vDir;
	vDir = m_pPhysicsCom->Get_Physics().vDir;

	vNewPos = vCurPos + vDir * fSpeed * fDT(0);


	//이동한 값을 대입시키고, 보정한다.
	//vNewPos += vDir * m_fEpsilon;

	//vFinalPos = Correct_Move(vNewPos);
	m_bInWall = false;

	if (m_pCurCell->IsWall())
	{
		for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
		{
			//세 점중 하나라도 내 높이보다 높다면 벽에 걸린거임.
			if (vCurPos.y + 1.f < XMVectorGetY(m_pCurCell->Get_Point(CCell::POINT(i))))
			{
				m_bInWall = true;
				break;
			}
		}
	}

	if (m_bInWall)
	{
		//벽에 들어있을 경우
		//벽의 법선벡터로 좀 밀려나야함
		vNewPos += m_pCurCell->Get_CellNormal() * 2.f * fDT(0);
	}

	vFinalPos = Correct_Move_Default(vNewPos);

	if (m_bInWall)
	{
		//벽에 들어있을 경우
		//벽의 법선벡터로 좀 밀려나야함
		vFinalPos += m_pCurCell->Get_CellNormal() * 2.f * fDT(0);
	}

	if (!m_bOrigin)
	{
		//vFinalPos -= vDir * m_fEpsilon;
	}

	if (!m_pCurCell->IsWall())
	{
		_float fNewGroundY = Get_NewPosFromCell(m_pCurCell, vFinalPos.XMLoad()).y;
		m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = fNewGroundY;
	}

	//일단 y 점프 값 넣어보고
	vFinalPos.y += fFallPower;

	//Check Air를 통해서 갱신해준 땅 높이보다 낮으면 땅으로 높이 보정
	_float fNewY = m_pPhysicsCom->Check_Air(vFinalPos.y);

	if (fNewY != -10.f)
		vFinalPos.y = fNewY;

	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vFinalPos);
}

void CNavigation::Late_Tick()
{
	if (nullptr == m_pPhysicsCom)
		return;
	m_pPhysicsCom->Late_Tick();
}

void CNavigation::Release()
{
}

//_float4 CNavigation::Correct_Move(_float4 vPos)
//{
//	if (!m_pCurCell)
//		return vPos;
//	//경우의 수 정리
//	// 1. 땅에 있을 때
//	// 2. 벽에 있을 때
//	// 3. 공중에 있을 때
//		return Correct_Move_Default(vPos);
//}

_float4 CNavigation::Correct_Move_Default(_float4 vPos)
{
	_float4 vOriginPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vFinalPos(vPos);

	CCell* pNeighBorCell = nullptr;
	CCell::LINE	eOutLine = CCell::LINE_END;

	_float4 vCurDir = m_pPhysicsCom->Get_Physics().vDir;
	_float fCurSpeed = m_pPhysicsCom->Get_Physics().fSpeed;

	

	//현재 셀 안에서만 움직였음
	if (m_pCurCell->isIn(vPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		m_bBlocked = false;

		return vFinalPos;
	}
	else
	{
		/* 무조건 특정 라인 밖으로 나간 상황 */

		// 나간쪽에 셀이 없음
		if (!pNeighBorCell)
		{
			//라인 타기
			vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
			return vFinalPos;
		}
		else
		{
			//못가는 셀이면 그냥 못가
			if (pNeighBorCell->IsBlocked())
			{
				vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
				return vFinalPos;
			}


			//벽 확인부터
			if (pNeighBorCell->IsWall())
			{
				_bool	bWallCol = false;
					for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
					{
						//세 점중 하나라도 내 높이보다 높다면 벽에 걸린거임.
						if (vFinalPos.y + 1.f < XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i))))
						{
							bWallCol = true;
							break;
						}
					}

				//벽에 박았으믄
				if (bWallCol)
				{
					if (pNeighBorCell->IsWall())
					{
						m_bOnWall = true;
						m_pCurWallCell = pNeighBorCell;
						m_fTargetY = -100.f;
						for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
						{
							_float fY = XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i)));
							if (m_fTargetY < fY)
								m_fTargetY = fY;
						}
					}

					//박았으면 못가
					vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
					return vFinalPos;
				}
				else
				{
					// 넘어간 셀 다음셀이 벽인데. 나보다 낮으면.. 일단 무시해야대는데

					//안박았는데 공중이 아니면 점프시키는데?
					if (!m_pPhysicsCom->Get_Physics().bAir)
					{
						_float fNewGroundY = Get_NewPosFromCell(pNeighBorCell, vFinalPos.XMLoad()).y;
						m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = -10.f;
						m_pPhysicsCom->Set_Jump();
						vFinalPos.y += 0.1f;
						m_pCurCell = pNeighBorCell;
						return vFinalPos;

					}
				}
			}
			
			

			//가도 되는 셀임
			_uint iLoopCnt = 0;
			while (1)
			{
				iLoopCnt++;
				if (iLoopCnt > 1000)
					return vOriginPos;

				if (pNeighBorCell->isIn(vFinalPos.XMLoad(), &pNeighBorCell, &eOutLine))
					break;

				if (!pNeighBorCell)
				{
					m_bOrigin = true;
					m_bBlocked = true;
					return vOriginPos;

				}
			}

			//못가는 셀이면 그냥 못가
			if (pNeighBorCell->IsBlocked())
			{
				vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
				return vFinalPos;
			}

			//한번 더 체크
			if (pNeighBorCell->IsWall())
			{
				_bool	bWallCol = false;
				for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
				{
					//세 점중 하나라도 내 높이보다 높다면 벽에 걸린거임.
					if (vFinalPos.y + 1.f< XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i))))
					{
						bWallCol = true;
						break;
					}
				}

				//벽에 박았으믄
				if (bWallCol)
				{
					if (pNeighBorCell->IsWall())
					{
						m_bOnWall = true;
						m_pCurWallCell = pNeighBorCell;

						m_fTargetY = -100.f;
						for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
						{
							_float fY = XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i)));
							if (m_fTargetY < fY)
								m_fTargetY = fY;
						}
					}

					//박았으면 못가
					vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
					return vFinalPos;
				}
				else
				{
					//안박았는데 공중아니면 못가

					//안박았으면 떨어지고, 밀려나
					if (!m_pPhysicsCom->Get_Physics().bAir)
					{
						_float fNewGroundY = Get_NewPosFromCell(pNeighBorCell, vFinalPos.XMLoad()).y;
						m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = -10.f;
						m_pPhysicsCom->Set_Jump();
						vFinalPos.y += 0.1f;
					}

					vFinalPos += pNeighBorCell->Get_CellNormal() * 2.f * fDT(0);

				}
			}

			


			//무난한 경우
			m_bOnWall = false;

			m_pCurCell = pNeighBorCell;
			return vFinalPos;
		}
	}


	return vFinalPos;
}

_float4 CNavigation::Correct_Move_BlockedAgain(CCell* pCell, _uint _eOutLine, _float4 vPos, _float4 _vDir, _float _fSpeed, _int iDepth)
{
	m_bBlocked = true;

	_float4 vOriginPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);

	if (iDepth >= 5)
		return vOriginPos;

	_float4 vFinalPos;
	CCell* pNeighBorCell = pCell;
	CCell::LINE	eOutLine = CCell::LINE_END;

	_float4 vNewPos;
	//새 위치가 나옴
	vNewPos = Get_NewDirFromCellLine(pNeighBorCell, _eOutLine, vPos.XMLoad(), _vDir, _fSpeed);

	
	// 새위치가 현재 Cell에 들어 있는지 검사
	if (pCell->isIn(vNewPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		m_pCurCell = pCell;
		return vNewPos;
	}
	else
	{
		//아니면 다시 찾아
		_uint iLoopCnt = 0;
		while (1)
		{
			iLoopCnt++;
			if (iLoopCnt > 1000)
				return vOriginPos;

			if (pNeighBorCell == nullptr)
				return vOriginPos;

			if (pNeighBorCell->isIn(vNewPos.XMLoad(), &pNeighBorCell, &eOutLine))
				break;
		}

		//다 찾았으면 갈 수 있는 셀인지 검사
		if (pNeighBorCell->IsBlocked() || pNeighBorCell->IsWall())
		{
			//못가는 곳이면 다시 검색해
			return Correct_Move_BlockedAgain(pNeighBorCell, eOutLine, vNewPos , _vDir, _fSpeed, ++iDepth);
		}
		else
		{
			m_pCurCell = pNeighBorCell;
			return vNewPos;
		}
	}

}

_float4 CNavigation::Get_NewPosFromCell(CCell* pCell, _vector vPos)
{
	_float4 vNewPos(vPos);

	/* ax + by + cz + d = 0 */
	_vector vPlane = XMPlaneFromPoints(pCell->Get_Point(CCell::POINT_A),
		pCell->Get_Point(CCell::POINT_B),
		pCell->Get_Point(CCell::POINT_C));

	_float fY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPos) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPos) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	vNewPos.y = fY;

	return vNewPos;
}

_float4 CNavigation::Get_NewDirFromCellLine(CCell* pCell, _uint eOutLine, _vector vPos, _float4 _vDir, _float _fSpeed)
{
	_float4 vOriginPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);

	//원래 가던 방향
	_float4 vMyDir = _vDir;
	_float4 vFinalPos = vOriginPos;

	//기존 길이
	_float	fOriginLength = _fSpeed * fDT(0);
	
	//일단 선에 닿을 때 까지는 움직여주고 길이도 줄여준다
	_float4 vCellDir = pCell->Get_Line((CCell::LINE)eOutLine);

	////직선의 방정식 교차점
	//vMyDir.y = 0.f;
	//vCellDir.y = 0.f;

	//_float4 vCrossPos = _float4(
	//()
	//)

	_float4 vOriginCellDir = vCellDir.Normalize();

	vMyDir *= fOriginLength;

	_float fNewSpeed = vMyDir.Dot(vCellDir);

	vFinalPos += vOriginCellDir * fNewSpeed;


	return vFinalPos;
}

_float4 CNavigation::Get_NewPosFromWall(CCell* pCell, _float4 _vRayPos, _float4 _vRayDir)
{

	_float4 vNewPos = _float4(0.f, 0.f, 0.f, 0.f);

	_vector			vVec0 = pCell->Get_Point(CCell::POINT_A);
	GXMVECTOR		vVec1 = pCell->Get_Point(CCell::POINT_B);
	HXMVECTOR		vVec2 = pCell->Get_Point(CCell::POINT_C);

	_vector vRayPos, vRayDir;
	_float fDist;
	_vRayDir.y = 0.f;
	_vRayDir.Normalize();
	vRayDir = _vRayDir.XMLoad();
	//위치는 셀의 법선벡터 방향으로 올려.
	vRayPos = _vRayPos.XMLoad();

	vRayDir = XMVector3Normalize(vRayDir);

	if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
	{
		vNewPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
	}

	return vNewPos;
}
