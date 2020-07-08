#include "stdafx.h"
#include "item.h"
#include "CGameWorld.h"
#include "Player.h"
#include "MainApp.h"
#include "CMap.h"
#include <assert.h>
constexpr float ItemMaxSize = 40.f;

CItem::CItem(CGameWorld& world, float fX, float fY, CItem::Type type):
	CObj{world, fX, fY},
	m_realScale{1.f},
	m_itemType{type},
	m_sign{-1}
{
	SetWidth(ItemMaxSize);
	SetHeight(ItemMaxSize);
}
template<>
CObj* CItem::CreateItem<Item::CCoin>(CGameWorld& world, float fX, float fY)
{
	auto* pCoin{ new Item::CCoin{world, fX, fY} };
	return pCoin;
}
template<>
CObj* CItem::CreateItem<Item::CLife>(CGameWorld& world, float fX, float fY)
{
	auto* pLife{ new Item::CLife{world, fX, fY} };
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
bool CItem::IsBlockInView(void)
{
	RECT rectViewSpace = TO_GAMEWORLD(GetGameWorld()).GetViewSpace()->GetRect();

	return IsCollided(GetConvRect(), rectViewSpace);
}
RECT CItem::GetConvRect(void) const
{
	RECT rc = {
		GetConvLeft(),
		GetTop(),
		GetConvRight(),
		GetBottom()
	};
	return rc;
}
float CItem::GetConvLeft(void) const
{
	return TO_GAMEWORLD(GetGameWorld()).GetMap()->GetConvLeft(GetX());
}
float CItem::GetConvRight(void) const
{
	return TO_GAMEWORLD(GetGameWorld()).GetMap()->GetConvRight(GetX());
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
	//�� �������� �պ��� ���� �Ÿ��� ������ ������ ���̴�.
	if (intersact)
	{
		auto p = dynamic_cast<CPlayer*>(pPlayer);
		assert(p != nullptr);
		//�������� �������� �ϰ�, �÷��̾�� ȿ���� �ο��Ѵ�.
		Effect(p);
		SetValid(false);
	}
}
Item::CCoin::CCoin(CGameWorld& world, float fX, float fY) :
	CItem{ world, fX, fY , CItem::Type::COIN }
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

Item::CLife::CLife(CGameWorld& world, float fX, float fY) :
	CItem{ world, fX, fY, CItem::Type::LIFE }
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




