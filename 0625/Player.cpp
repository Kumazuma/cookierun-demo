#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "ScrewBullet.h"
#include "GuidedBullet.h"
#include "GunBarrel.h"
#include "Shield.h"
#include "MainApp.h"
#include "State.h"
#include "CMap.h"
#include "CBlock.h"
#include <assert.h>
#include "CMap.h"
#include "CBlock.h"
#include "item.h"
#include "UI_Gauge.h"
CPlayer::CPlayer(CGameWorld& _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, ciPlayerWidth, ciPlayerHeight, cfPlayerSpeed, Rectangle),
	m_hp{MAX_HP},
	m_pHpGauge(nullptr)
	//m_pHpGauge(new CUI_Gauge(_rGameWorld, this, _iWidth * 2.f, 10.f, m_fHp, m_fHp, 0.f, -((m_iHeight >> 1) + 10.f)))
	
{

}

CPlayer::CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth /*= ciPlayerSize*/, size_t _iHeight /*= ciPlayerSize*/, float _fSpeed /*= cfPlayerSpeed*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fSpeed, Rectangle),
	m_hp{ MAX_HP },
	m_pHpGauge(new CUI_Gauge(_rGameWorld, this, _iWidth * 2.f, 20.f, m_hp, m_hp, 0.f, -((m_iHeight >> 1) + 10.f)))
{
	m_pHpGauge->AdjustPositionToOwner(false);
	m_pHpGauge->SetX(600.f);
	m_pHpGauge->SetY(35.f);
	m_pHpGauge->SetMaxWidth(WINCX >> 1);
	m_pHpGauge->SetWidth(WINCX >> 1);
}


CPlayer::~CPlayer()
{
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}
}

void CPlayer::Ready(void)
{
	m_pMoveState = new State::Move::Default{};
	m_pMoveState->OnLoaded(this);
}

int CPlayer::Update(void)
{
	if (m_pMoveState != nullptr)
	{
		const float delta = GetGameWorld().GetTimer()->GetElapsedTimePerFrame();
		//TODO: 나중에 실제 델타 값을 넣을 수 있어야 함.
		IPlayeMoveState* nextState = m_pMoveState->Update(this, delta);
		if (nextState != m_pMoveState)
		{
			assert(nextState != nullptr);
			delete m_pMoveState;
			m_pMoveState = nextState;
			m_pMoveState->OnLoaded(this);
		}
		m_hp -= delta * DECREASE_POINT_PER_SECOND;
		m_pHpGauge->SetCurrentGauge(m_hp);
		static wchar_t tmp[1024]{};
		static size_t len = 0;
		swprintf_s(tmp, L"%f", m_hp * 100.f / CPlayer::MAX_HP);
		SetWindowTextW(g_hWND, tmp);
		
		if (m_hp < 0 || WINCY < GetY())
		{
			SetWindowTextW(g_hWND, L"Game Over!");
			//SetValid(false);
			//TODO: 사망처리
			GetGameWorld().GameOver();
		}
	}

	DO_IF_IS_VALID_OBJ(m_pHpGauge) { m_pHpGauge->Update(); }

	return 0;
}

void CPlayer::LateUpdate(void)
{

}

void CPlayer::Render(const HDC & _hdc)
{
	CObj::Render(_hdc);
	//임시 땜빵 나중에 지워야 함.

	DO_IF_IS_VALID_OBJ(m_pHpGauge) { m_pHpGauge->Render(_hdc); }
}

void CPlayer::Release(void)
{
	DeleteSafe(m_pHpGauge);
}

void CPlayer::UseItem(const CObj* const pItem)
{
	auto pCoin = dynamic_cast<const Item::CCoin*>(pItem);
}

