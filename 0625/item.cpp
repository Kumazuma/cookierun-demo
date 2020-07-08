#include "stdafx.h"
#include "item.h"
#include "CGameWorld.h"
#include "Player.h"
#include "MainApp.h"
#include "CMap.h"
#include <assert.h>
constexpr float ItemMaxSize = 40.f;

CItem::CItem(CGameWorld& world, CMap& _rMap, float fX, float fY, CItem::Type type):
	CMapObj(world, _rMap, fX, fY, 0.f, 0.f),
	m_realScale{1.f},
	m_itemType{type},
	m_sign{-1}
{
	SetWidth(ItemMaxSize);
	SetHeight(ItemMaxSize);
}
template<>
CObj* CItem::CreateItem<Item::CCoin>(CGameWorld& world, CMap& _rMap, float fX, float fY)
{
	auto* pCoin{ new Item::CCoin{world, _rMap, fX, fY} };
	return pCoin;
}
template<>
CObj* CItem::CreateItem<Item::CLife>(CGameWorld& world, CMap& _rMap, float fX, float fY)
{
	auto* pLife{ new Item::CLife{world, _rMap, fX, fY} };
	return pLife;
}
int CItem::Update(void)
{
	//TODO: 실제 타임 델타가 필요함
	float fTimeDelta = GetGameWorld().GetTimer()->GetElapsedTimePerFrame();
	m_realScale += m_sign  * m_fSpeed * fTimeDelta; //1초당 3%씩 스케일이 변한다.
	//작아지는 중인데, 스케일이 0보다 떨어지면,
	
	if (m_sign == -1 && m_realScale < -1.f)
	{
		m_sign = 1;
	}
	else if (m_sign == 1 && m_realScale > 1.f)
	{
		m_sign = -1;
	}
	if (m_realScale > 1.f)
	{
		m_realScale = 1.f;
	}
	else if (m_realScale < -1.f)
	{
		m_realScale = -1.f;
	}
	return 0;
}

void CItem::LateUpdate(void)
{
	auto& app = (CMainApp&) GetGameWorld();
	auto pMap = app.GetMap();

	CObj* pPlayer = app.GetPlayer();
	float distanceXBetweenCenters =
		fabsf(pMap->GetConvX(GetX()) - pPlayer->GetX());
	float distanceYBetweenCenters =
		fabsf(GetY() - pPlayer->GetX());
	float sumRadiusX = (GetWidth() + pPlayer->GetWidth()) / 2.f;
	float sumRadiusY = (GetHeight() + pPlayer->GetHeight()) / 2.f;
	bool intersact =
		sumRadiusX >= distanceXBetweenCenters &&
		sumRadiusY >= distanceYBetweenCenters;
	//두 반지름의 합보다 실제 거리가 작으면 겹쳐진 것이다.
	if (intersact)
	{
		auto p = dynamic_cast<CPlayer*>(pPlayer);
		assert(p != nullptr);
		//합쳐지면 지워여야 하고, 플레이어에게 효과를 부여한다.
		Effect(p);
		SetValid(false);
	}
}
Item::CCoin::CCoin(CGameWorld& world, CMap& _rMap, float fX, float fY) :
	CItem{ world, _rMap, fX, fY , CItem::Type::COIN }
{
	m_fSpeed = 3.f;
}
void Item::CCoin::Render(const HDC& hDC)
{
	if (IsBlockInView()) {
		auto& app = (CMainApp&)GetGameWorld();
		auto pMap = app.GetMap();
		HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 128));
		HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hYellowBrush);
		Ellipse(hDC,
			static_cast<int>(pMap->GetConvX(m_fX) - m_realScale * ItemMaxSize / 2.f),
			static_cast<int>(m_fY - ItemMaxSize / 2.f),
			static_cast<int>(pMap->GetConvX(m_fX) + m_realScale * ItemMaxSize / 2.f),
			static_cast<int>(m_fY + ItemMaxSize / 2.f)
		);
		SelectObject(hDC, hPrevBrush);
		DeleteObject(hYellowBrush);
	}
}


Item::CCoin::~CCoin()
{
	Release();
}

void Item::CCoin::Effect(CPlayer* const pPlayer)
{
	pPlayer->AddScore(100);
}

Item::CLife::CLife(CGameWorld& world, CMap& _rMap, float fX, float fY) :
	CItem{ world, _rMap, fX, fY, CItem::Type::LIFE }
{
	m_fSpeed = 1.5f;
}
Item::CLife::~CLife()
{
	Release();
}

void Item::CLife::Effect(CPlayer* const pPlayer)
{
	//10초를 지속시켜 준다.
	pPlayer->AddScore(CPlayer::DECREASE_POINT_PER_SECOND * 10);
}

void Item::CLife::Render(const HDC& hDC)
{
	HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 128, 128));
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hRedBrush);
	auto& app = (CMainApp&)GetGameWorld();
	auto pMap = app.GetMap();
	Rectangle(hDC,
		static_cast<int>(pMap->GetConvX(m_fX) - m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY - m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(pMap->GetConvX(m_fX) + m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY + m_realScale * ItemMaxSize / 2.f)
	);
	SelectObject(hDC, hPrevBrush);
	DeleteObject(hRedBrush);
}




