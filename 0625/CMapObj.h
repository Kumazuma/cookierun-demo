#pragma once
#include "CObj.h"

class CMap;

class CMapObj :
	public CObj
{
public:
	CMapObj(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight);
	~CMapObj();

	virtual void LateUpdate(void);

public:
	bool IsBlockInView(void);
	RECT GetConvRect(void) const;
	float GetConvLeft(void) const;
	float GetConvRight(void) const;
	float GetConvX(void) const;

protected:
	CMap& m_rMap;
};

