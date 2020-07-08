#pragma once
#include "CObj.h"
class CGunBarrel;
struct IPlayeMoveState;
struct IPlayerCollisionState;
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
	CUI_Gauge* GetGauge() { return m_pHpGauge; }
public:
	virtual void Ready(void)				override;
	virtual int Update(void)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc)	override;
	virtual void Release(void)				override;
	HP GetHP() const { return m_hp; }
	void HealHP(HP hp);
	void AddScore(int score) { m_nScore += score; }
	int GetScore() const { return m_nScore; }
	bool IsImmortal(void) const override { return m_bIsImmortal; }
	void SetImmortal(bool value) { m_bIsImmortal = value; }
private:
	bool m_bIsImmortal;
	IPlayeMoveState* m_pMoveState;
	IPlayerCollisionState* m_pCollisionState;
	HP m_hp;
	CUI_Gauge* m_pHpGauge;
	int m_nScore;
};

