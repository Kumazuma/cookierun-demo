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
	CGameWorld& m_rGameWorld;		// Block�� ������ GameWorld ������ ����
	float m_fMapX;					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	float m_fMapSpeed;				// X�� �̵��� ���� Map�� �̵��ӵ�(��� => �������� ���� �̵�)
	float m_fFirstBlockX;			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	float m_fFirstBlockY;			// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	size_t m_iBlockWidth;			// Block�� �ʺ�
	size_t m_iBlockHeight;			// Block�� ����
	vector<CObj*> m_vecBlocks;		// Block��
};

