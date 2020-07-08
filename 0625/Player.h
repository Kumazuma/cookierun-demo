#pragma once
#include "CObj.h"
class CGunBarrel;
struct IPlayeMoveState;
class CUI_Gauge;

class CPlayer final : public CObj
{
public:
	using HP = int;
	constexpr static HP MAX_HP = static_cast<HP>(1000);
	constexpr static HP DECREASE_POINT_PER_SECOND = static_cast<HP>(50);
public:
	CPlayer(CGameWorld& _rGameWorld);
	CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth = ciPlayerWidth, size_t _iHeight = ciPlayerHeight, float _fSpeed = cfPlayerSpeed);
	virtual ~CPlayer();

public:
	virtual void Ready(void)				override;
	virtual int Update(void)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc)	override;
	virtual void Release(void)				override;
	HP GetHP() const { return m_hp; }
	void HealHP(HP hp) { m_hp += hp; }
	void AddScore(int score) { m_nScore += score; }
	int GetScore() const { return m_nScore; }
	void UseItem(const CObj* const pItem);
private:
	IPlayeMoveState* m_pMoveState;
	HP m_hp;
	CUI_Gauge* m_pHpGauge;
	int m_nScore;
};

