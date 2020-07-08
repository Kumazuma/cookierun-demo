#pragma once
#include "CMapObj.h"
class CObstacle :
	public CMapObj
{
public:
	CObstacle(CGameWorld& _rGameWorld, CMap& _rMap, float _fX, float _fY, size_t _iWidth, size_t _iHeight);
	~CObstacle();

public:
	virtual void LateUpdate(void);
	virtual void Render(const HDC& _hdc);
};

