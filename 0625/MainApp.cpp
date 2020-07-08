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
	m_hWindowDC = GetDC(g_hWND);
	//Double Buffering
	m_hBackbuffer = CreateCompatibleBitmap(m_hWindowDC, WINCX, WINCY);
	m_hBackbufferDC = CreateCompatibleDC(m_hWindowDC);
	SelectObject(m_hBackbufferDC, m_hBackbuffer);

	srand((time_t)time(nullptr));

	// 플레이어 생성
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer(*this, 20, WINCY >> 1);
		m_pPlayer->Ready();
	}
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 100, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 200, 100));
	m_items.push_back(CItem::CreateItem<Item::CCoin>(*this, 300, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 400, 100));
	m_items.push_back(CItem::CreateItem<Item::CLife>(*this, 500, 100));
	m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, WINCX, WINCY, nullptr);

	m_pMap = new CMap(*this, "../MapDatas/Map1.txt");
}

void CMainApp::Update(void)
{
	m_pMap->Update();
	for (auto& pItem : m_items) { pItem->Update(); }
	m_pPlayer->Update();
}

void CMainApp::LateUpdate(void)
{
	m_pMap->LateUpdate();
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }

	DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { DeleteSafe(m_pPlayer); }
}

void CMainApp::Render(void)
{
	ClearWindow();

	m_pViewSpace->Render(m_hBackbufferDC);
	m_pMap->Render(m_hBackbufferDC);
	for (auto& pItem : m_items) { pItem->Render(m_hBackbufferDC); }
	m_pPlayer->Render(m_hBackbufferDC);

	BitBlt(m_hWindowDC, 0, 0, WINCX, WINCY, m_hBackbufferDC, 0, 0, SRCCOPY);
}

void CMainApp::Release(void)
{
	DeleteSafe(m_pPlayer);
	DeleteSafe(m_pViewSpace);
	DeleteSafe(m_pMap);
	DeleteSafe(m_items);
	
	DeleteDC(m_hBackbufferDC);
	DeleteObject(m_hBackbuffer);
	ReleaseDC(g_hWND, m_hWindowDC);
}

void CMainApp::OnResize()
{
}

inline void CMainApp::ClearWindow(void)
{
	RECT rc{0,0, WINCX, WINCY};
	FillRect(m_hBackbufferDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
}
