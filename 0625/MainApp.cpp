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
	m_pMap(nullptr),
	m_bIsRun{false}
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
	ClearWindow();
	srand((time_t)time(nullptr));

	HFONT hFont = CreateFontW(
		72, 0, 0, 0, 900,
		false, false, false, HANGUL_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"돋움");
	HFONT hPrevFont = (HFONT)SelectObject(m_hBackbufferDC, hFont);
	constexpr const wchar_t* const lpszGameOver = L"시작하려면 ENTER를 누르시오";
	SIZE size{};
	GetTextExtentPoint32W(m_hBackbufferDC, lpszGameOver, wcslen(lpszGameOver), &size);
	TextOutW(m_hBackbufferDC, (WINCX - size.cx) / 2, (WINCY - size.cy) / 2, lpszGameOver, wcslen(lpszGameOver));
	SelectObject(m_hBackbufferDC, hPrevFont);
	DeleteObject(hFont);
}

void CMainApp::Update(void)
{
	if (m_bIsRun)
	{
		m_pMap->Update();
		for (auto& pItem : m_items) { pItem->Update(); }
		m_pPlayer->Update();
	}
	else if(IS_PRESS_KEY_(VK_RETURN))
	{
		Start();
	}
	
}

void CMainApp::LateUpdate(void)
{
	if (m_bIsRun)
	{
		m_pMap->LateUpdate();
		DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }

		DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { DeleteSafe(m_pPlayer); }
	}
}

void CMainApp::Render(void)
{
	if (m_bIsRun)
	{
		ClearWindow();
		m_pViewSpace->Render(m_hBackbufferDC);
		m_pMap->Render(m_hBackbufferDC);
		for (auto& pItem : m_items) { pItem->Render(m_hBackbufferDC); }
		m_pPlayer->Render(m_hBackbufferDC);
	}
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

void CMainApp::Start(void)
{
	if (m_bIsRun == false)
	{
		
		DeleteSafe(m_pViewSpace);
		DeleteSafe(m_pMap);
		DeleteSafe(m_pPlayer);
		m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, WINCX, WINCY, nullptr);
		m_pMap = new CMap(*this, "../MapDatas/Map1.txt");
		// 플레이어 생성
		if (!m_pPlayer) {
			m_pPlayer = new CPlayer(*this, 120, WINCY >> 1);
			m_pPlayer->Ready();
		}
		m_bIsRun = true;
	}
}

void CMainApp::Stop(void)
{
	if (m_bIsRun = false)
	{
		m_bIsRun = true;
	}
}

void CMainApp::GameOver(void)
{
	if (m_bIsRun)
	{
		m_bIsRun = false;
		HFONT hFont = CreateFontW(
			72, 0, 0, 0, 900,
			false, false, false, HANGUL_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"돋움");
		HFONT hPrevFont = (HFONT)SelectObject(m_hBackbufferDC, hFont);
		constexpr const wchar_t* const lpszGameOver = L"G A M E  O V E R";
		constexpr const wchar_t* const lpszGameOver2 = L"다시 하려면 ENTER를 누르시오";
		SIZE size{};
		GetTextExtentPoint32W(m_hBackbufferDC, lpszGameOver, wcslen(lpszGameOver), &size);
		TextOutW(m_hBackbufferDC, (WINCX - size.cx) / 2, WINCY / 2 - size.cy /2, lpszGameOver, wcslen(lpszGameOver));
		GetTextExtentPoint32W(m_hBackbufferDC, lpszGameOver2, wcslen(lpszGameOver2), &size);
		TextOutW(m_hBackbufferDC, (WINCX - size.cx) / 2, WINCY / 2 + size.cy /2, lpszGameOver2, wcslen(lpszGameOver2));
		SelectObject(m_hBackbufferDC, hPrevFont);
		DeleteObject(hFont);
		//TODO: 만약에 게임 오버 메시지를 띄우고 싶다면 여기에서 할 것
		Stop();
	}
}

bool CMainApp::IsRunning(void) const
{
	return m_bIsRun;
}

void CMainApp::OnResize()
{

}

inline void CMainApp::ClearWindow(void)
{
	RECT rc{0,0, WINCX, WINCY};
	FillRect(m_hBackbufferDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
}
