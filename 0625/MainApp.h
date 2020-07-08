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

public:
	CObj* GetPlayer(void) const { return m_pPlayer; }
	const CObj* GetViewSpace(void) const { return m_pViewSpace; }
	list<CObj*>& GetItems(void) { return m_items; }
	CMap* GetMap() { return m_pMap; }
private:
	inline void ClearWindow(void);

private:
	HDC m_hDC;
	CObj* m_pPlayer; 
	CObj* m_pViewSpace;
	list<CObj*> m_items;
	CMap* m_pMap;
};

