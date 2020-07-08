#pragma once

class CGameWorld;
class CBlock;

class CMap
{
public:
	CMap(CGameWorld& _rGameWorld, const char* _szDataDirectory);
	~CMap();

public:
	void Update(void);
	void LateUpdate(void);
	void Render(const HDC& _hdc);
	void Release(void);

public:
	float GetMapX(void) const { return m_fMapX; }
	float GetConvX(float _fX) const;
	float GetConvLeft(float _fX) const;
	float GetConvRight(float _fX) const;
	float GetMapDistance(void) const { return m_fRespawnX - m_fKillX; }
	float GetRespawnX(void) const { return m_fRespawnX; }
	float GetKillX(void) const { return m_fKillX; }
	size_t GetBlockWidth(void) const { return m_iBlockWidth; }
	size_t GetBlockHeight(void) const { return m_iBlockHeight; }
	void SlowMap(float _fSlowTime);

	void SetBlockUnderPlayer(CBlock* _pBlock);
	CBlock* GetBlockUnderPlayer(void) const;
	const vector<const CObj*>& GetObstacles() const {return (vector<const CObj*>&)m_vecObstacles;}
private:
	void UpdateSlow(void);

private:
	CGameWorld& m_rGameWorld;		// Block에 전달한 GameWorld 참조자 변수
	float m_fMapX;					// Map의 전체적인 X좌표 // Y 좌표는 고정
	float m_fMapMaxSpeed;			// X축 이동에 대한 Map의 최대이동속도(양수 => 왼쪽으로 맵이 이동)
	float m_fMapSpeed;				// 평상시 맵 속도 (평소 : 최대치, 슬로우 : 최대치 절반)
	float m_fKillX;					// Block이 사라지는 지점
	float m_fRespawnX;				// KillX를 넘은 Block이 리스폰되는 지점의 X
	float m_fFirstBlockX;			// 첫 번째 블록의 X좌표 (기준이 되는 Block이다.)
	float m_fFirstBlockY;			// 첫 번재 블록의 Y좌표 (기준이 되는 Block이다.)
	size_t m_iBlockWidth;			// Block의 너비
	size_t m_iBlockHeight;			// Block의 높이

	vector<CObj*> m_vecItems;		// 아이템들
	vector<CObj*> m_vecBlocks;		// Block들
	vector<CObj*> m_vecObstacles;	// 장애물들

	bool m_bIsSlow;
	float m_fSlowTime;
	float m_fSlowElapsedTime;
	CBlock* m_pBlockUnderPlayer;
};

