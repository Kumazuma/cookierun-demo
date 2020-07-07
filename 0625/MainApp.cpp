#include "stdafx.h"
#include "MainApp.h"
#include "Player.h"
#include "Monster.h"
#include "CSpace.h"
#include "item.h"
CMainApp::CMainApp()
	:
	m_pPlayer(nullptr),
	m_pViewSpace(nullptr)
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

	if (!m_pPlayer) {
		m_pPlayer = new CPlayer(*this, WINCX >> 1, WINCY >> 1);
		m_pPlayer->Ready();
	}
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 100, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 200, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 300, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 400, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 500, 100));
	m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX / 1.2f), static_cast<int>(WINCY / 1.2f), &m_listBullets);

	//CObj* pMonster = nullptr;
	//for (int i = 0; i < 4; i++) {
	//	pMonster = new CMonster(
	//		*this,
	//		GetNumberMinBetweenMax(m_pViewSpace->GetLeft(), m_pViewSpace->GetRight()),
	//		GetNumberMinBetweenMax(m_pViewSpace->GetTop(), m_pViewSpace->GetBottom()));
	//	m_listMonsters.emplace_back(pMonster);
	//}
}

void CMainApp::Update(void)
{
	for (auto& pBullet : m_listBullets) { pBullet->Update(); }
	for (auto& pMonster : m_listMonsters) { pMonster->Update(); }
	for (auto& pItem : m_items) { pItem->Update(); }
	m_pPlayer->Update();
}

void CMainApp::LateUpdate(void)
{
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->LateUpdate(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->LateUpdate(); } }
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }


	CollectGarbageObjects(m_listBullets);
	CollectGarbageObjects(m_listMonsters);
	DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { DeleteSafe(m_pPlayer); }
}

void CMainApp::Render(void)
{
	ClearWindow();
	//InvalidateRect(g_hWND, nullptr, true);

	m_pViewSpace->Render(m_hDC);
	for (auto& pMonster : m_listMonsters) { pMonster->Render(m_hDC); }
	for (auto& pBullet : m_listBullets) { pBullet->Render(m_hDC); }
	for (auto& pItem : m_items) { pItem->Render(m_hDC); }
	m_pPlayer->Render(m_hDC);
}

void CMainApp::Release(void)
{
	DeleteSafe(m_pPlayer);
	DeleteSafe(m_pViewSpace);
	DeleteSafe(m_listBullets);
	DeleteSafe(m_listMonsters);
	DeleteSafe(m_items);
	ReleaseDC(g_hWND, m_hDC);
}

inline void CMainApp::ClearWindow(void)
{
	//IntersectRect()
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);
}
