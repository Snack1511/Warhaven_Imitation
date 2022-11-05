//#include "..\Public\ZFrustum.h"
//
//
//
//CZFrustum::CZFrustum()
//{
//}
//
//
//CZFrustum::~CZFrustum()
//{
//}
//
//_bool CZFrustum::Make_Plane(const _float4x4 & matViewProj)
//{
//	int				i;
//	D3DXMATRIXA16	matInv;
//
//	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
//	// m_vtx�� �� ���������� ��谪�� �־�д�.
//	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
//	m_vtx[1].x = 1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
//	m_vtx[2].x = 1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
//	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
//	m_vtx[4].x = -1.0f;	m_vtx[4].y = 1.0f;	m_vtx[4].z = 0.0f;
//	m_vtx[5].x = 1.0f;	m_vtx[5].y = 1.0f;	m_vtx[5].z = 0.0f;
//	m_vtx[6].x = 1.0f;	m_vtx[6].y = 1.0f;	m_vtx[6].z = 1.0f;
//	m_vtx[7].x = -1.0f;	m_vtx[7].y = 1.0f;	m_vtx[7].z = 1.0f;
//
//	// view * proj�� ������� ���Ѵ�.
//	D3DXMatrixInverse(&matInv, NULL, &matViewProj);
//
//	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
//	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
//	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
//	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
//	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
//	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
//	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
//	for (i = 0; i < 8; i++)
//		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);
//
//	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
//	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
//	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;
//
//	// ����� ������ǥ�� �������� ����� �����
//	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
//	//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// �� ���(top)
//	//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// �� ���(bottom)
//	//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// �� ���(near)
//	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);	// �� ���(far)
//	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);	// �� ���(left)
//	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);	// �� ���(right)
//
//	return TRUE;
//}
//
//_bool CZFrustum::IsInPlane(const _float4 & vPos)
//{
//	float		fDist;
//	//	int			i;
//
//	// ����� left, right, far plane�� �����Ѵ�.
//	//	for( i = 0 ; i < 6 ; i++ )
//	{
//		fDist = D3DXPlaneDotCoord(&m_plane[3], &vPos);
//		if (fDist > PLANE_EPSILON) return FALSE;	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
//		fDist = D3DXPlaneDotCoord(&m_plane[4], &vPos);
//		if (fDist > PLANE_EPSILON) return FALSE;	// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
//		fDist = D3DXPlaneDotCoord(&m_plane[5], &vPos);
//		if (fDist > PLANE_EPSILON) return FALSE;	// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
//	}
//
//	return TRUE;
//}
//
//_bool CZFrustum::IsInSphere(const _float4 & vPos, const _float& fRadius)
//{
//	float		fDist;
//
//	fDist = D3DXPlaneDotCoord(&m_plane[3], &vPos);
//	if (fDist > (fRadius + PLANE_EPSILON)) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
//	fDist = D3DXPlaneDotCoord(&m_plane[4], &vPos);
//	if (fDist > (fRadius + PLANE_EPSILON)) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
//	fDist = D3DXPlaneDotCoord(&m_plane[5], &vPos);
//	if (fDist > (fRadius + PLANE_EPSILON)) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
//
//	return TRUE;
//}
