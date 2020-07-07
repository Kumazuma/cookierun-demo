#pragma once
#include "CGameWorld.h"

class CObj;

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
	list<CObj*>& GetMonsters(void) { return m_listMonsters; }
	list<CObj*>& GetBullets(void) { return m_listBullets; }

private:
	inline void ClearWindow(void);

private:
	HDC m_hDC;
	CObj* m_pPlayer; 
	CObj* m_pViewSpace;
	list<CObj*> m_listMonsters;
	list<CObj*> m_listBullets;
};

