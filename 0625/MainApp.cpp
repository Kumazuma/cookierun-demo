#include "stdafx.h"
#include "MainApp.h"
#include "Player.h"
#include "Monster.h"
#include "CSpace.h"
#include "CMap.h"

#include "item.h"
CMainApp::CMainApp()
	:
	m_pPlayer(nullptr),
	m_pViewSpace(nullptr),
	m_pMap(nullptr)
{
}


CMainApp::~CMainApp()
{
	Release();
}

void CMainApp::Ready(void)
{
	// 얻어왔으면 반드시 지워주기!
	m_hDC = GetDC(g_hWND);

	srand((time_t)time(nullptr));

	// 플레이어 생성
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer(*this, WINCX >> 1, WINCY >> 1);
		m_pPlayer->Ready();
	}
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 100, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 200, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 300, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 400, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 500, 100));
	m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX / 1.2f), static_cast<int>(WINCY / 1.2f), nullptr);

	m_pMap = new CMap(*this, "../MapDatas/Map1.txt");
}

void CMainApp::Update(void)
{
	for (auto& pItem : m_items) { pItem->Update(); }
	m_pPlayer->Update();
	m_pMap->Update();
}

void CMainApp::LateUpdate(void)
{
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }
	m_pMap->LateUpdate();

	DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { DeleteSafe(m_pPlayer); }
}

void CMainApp::Render(void)
{
	ClearWindow();

	m_pViewSpace->Render(m_hDC);
	m_pMap->Render(m_hDC);
	for (auto& pItem : m_items) { pItem->Render(m_hDC); }
	m_pPlayer->Render(m_hDC);
}

void CMainApp::Release(void)
{
	DeleteSafe(m_pPlayer);
	DeleteSafe(m_pViewSpace);
	DeleteSafe(m_pMap);
	DeleteSafe(m_items);
	ReleaseDC(g_hWND, m_hDC);
}

inline void CMainApp::ClearWindow(void)
{
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
}
