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

	if (m_rMap.GetConvLeft(GetX()) < pPlayer->GetRight()		// �÷��̾� �߳��ڶ��� ��� �¿� ���� �ְ�
		&& pPlayer->GetRight() <= m_rMap.GetConvRight(GetX())
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

bool CBlock::IsBlockInView(void)
{
	RECT rectViewSpace = TO_GAMEWORLD(GetGameWorld()).GetViewSpace()->GetRect();

	return IsCollided(GetConvRect(), rectViewSpace);
}

RECT CBlock::GetConvRect(void) const
{
	RECT rc = {
		GetConvLeft(),
		GetTop(),
		GetConvRight(),
		GetBottom()
	};
	return rc;
}

float CBlock::GetConvLeft(void) const
{
	return m_rMap.GetConvLeft(GetX());
}

float CBlock::GetConvRight(void) const
{
	return m_rMap.GetConvRight(GetX());
}
