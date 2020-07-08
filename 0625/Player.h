#pragma once
#include "CObj.h"
class CGunBarrel;
struct IPlayerState;

class CPlayer final : public CObj
{
public:
	using HP = int;
	constexpr static HP MAX_HP = static_cast<HP>(10000000);
	constexpr static HP DECREASE_POINT_PER_SECOND = static_cast<HP>(10000);
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
	HP GetHP() const { return m_hp; }
	void HealHP(HP hp) { m_hp += hp; }
	void AddScore(int score) { m_nScore += score; }
	int GetScore() const { return m_nScore; }
	void UseItem(const CObj* const pItem);
private:
	IPlayerState* m_pPlayerState;
	HP m_hp;
	int m_nScore;
};

