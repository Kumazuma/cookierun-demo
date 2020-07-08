#include "stdafx.h"
#include "CMapObj.h"
#include "MainApp.h"
#include "CMap.h"


CMapObj::CMapObj(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, 0.f),
	m_rMap(_rMap)
{
}


CMapObj::~CMapObj()
{
}

void CMapObj::LateUpdate(void)
{
	CObj* pPlayer = TO_GAMEWORLD(GetGameWorld()).GetPlayer();
	DO_IF_IS_VALID_OBJ(pPlayer) {
		// 재순회시 아이템 재생성
		if (!IsValid() && pPlayer->GetRight() < GetConvLeft()) {
			SetValid(true);
		}
		DO_IF_IS_VALID_OBJ(this) {
			if (IsCollided(pPlayer->GetRect(), GetConvRect())) {
				SetValid(false);
			}
		}
	}
}

bool CMapObj::IsBlockInView(void)
{
	RECT rectViewSpace = TO_GAMEWORLD(GetGameWorld()).GetViewSpace()->GetRect();

	return IsCollided(GetConvRect(), rectViewSpace);
}

RECT CMapObj::GetConvRect(void) const
{
	RECT rc = {
		GetConvLeft(),
		GetTop(),
		GetConvRight(),
		GetBottom()
	};
	return rc;
}

float CMapObj::GetConvLeft(void) const
{
	return m_rMap.GetConvLeft(GetX());
}

float CMapObj::GetConvRight(void) const
{
	return m_rMap.GetConvRight(GetX());
}

float CMapObj::GetConvX(void) const
{
	return m_rMap.GetConvX(GetX());
}
