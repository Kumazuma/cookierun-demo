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
	//TODO: ���� Ÿ�� ��Ÿ�� �ʿ���
	float fTimeDelta = GetGameWorld().GetTimer()->GetElapsedTimePerFrame();
	m_realScale += m_sign  * m_fSpeed * fTimeDelta; //1�ʴ� 3%�� �������� ���Ѵ�.
	//�۾����� ���ε�, �������� 0���� ��������,
	
	if (m_sign == -1 && m_realScale < -1.f)
	{
		m_sign = 1;
	}
	else if (m_sign == 1 && m_realScale > 1.f)
	{
		m_sign = -1;
	}
	m_realScale = Clamp(m_realScale, -1.f, 1.f);
	return 0;
}

void CItem::LateUpdate()
{
	auto pPlayer = ((CMainApp&)GetGameWorld()).GetPlayer();
	if (!IsValid() && pPlayer->GetRight() < GetConvLeft()) {
		SetValid(true);
	}
	if (IsCollided(pPlayer->GetRect(), GetConvRect())) {
		Effect((CPlayer*)pPlayer);
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
	//10�ʸ� ���ӽ��� �ش�.
	pPlayer->HealHP(CPlayer::DECREASE_POINT_PER_SECOND );
}

void Item::CLife::Render(const HDC& hDC)
{
	if (IsBlockInView()) {
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
}




