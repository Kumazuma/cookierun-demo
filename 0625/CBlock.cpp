#include "stdafx.h"
#include "CBlock.h"
#include "CGameWorld.h"
#include "MainApp.h"
#include "CMap.h"


CBlock::CBlock(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CMapObj(_rGameWorld, _rMap, _fX, _fY, _iWidth, _iHeight)
{
}


CBlock::~CBlock()
{
}


void CBlock::LateUpdate(void)
{
	RECT rectCollidedPoint;
	CObj* pPlayer = TO_GAMEWORLD(GetGameWorld()).GetPlayer();

	if (MyIntersectRect(pPlayer->GetRect(), GetConvRect(), &rectCollidedPoint)) {
		/*if (rectCollidedPoint.bottom - rectCollidedPoint.top > rectCollidedPoint.right - rectCollidedPoint.left) {
			if (pPlayer->GetX() < GetX()) {
				pPlayer->SetX(rectCollidedPoint.left - (pPlayer->GetWidth() >> 1) - 1);
			}
			else if (pPlayer->GetX() >= GetX()) {
				pPlayer->SetX(rectCollidedPoint.right + (pPlayer->GetWidth() >> 1) + 1);
			}
		}
		else {*/
			if (pPlayer->GetY() > GetY()) {
				pPlayer->SetY(rectCollidedPoint.bottom + (pPlayer->GetHeight() >> 1) + 1);
			}
			else if (pPlayer->GetY() <= GetY()) {
				pPlayer->SetY(rectCollidedPoint.top - (pPlayer->GetHeight() >> 1) - 1);
			}
		//}
	}
	
	if (GetConvLeft() < pPlayer->GetRight()		// �÷��̾� �߳��ڶ��� ��� �¿� ���� �ְ�
		&& pPlayer->GetRight() <= GetConvRight()
		&& pPlayer->GetY() <= GetY()) {							// �÷��̾ ��Ϻ��� ���� ���� ���.
		m_rMap.SetBlockUnderPlayer(this);
	}
}

void CBlock::Render(const HDC & _hdc)
{
	if (IsBlockInView()) {
		Rectangle(_hdc, m_rMap.GetConvX(GetX()) - (GetWidth() >> 1) , GetTop(), m_rMap.GetConvX(GetX()) + (GetWidth() >> 1), GetBottom());
	}
}