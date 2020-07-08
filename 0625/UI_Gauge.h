#pragma once
#include "CObj.h"
class CUI_Gauge :
	public CObj
{
public:
	CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iMaxWidth, size_t _iMaxHeight, float _fMax, float _fCurrent, float _fOffsetX = 0.f, float _fOffsetY = 0.f, UI::E_GAUGE _eDir = UI::GAUGE_HORIZONTAL);
	~CUI_Gauge();

public:
	virtual int Update(void);
	virtual void Render(const HDC& _hdc);
	virtual void Release(void) { m_pOwner = nullptr; }

public:
	void SetCurrentGauge(float _fCurrent);
	void AdjustPositionToOwner(bool _bIsDependent) { m_bIsDependent = _bIsDependent; }
	void SetMaxWidth(size_t _iMaxWidth) { m_iMaxWidth = _iMaxWidth; }
	void SetMaxHeight(size_t _iMaxHeight) { m_iMaxHeight = _iMaxHeight; }
private:
	CObj* m_pOwner;
	bool m_bIsDependent;
	RECT m_rcGauge;
	float m_fOffsetX;
	float m_fOffsetY;
	size_t m_iMaxWidth;
	size_t m_iMaxHeight;
	UI::E_GAUGE m_eDir;
	TCHAR szBuf[64];
	float m_fMax;
	float m_fCurrent;
};

