#include "stdafx.h"
#include "CObstacle.h"
#include "MainApp.h"
#include "CMap.h"


CObstacle::CObstacle(CGameWorld & _rGameWorld, CMap & _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CMapObj(_rGameWorld, _rMap, _fX, _fY, _iWidth, _iHeight)
{
	CObj* pPlayer = TO_GAMEWORLD(_rGameWorld).GetPlayer();
	float fDeltaHeight = (_rMap.GetBlockHeight() - pPlayer->GetHeight() / 2.f) * 0.7f;
	float fNewHeight = static_cast<int>(GetBottom() + fDeltaHeight - GetTop());
	float fNewY = (static_cast<int>(GetBottom() + fDeltaHeight + GetTop()) >> 1);

	SetHeight(fNewHeight);
	SetY(fNewY);
}

CObstacle::~CObstacle()
{
}

void CObstacle::LateUpdate(void)
{
	CObj* pPlayer = TO_GAMEWORLD(GetGameWorld()).GetPlayer();

	if (IsCollided(pPlayer->GetRect(), GetConvRect())) {
		// TODO1 : �÷��̾�� �������� �ش�.
		// TODO2 : ���� �ӵ��� �Ͻ������� ��������.
	}
}

void CObstacle::Render(const HDC & _hdc)
{
	if (IsBlockInView()) {
		Rectangle(_hdc, m_rMap.GetConvX(GetX()) - (GetWidth() >> 1), GetTop(), m_rMap.GetConvX(GetX()) + (GetWidth() >> 1), GetBottom());
	}
}
