#pragma once

class CGameWorld;

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

private:
	CGameWorld& m_rGameWorld;		// Block에 전달한 GameWorld 참조자 변수
	float m_fMapX;					// Map의 전체적인 X좌표 // Y 좌표는 고정
	float m_fMapSpeed;				// X축 이동에 대한 Map의 이동속도(양수 => 왼쪽으로 맵이 이동)
	float m_fFirstBlockX;			// 첫 번째 블록의 X좌표 (기준이 되는 Block이다.)
	float m_fFirstBlockY;			// 첫 번재 블록의 Y좌표 (기준이 되는 Block이다.)
	size_t m_iBlockWidth;			// Block의 너비
	size_t m_iBlockHeight;			// Block의 높이
	vector<CObj*> m_vecBlocks;		// Block들
};

