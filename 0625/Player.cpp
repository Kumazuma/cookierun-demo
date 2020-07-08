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
CPlayer::CPlayer(CGameWorld& _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, ciPlayerSize, ciPlayerSize, cfPlayerSpeed, Rectangle)
	
{
}

CPlayer::CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth /*= ciPlayerSize*/, size_t _iHeight /*= ciPlayerSize*/, float _fSpeed /*= cfPlayerSpeed*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fSpeed, Rectangle)
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
		
		//TODO: 나중에 실제 델타 값을 넣을 수 있어야 함.
		IPlayerState* nextState = m_pPlayerState->Update(this, GetGameWorld().GetTimer()->GetElapsedTimePerFrame());
		if (nextState != m_pPlayerState)
		{
			assert(nextState != nullptr);
			delete m_pPlayerState;
			m_pPlayerState = nextState;
			m_pPlayerState->OnLoaded(this);
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
}

void CPlayer::Release(void)
{
}
