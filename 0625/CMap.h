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

	void SetBlockUnderPlayer(CBlock* _pBlock);
	CBlock* GetBlockUnderPlayer(void) const;

private:
	CGameWorld& m_rGameWorld;		// Block에 전달한 GameWorld 참조자 변수
	float m_fMapX;					// Map의 전체적인 X좌표 // Y 좌표는 고정
	float m_fMapSpeed;				// X축 이동에 대한 Map의 이동속도(양수 => 왼쪽으로 맵이 이동)
	float m_fKillX;					// Block이 사라지는 지점
	float m_fRespawnX;				// KillX를 넘은 Block이 리스폰되는 지점의 X
	float m_fFirstBlockX;			// 첫 번째 블록의 X좌표 (기준이 되는 Block이다.)
	float m_fFirstBlockY;			// 첫 번재 블록의 Y좌표 (기준이 되는 Block이다.)
	size_t m_iBlockWidth;			// Block의 너비
	size_t m_iBlockHeight;			// Block의 높이
	vector<CObj*> m_vecBlocks;		// Block들

	CBlock* m_pBlockUnderPlayer;
};

