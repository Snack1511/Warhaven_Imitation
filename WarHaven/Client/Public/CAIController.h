#pragma once
#include "Component.h"
#include "Client_Defines.h"
#include "CTable_Conditions.h"
/*
    * AI�� ������Ʈ ���·� ����
    * ���º��濡 ���������� ����
    * �ܺο��� AIPersonality��� Ŭ������ �����༭ AI�� ������ ����
    * AIPersonality�� AI���� ������ ����ϴ� Ŭ���� --> ���� ���� �۾��غ��� ���� ���� ��...
    * ����� AIController�� �̸��� �����Ǿ� ������, ��Ȯ���� ���¼��������ڰ� ���� ���� ��...
    * �ش� ������Ʈ�� Transform������ ���ƾ� ��
    * 
    * AIController�� ������ �ܰ踦 ������ �ϴ� ������Ʈ
        0. ���� ���� �ܰ� CPhase_SetBehaviorCondition
            --> � �ൿ�� ���� �����ϴ� ����
            --> Personality�� ���� ���� ������ ��ǥ �÷��װ� ����
            --> ���ŵ� �÷��׵鿡 ���� �����ൿ��, �ݴ��ൿ��, �ൿ����, ��ǥ �缱�� ����, ���� �缱�� ���� ������
            -->  �����ൿ���, �ݴ��ൿ���� �������� ���Ұ�� ���� �Ұ�
        
         1. �ൿ���ôܰ� �ܰ� CPhase_SelectBehavior
            --> �ռ� ������ �ൿ�� �� �ϳ����� �����ϴ� �ܰ�
            --> �ൿ ���ǿ� ���� �ൿ���� ����
            --> �ൿ ���� ���Ƕ��� ���⼭ ����
            --> �ൿ ���ᰡ �Ǹ� ���⼭ ���ο� �ൿ���� �Ҵ�
        
        2. Ž�� �ܰ� 
            --> Ÿ���� ��ġ�� Ž���ϴ� �ܰ� 
            --> ���õ� ��ǥ�� ���� TileLayer�� ������ ��ã�� 
            --> ��ǥ�� �ٰŸ��̰� ���� TileLayer�� ��� �� ������ Ž��, �ƴ� ��� ���� ����(���� ���̾� ���� or Ư�� �ð���)���� Ž��

        3. �ൿ �ܰ�
            --> ���õ� Ÿ�ٿ� ������ ������ �޼��ϱ� ���� Unit�� ���¸� �����ϴ� �ܰ� 
            --> �ൿ ���� �� �����޼� �ܰ�� �Ѿ �� ����

        4. ���� �޼� �ܰ� 
            --> �ʱ� ������ ������ �޼� �ߴٰ� '�Ǵ�'�ϴ� �ܰ� 
            --> �޼� ���̵� Personality�� ���� �޼��ߴ� ġ�� �Ѿ �� ����
    * 
    * �����ƾ
    --> GameSystem���� CPlayer��ü�� ������ ��, mainPlayer�� �ƴ� �͵��� �ʼ������� AIPersonality�� �����ؼ� �־��ֵ���..
    --> AIPersonality�� ����� ���� Ȯ�强�� �߱��ؾ���

    * ������ ��ǥ
        --> ������ ��ǥ�� ������ �ش� ��Ȳ�� �´� �ൿ�� �ϵ��� ����������!
        ex1) Fight + Allies = �Ʊ� ����
        ex2) Fight + Enemy = ���� ����
        ex3) Interact + Enemy = Ƽ���
        ex4) Interact + Allies = Ƽ��� or ��Ȱ or ��ê���.. ������� �ٲ��..

    * ���� ����
    * ���� + ��ǥ + Personality�� ���� �ൿ������ ������ --> ���� ��ǥ �缱�� ���ǰ� ���� �缱�� ���Ƕ��� ���ÿ� ������ --> 

*/
BEGIN(Engine)

class CGameObject;
class CTile;
class CTileLayer;

END

BEGIN(Client)

class CUnit;
class CPlayer;
class CAIPersonality;
class CBehavior;
class CTrigger;
class CPath;

class CAIController :
    public CComponent
{

    DECLARE_PROTOTYPE(CAIController)
public:
    friend class CPlayer;
private:
    CAIController(_uint iGroupID);
    CAIController(const CAIController& rhs);
    virtual ~CAIController();

public:
    static CAIController* Create(CAIPersonality* pPersonality);

public:
    // CComponent��(��) ���� ��ӵ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize() override;
    virtual void Start() override;
    virtual void Tick() override;
    virtual void Late_Tick() override;
    virtual void Release() override;

    virtual void OnEnable() override;
    virtual void OnDisable() override;

public:
    list<CPlayer*> Get_NearAllies() { return m_NearAlliesList; }
    list<CPlayer*> Get_NearEnemy() { return m_NearEnemyList; }
    list<CTrigger*> Get_NearTrigger() { return m_NearTriggerList; }
public:
    void Ready_Controller();

private:
    CPlayer* m_pOwnerPlayer = nullptr;
    CAIPersonality* m_pPersonality = nullptr;
    list<CGameObject*> m_NearObjectList;
   
    CBehavior* m_pCurrentBehavior = nullptr;
    list<CBehavior*> m_BehaviorList;

private:
    /* AI �Ǵܿ� �ʿ��� ������ */
    list<CPlayer*> m_NearAlliesList;
    list<CPlayer*> m_NearEnemyList;
    list<CTrigger*> m_NearTriggerList;
    /* ���� Ÿ�� �ִ� ���*/
    CPath* m_pCurPath = nullptr;
};

END