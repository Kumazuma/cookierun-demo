#include "stdafx.h"
#include "CBlock.h"
#include "CGameWorld.h"
#include "MainApp.h"
#include "CMap.h"


CBlock::CBlock(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, 0.f),
	m_rMap(_rMap)
{
}


CBlock::~CBlock()
{
}

void CBlock::Ready(void)
{
}

int CBlock::Update(void)
{


	return 0;
}

void CBlock::LateUpdate(void)
{
	RECT rectCollidedPoint;
	CObj* pPlayer = TO_GAMEWORLD(GetGameWorld()).GetPlayer();

	if (MyIntersectRect(pPlayer->GetRect(), GetRect(), &rectCollidedPoint)) {
		if (rectCollidedPoint.bottom - rectCollidedPoint.top > rectCollidedPoint.right - rectCollidedPoint.left) {
			if (pPlayer->GetX() < GetX()) {
				pPlayer->SetX(rectCollidedPoint.left - (pPlayer->GetWidth() >> 1) - 10);
			}
			else if (pPlayer->GetX() >= GetX()) {
				pPlayer->SetX(rectCollidedPoint.right + (pPlayer->GetWidth() >> 1) + 10);
			}
		}
		else {
			if (pPlayer->GetY() > GetY()) {
				pPlayer->SetY(rectCollidedPoint.bottom + (pPlayer->GetHeight() >> 1) + 10);
			}
			else if (pPlayer->GetY() <= GetY()) {
				pPlayer->SetY(rectCollidedPoint.top - (pPlayer->GetHeight() >> 1) - 10);
			}

		}
	}
}

void CBlock::Render(const HDC & _hdc)
{
	RECT rectViewSpace = TO_GAMEWORLD(GetGameWorld()).GetViewSpace()->GetRect();
	if (IsCollided(GetRect(), rectViewSpace)) {
		Rectangle(_hdc, GetLeft() - m_rMap.GetMapX(), GetTop(), GetRight() - m_rMap.GetMapX(), GetBottom());
	}
}

bool CBlock::IsBlockInView(void)
{
	RECT rectViewSpace = TO_GAMEWORLD(GetGameWorld()).GetViewSpace()->GetRect();

	return IsCollided(GetRect(), rectViewSpace);
}
