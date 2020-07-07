#pragma once
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
};

