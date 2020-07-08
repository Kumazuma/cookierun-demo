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
	CGameWorld& m_rGameWorld;		// Block�� ������ GameWorld ������ ����
	float m_fMapX;					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	float m_fMapSpeed;				// X�� �̵��� ���� Map�� �̵��ӵ�(��� => �������� ���� �̵�)
	float m_fKillX;					// Block�� ������� ����
	float m_fRespawnX;				// KillX�� ���� Block�� �������Ǵ� ������ X
	float m_fFirstBlockX;			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	float m_fFirstBlockY;			// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	size_t m_iBlockWidth;			// Block�� �ʺ�
	size_t m_iBlockHeight;			// Block�� ����

	vector<CObj*> m_vecItems;		// �����۵�
	vector<CObj*> m_vecBlocks;		// Block��
	vector<CObj*> m_vecObstacles;	// ��ֹ���

	CBlock* m_pBlockUnderPlayer;
};

