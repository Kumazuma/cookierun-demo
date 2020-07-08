#pragma once

class CTimer;

class CGameWorld abstract
{
public:
	CGameWorld();
	~CGameWorld();

public:
	virtual void Ready(void) = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(void) = 0;
	virtual void Release(void) = 0;
	virtual void Start(void) = 0;
	virtual void Stop(void) = 0;
	virtual void GameOver(void) = 0;
	virtual bool IsRunning(void) const = 0;
public:
	void RunTick(void);

public:
	CTimer* GetTimer(void) { return m_pTimer; }

private:
	CTimer* m_pTimer;
};

