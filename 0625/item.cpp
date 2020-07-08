#include "stdafx.h"
#include "item.h"
#include "CGameWorld.h"
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
Item::CCoin::CCoin(CGameWorld& world, float fX, float fY) :
	CItem{ world, fX, fY , CItem::Type::COIN }
{
	m_fSpeed = 3.f;
}
void Item::CCoin::Render(const HDC& hDC)
{
	HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 128));
	HBRUSH hPrevBrush =(HBRUSH) SelectObject(hDC, hYellowBrush);
	Ellipse(hDC,
		static_cast<int>(m_fX - m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY - ItemMaxSize / 2.f),
		static_cast<int>(m_fX + m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY + ItemMaxSize / 2.f)
	);

	SelectObject(hDC, hPrevBrush);
	DeleteObject(hYellowBrush);
}


Item::CCoin::~CCoin()
{
	Release();
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

void Item::CLife::Render(const HDC& hDC)
{
	HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 128, 128));
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hRedBrush);
	Rectangle(hDC,
		static_cast<int>(m_fX - m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY - m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fX + m_realScale * ItemMaxSize / 2.f),
		static_cast<int>(m_fY + m_realScale * ItemMaxSize / 2.f)
	);
	SelectObject(hDC, hPrevBrush);
	DeleteObject(hRedBrush);
}




