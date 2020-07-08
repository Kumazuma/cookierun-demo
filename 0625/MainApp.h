#pragma once
#include "CGameWorld.h"

class CObj;
class CMap;

class CMainApp final : public CGameWorld 
{
public:
	CMainApp();
	~CMainApp();

public:
	void Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(void);
	void Release(void);
	void Start(void);
	void Stop(void);
	void GameOver(void);
	bool IsRunning(void) const;
public:
	CObj* GetPlayer(void) const { return m_pPlayer; }
	const CObj* GetViewSpace(void) const { return m_pViewSpace; }
	list<CObj*>& GetItems(void) { return m_items; }
	CMap* GetMap(void) { return m_pMap; }
	void OnResize();
private:
	inline void ClearWindow(void);

private:
	bool m_bIsRun;
	HDC m_hWindowDC;
	HBITMAP m_hBackbuffer;
	HDC m_hBackbufferDC;
	CObj* m_pPlayer; 
	CObj* m_pViewSpace;
	list<CObj*> m_items;
	CMap* m_pMap;
};

