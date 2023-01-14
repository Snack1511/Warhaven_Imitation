#include "CNavigation.h"

#include "CCell.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "CNode.h"
#include "CCellLayer.h"

#include "PhysX_Manager.h"

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

_bool CNavigation::Is_BlockedCell(_float4 vPosition)
{
	CCell* pCurCell = Get_CurCell(vPosition, *m_pLayers, nullptr);
	return pCurCell->IsBlocked();
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

//�񸮽�Ʈ �����
//�ش縮��Ʈ�� �������� ��ã�� �� �� ��ġ �ֱ�
//A. ���귮 ����
list<pair<_float4, CCellLayer*>> CNavigation::Get_Goals(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd, CCellLayer* pStartLayer)
{
	//���� ��ġ�� �����̾����
	//���� ��ġ�� �����̾����
	//�� ���� �ִܰŸ��� ��� ���� ����������Ʈ�� ����       
	list<pair<_float4, CCellLayer*>> GoalList;
	//CCellLayer* pStartLayer = nullptr;
	CCellLayer* pEndLayer = nullptr;

	
	CCell* pEndCell = nullptr;
	pEndCell = Get_CurCell(vEnd, Layers, &pEndLayer);

	//�̷л� ������ �ϳ��� ���̾ ������ �־�� ��
	if (nullptr == pEndLayer)
		assert(0);

	if (pEndCell->Check_Attribute(CELL_BLOCKED))
	{
		pEndCell = pEndLayer->Find_NearOpenCell(pEndCell);
	}


	if (pEndCell->Check_Attribute(CELL_BLOCKED)) 
	{
		assert(0);
	}


	_float4 vEndPos = pEndCell->Get_Position();
	if (pStartLayer == pEndLayer)
	{
		GoalList.push_back(make_pair(vEndPos, pEndLayer));
		return GoalList;
	}

	CCellLayer* pCurLayer = pStartLayer;
	CCellLayer* pNextLayer = pEndLayer;
	//���� ���̾�� ��ǥ ���̾������ ���ľ� �� ���̾� ������ �����ϸ� ��
	if (pCurLayer->Is_Access(pNextLayer->Get_MinHeight()))
	{
		//���� ���̾�� ��ǥ ���̾������ ���ľ� �� ���̾� ����
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

			//���� ���̾�� ����� ���� ���̾��� StairŰ
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

			//���� ���̾��� Stairlist --> �������̾�� ���� �ε����� �� ����
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
	//	//�� �����̾��� Stair���� ���� �� �����Ǵ� ������ ������ �� ���̾�� �ش� ������� �����
	//	list<CCell*> StairCellList;
	//	for (auto Iter = CmpIter; Iter != Layers.end(); ++Iter)
	//	{
	//		//�ٸ� ���̾����..CmpIter�� ����� ��� ����Ʈ ��������
	//		list<CCell*> StairList = Iter->second->Get_StairCellList(CmpIter->first);
	//		//���� ����Ʈ�� ����
	//		StairCellList.merge(StairList);
	//	}
	//	if (!StairCellList.empty()) 
	//	{
	//		//������ġ ����� ������ ����
	//		StairCellList.sort([&vStartPos](auto& sour, auto& Dest)
	//			{
	//				_float SourLen = (sour->Get_Position() - vStartPos).Length();
	//				_float DestLen = (Dest->Get_Position() - vStartPos).Length();
	//				if (SourLen > DestLen)
	//					return true;
	//				else return false;
	//			});
	//		//���� ����� ��ġ ����
	//		GoalList.push_back(make_pair((*StairCellList.begin())->Get_Position(), CmpIter->second));
	//	}
	//	if (GoalList.empty())
	//		break;
	//	//CmpIter�� vStartPos����
	//	vStartPos = GoalList.back().first;
	//}
 //
	//GoalList.push_back(make_pair(vEnd, pEndLayer));
	return GoalList;
}

/*

//���� ��ġ�� �����̾����
	//���� ��ġ�� �����̾����
	//�� ���� �ִܰŸ��� ��� ���� ����������Ʈ�� ����
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

	
	* EndLayer���� ���� ���� ó�� ���̾���� �ݺ�
	* CmpLayer = EndLayer--
	* EndLayer�� Stair --> �갡 �����Ǹ� ����� ���̾�
	*		Stair�����ͼ� �ش� ���� �ε����� ��ġ�ϴ� �ε����� ����� ���̾�� ��ġ ���
	*		���� ��� ��ġ�� CmpPosition�� ���� ª�������� ����
	*		���� ����� ��ġ�� ���̾� ��������
				--> StartLayer�� ������ ������ �ݺ�
	* ����� ���̾ ���ٸ�? --> � ���̾�͵� ������� ���� ���̾�
	
_float4 vTargetPosition = vEnd;
auto EndLayerIter = Layers.find(pEndLayer->Get_MinHeight());
if (EndLayerIter == Layers.end() || EndLayerIter == Layers.begin())
assert(0);
//�ϴ� �����ϰ� ã��
for (auto TargetLayerIter = EndLayerIter; TargetLayerIter->second != pStartLayer;)
{
	//��� ����Ʈ ������
	list<pair<_float4, CCellLayer*>> LayerList;
	list<CCell*> StairList = TargetLayerIter->second->Get_StairCellList();
	//����Ʈ ���鼭
	for (auto Cell : StairList)
	{
		_int CellIndex = Cell->Get_Index();
		//��� �����̾���� �˻���
		for (auto Layer : Layers)//n
		{
			//���� ���̾��?
			if (TargetLayerIter->second == Layer.second)
			{
				//����
				continue;
			}
			CCell* pCell = Layer.second->Get_Cell(CellIndex);
			//����� �ֳ�?
			if (nullptr != pCell && pCell->Check_Attribute(CELL_STAIR))
			{
				_float4 vCellPosition = pCell->Get_Position();
				//�׷� �����
				//�ٴ� ���۷��̾�� ������ֳ�?
				if (Layer.second == pStartLayer)
				{
					//�׷� �̶����� ����Ѱ� �� ������ �̰Ÿ� ����ϰ� ����
					LayerList.clear();
					LayerList.push_back(make_pair(vCellPosition, Layer.second));
					break;
				}
				LayerList.push_back(make_pair(vCellPosition, Layer.second));

			}
		}
	}

	//vTargetPosition���� �Ÿ��������� ������
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

	//front���� ������
	GoalList.push_back(LayerList.front());

	//���� StartLayer�� ������
	if (LayerList.front().second == pStartLayer)
	{
		//����
		break;

	}
	else//�ƴ϶��
	{
		//���� �� ������Ʈ �ϰ� �ݺ���
		vTargetPosition = LayerList.front().first;
		TargetLayerIter = Layers.find(LayerList.front().second->Get_MinHeight());
	}
}

//�Ųٷ� ���ִ°� ����
GoalList.reverse();

return GoalList;

*/

//A. ���귮 ����

int CNavigation::Func_MakeRoute(list<_float4>* NodeList, map<_float, CCellLayer*>* Layers, _float4 vStart, _float4 vEnd, CNavigation* pNaviCom)
{
	/* Locked üũ */
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

	/* ��ã�� */
	*NodeList = pNaviCom->Get_BestRoute(*Layers, vStart, vEnd);

	for (auto& elem : *Layers)
		elem.second->UnLock();

	pNaviCom->m_bThreadOn = false;

	return 0;
}

int CNavigation::Func_GetStartCell(list<_float4>* NodeList, map<_float, CCellLayer*>* Layers, _float4 vStart, _float4 vEnd, CNavigation* pNaviCom)
{
	pNaviCom->Get_NearOpenCell(vStart, *Layers, &pNaviCom->m_pStartLayer);

	return 0;

}


void CNavigation::Make_Route(list<_float4>* NodeList, map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd)
{
	if (m_bThreadOn)
		return;

	//std::future<int> result = std::async(bind(Func_GetStartCell, NodeList, &Layers, vStart, vEnd, this));


	std::future<int> result2 = std::async(bind(Func_MakeRoute, NodeList, &Layers, vStart, vEnd, this));

	//*NodeList = Get_BestRoute(Layers, vStart, vEnd);
}

list<_float4> CNavigation::Get_BestRoute(map<_float, CCellLayer*>& Layers, _float4 vStart, _float4 vEnd)
{
	m_DebugRouteNode.clear();


	//start cell�� start layer���� ���ؾ���

	m_pStartCell = Get_CurCell(vStart, Layers, &m_pStartLayer);

	if (!m_pStartCell)
		return list<_float4>();

	if (!m_pStartLayer)
		return list<_float4>();

	if (m_pStartCell->Check_Attribute(CELL_BLOCKED))
		return list<_float4>();

	CCellLayer* pEndLayer = nullptr;
	CCell* pEndCell = Get_CurCell(vEnd, Layers, &pEndLayer);

	if (!pEndCell)
		return list<_float4>();

	if (!pEndLayer)
		return list<_float4>();

	if (pEndCell->Check_Attribute(CELL_BLOCKED))
		return list<_float4>();



	_float4 vStartPos = m_pStartCell->Get_Position();
	//GoalList�� ���������� ��� �������� ��ġ
	list<pair<_float4, CCellLayer*>> GoalList = Get_Goals(Layers, vStartPos, vEnd, m_pStartLayer);
	
	GoalList.push_front(make_pair(vStartPos, m_pStartLayer));

	CCellLayer::CellList Routes;

	m_pStartNode->Set_NodePosition(vStart);
	Routes.push_back(m_pStartLayer->Find_Cell(vStart));
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

	
	//������ġ�� �� ��ġ ��ó�� ���� ���� ����
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

	//�ͷ��κ��� PositionY�� ������
	_float fRelativeY = 0.f;
	_float4 vOutPos;

	if (!CPhysX_Manager::Get_Instance()->Shoot_RaytoTerrain(&vOutPos, &fRelativeY, vPosition))
	{
		assert(0);
		return nullptr;
	}

	for (auto Layer : Layers)
	{
		if (Layer.first <= fRelativeY)
			pCellLayer = Layer.second;
	}

	//�����̷� ���̾� ������
	//auto TargetLayerIter = Layers.find(fLayerKey);
	//pCellLayer = TargetLayerIter->second;


	if (!pCellLayer)
	{
		pCellLayer = Layers.begin()->second;
	}

	pReturn = pCellLayer->Find_Cell(vPosition);

	if (ppOutInCellLayer)
		(*ppOutInCellLayer) = pCellLayer;


	return pReturn;
}

CCell* CNavigation::Get_NearOpenCell(_float4 vPosition, map<_float, CCellLayer*>& Layers, CCellLayer** ppOutInCellLayer)
{
	CCell* pReturn = nullptr;
	CCellLayer* pCellLayer = nullptr;


	pReturn = Get_CurCell(vPosition, Layers, &pCellLayer);

	if (ppOutInCellLayer)
		*ppOutInCellLayer = pCellLayer;

	m_pStartLayer = pCellLayer;

	if (!m_pStartLayer)
	{
		MessageBox(0, L"����2", TEXT("System Error"), MB_OK);
	}

	if (!pReturn->IsBlocked())
	{
		m_pStartCell = pReturn;

	}
	else
	{
		static _int g_iCnt = 0;
		g_iCnt++;
		pReturn = pCellLayer->Find_NearOpenCell(pReturn);
		m_pStartCell = pReturn;
	}

	if (nullptr == m_pStartCell)
		assert(0);

	return m_pStartCell;
}

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

	//���� ��ġ ���ϱ�
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


	//�̵��� ���� ���Խ�Ű��, �����Ѵ�.
	//vNewPos += vDir * m_fEpsilon;

	//vFinalPos = Correct_Move(vNewPos);
	m_bInWall = false;

	if (m_pCurCell->IsWall())
	{
		for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
		{
			//�� ���� �ϳ��� �� ���̺��� ���ٸ� ���� �ɸ�����.
			if (vCurPos.y + 1.f < XMVectorGetY(m_pCurCell->Get_Point(CCell::POINT(i))))
			{
				m_bInWall = true;
				break;
			}
		}
	}

	if (m_bInWall)
	{
		//���� ������� ���
		//���� �������ͷ� �� �з�������
		vNewPos += m_pCurCell->Get_CellNormal() * 2.f * fDT(0);
	}

	vFinalPos = Correct_Move_Default(vNewPos);

	if (m_bInWall)
	{
		//���� ������� ���
		//���� �������ͷ� �� �з�������
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

	//�ϴ� y ���� �� �־��
	vFinalPos.y += fFallPower;

	//Check Air�� ���ؼ� �������� �� ���̺��� ������ ������ ���� ����
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
//	//����� �� ����
//	// 1. ���� ���� ��
//	// 2. ���� ���� ��
//	// 3. ���߿� ���� ��
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

	

	//���� �� �ȿ����� ��������
	if (m_pCurCell->isIn(vPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		m_bBlocked = false;

		return vFinalPos;
	}
	else
	{
		/* ������ Ư�� ���� ������ ���� ��Ȳ */

		// �����ʿ� ���� ����
		if (!pNeighBorCell)
		{
			//���� Ÿ��
			vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
			return vFinalPos;
		}
		else
		{
			//������ ���̸� �׳� ����
			if (pNeighBorCell->IsBlocked())
			{
				vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
				return vFinalPos;
			}


			//�� Ȯ�κ���
			if (pNeighBorCell->IsWall())
			{
				_bool	bWallCol = false;
					for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
					{
						//�� ���� �ϳ��� �� ���̺��� ���ٸ� ���� �ɸ�����.
						if (vFinalPos.y + 1.f < XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i))))
						{
							bWallCol = true;
							break;
						}
					}

				//���� �ھ�����
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

					//�ھ����� ����
					vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
					return vFinalPos;
				}
				else
				{
					// �Ѿ �� �������� ���ε�. ������ ������.. �ϴ� �����ؾߴ�µ�

					//�ȹھҴµ� ������ �ƴϸ� ������Ű�µ�?
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
			
			

			//���� �Ǵ� ����
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

			//������ ���̸� �׳� ����
			if (pNeighBorCell->IsBlocked())
			{
				vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
				return vFinalPos;
			}

			//�ѹ� �� üũ
			if (pNeighBorCell->IsWall())
			{
				_bool	bWallCol = false;
				for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
				{
					//�� ���� �ϳ��� �� ���̺��� ���ٸ� ���� �ɸ�����.
					if (vFinalPos.y + 1.f< XMVectorGetY(pNeighBorCell->Get_Point(CCell::POINT(i))))
					{
						bWallCol = true;
						break;
					}
				}

				//���� �ھ�����
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

					//�ھ����� ����
					vFinalPos = Correct_Move_BlockedAgain(m_pCurCell, eOutLine, vFinalPos, vCurDir, fCurSpeed, 0);
					return vFinalPos;
				}
				else
				{
					//�ȹھҴµ� ���߾ƴϸ� ����

					//�ȹھ����� ��������, �з���
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

			


			//������ ���
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
	//�� ��ġ�� ����
	vNewPos = Get_NewDirFromCellLine(pNeighBorCell, _eOutLine, vPos.XMLoad(), _vDir, _fSpeed);

	
	// ����ġ�� ���� Cell�� ��� �ִ��� �˻�
	if (pCell->isIn(vNewPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		m_pCurCell = pCell;
		return vNewPos;
	}
	else
	{
		//�ƴϸ� �ٽ� ã��
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

		//�� ã������ �� �� �ִ� ������ �˻�
		if (pNeighBorCell->IsBlocked() || pNeighBorCell->IsWall())
		{
			//������ ���̸� �ٽ� �˻���
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

	//���� ���� ����
	_float4 vMyDir = _vDir;
	_float4 vFinalPos = vOriginPos;

	//���� ����
	_float	fOriginLength = _fSpeed * fDT(0);
	
	//�ϴ� ���� ���� �� ������ �������ְ� ���̵� �ٿ��ش�
	_float4 vCellDir = pCell->Get_Line((CCell::LINE)eOutLine);

	////������ ������ ������
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
	//��ġ�� ���� �������� �������� �÷�.
	vRayPos = _vRayPos.XMLoad();

	vRayDir = XMVector3Normalize(vRayDir);

	if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
	{
		vNewPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
	}

	return vNewPos;
}
