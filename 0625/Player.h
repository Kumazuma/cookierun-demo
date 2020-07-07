#pragma once
#include "CObj.h"
class CGunBarrel;
struct IPlayerState;
class CPlayer final : public CObj
{
public:
	CPlayer(CGameWorld& _rGameWorld);
	CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth = ciPlayerSize, size_t _iHeight = ciPlayerSize, float _fSpeed = cfPlayerSpeed);
	virtual ~CPlayer();

public:
	virtual void Ready(void)				override;
	virtual int Update(void)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc)	override;
	virtual void Release(void)				override;

private:
	IPlayerState* m_pPlayerState;

};

