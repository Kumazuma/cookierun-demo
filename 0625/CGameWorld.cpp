#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"

CGameWorld::CGameWorld()
	:
	m_pTimer(new CTimer(TIMER::TIMER_TYPE_TICKCOUNT))
{
	if (m_pTimer) m_pTimer->Reset();
}


CGameWorld::~CGameWorld()
{
	DeleteSafe(m_pTimer);
}

void CGameWorld::RunTick(void)
{
	if (m_pTimer) m_pTimer->RunTick();
}
