#pragma once
#include "CObj.h"

class CGameWorld;
class CMap;

class CBlock :
	public CObj
{
public:
	CBlock(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight);
	~CBlock();

public:
	virtual void Ready(void);
	virtual int Update(void); // ������ �ڷ� �ű��.
	virtual void LateUpdate(void);
	virtual void Render(const HDC& _hdc);
	virtual void Release(void) {};

public:
	bool IsBlockInView(void);
	RECT GetConvRect(void) const;

private:
	CMap& m_rMap;
};

