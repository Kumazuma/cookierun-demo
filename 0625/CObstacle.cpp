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
		// TODO1 : 플레이어에게 데미지를 준다.
		// TODO2 : 맵의 속도가 일시적으로 느려진다.
	}
}

void CObstacle::Render(const HDC & _hdc)
{
	if (IsBlockInView()) {
		Rectangle(_hdc, m_rMap.GetConvX(GetX()) - (GetWidth() >> 1), GetTop(), m_rMap.GetConvX(GetX()) + (GetWidth() >> 1), GetBottom());
	}
}
