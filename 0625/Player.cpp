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
CPlayer::CPlayer(CGameWorld& _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, ciPlayerSize, ciPlayerSize, cfPlayerSpeed, Rectangle),
	m_hp{MAX_HP}
	
{

}

CPlayer::CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth /*= ciPlayerSize*/, size_t _iHeight /*= ciPlayerSize*/, float _fSpeed /*= cfPlayerSpeed*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fSpeed, Rectangle),
	m_hp{ MAX_HP }
{

}


CPlayer::~CPlayer()
{
	if (m_pPlayerState != nullptr)
	{
		delete m_pPlayerState;
		m_pPlayerState = nullptr;
	}
}

void CPlayer::Ready(void)
{
	m_pPlayerState = new State::Default{};
	m_pPlayerState->OnLoaded(this);
}

int CPlayer::Update(void)
{
	if (m_pPlayerState != nullptr)
	{
		const float delta = GetGameWorld().GetTimer()->GetElapsedTimePerFrame();
		//TODO: ���߿� ���� ��Ÿ ���� ���� �� �־�� ��.
		IPlayerState* nextState = m_pPlayerState->Update(this, delta);
		if (nextState != m_pPlayerState)
		{
			assert(nextState != nullptr);
			delete m_pPlayerState;
			m_pPlayerState = nextState;
			m_pPlayerState->OnLoaded(this);
		}
		m_hp -= delta * DECREASE_POINT_PER_SECOND;
		static wchar_t tmp[1024]{};
		static size_t len = 0;
		swprintf_s(tmp, L"%f", m_hp * 100.f / CPlayer::MAX_HP);
		SetWindowTextW(g_hWND, tmp);
		
		if (m_hp < 0 || WINCY < GetY())
		{
			SetWindowTextW(g_hWND, L"Game Over!");
			//SetValid(false);
			//TODO: ���ó��
			//app.GameOver();
		}
	}
	return 0;
}

void CPlayer::LateUpdate(void)
{

}

void CPlayer::Render(const HDC & _hdc)
{
	CObj::Render(_hdc);
	//�ӽ� ���� ���߿� ������ ��.


}

void CPlayer::Release(void)
{
}

void CPlayer::UseItem(const CObj* const pItem)
{
	auto pCoin = dynamic_cast<const Item::CCoin*>(pItem);
}

