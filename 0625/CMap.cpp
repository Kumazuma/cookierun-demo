#include "stdafx.h"
#include "CMap.h"
#include "CBlock.h"
#include "MainApp.h"

CMap::CMap(CGameWorld & _rGameWorld, const char * _szDataDirectory)
	:
	m_rGameWorld(_rGameWorld),
	m_fMapX(0.f),					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	m_fMapSpeed(0.f),				// X�� �̵��� ���� Map�� �̵��ӵ�
	m_fFirstBlockX(0.f),			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	m_fFirstBlockY(WINCY >> 1),		// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	m_iBlockWidth(100.f),			// Block�� �ʺ�
	m_iBlockHeight(100.f),			// Block�� ����
	m_pBlockUnderPlayer(nullptr)
{
	FILE* fpIn;
	errno_t err = 0;

	err = fopen_s(&fpIn, _szDataDirectory, "rt");
	if (!err) {
		fscanf_s(fpIn, "%f %f %f %f %d %d",
			&m_fMapX,
			&m_fMapSpeed,
			&m_fFirstBlockX,
			&m_fFirstBlockY,
			&m_iBlockWidth,
			&m_iBlockHeight);

		

		CObj* pBlock = nullptr;
		int iHeightIndex = 0;
		int iBlockNum = 0;
		float fCurrentX = m_fFirstBlockX;
		while (!feof(fpIn)) {
			fscanf_s(fpIn, "%d %d", &iHeightIndex, &iBlockNum);
			if (iHeightIndex == -1) {
				for (int i = 0; i < iBlockNum; i++) fCurrentX += m_iBlockWidth;
				continue;
			}
			for (int i = 0; i < iBlockNum; i++) {
				pBlock = new CBlock(_rGameWorld, *this, fCurrentX, m_fFirstBlockY - m_iBlockHeight * iHeightIndex, m_iBlockWidth, m_iBlockHeight);
				m_vecBlocks.emplace_back(pBlock);
				fCurrentX += m_iBlockWidth;
			}
		}
		m_fKillX = m_fFirstBlockX - m_iBlockWidth;
		m_fRespawnX = m_vecBlocks.back()->GetX();
	}
	else abort();
}

CMap::~CMap()
{
}

void CMap::Update(void)
{
	m_fMapX += m_fMapSpeed;
	if (m_fMapX >= GetMapDistance()) m_fMapX -= GetMapDistance();
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Update(); } }
}

void CMap::LateUpdate(void)
{
	m_pBlockUnderPlayer = nullptr;	// ���� �÷��̾� �Ʒ��� �ִ� ����� �÷��̾� �Ʒ��� ���� ���� �� �����Ƿ� �簻���� ���� nullptr�� ����.
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->LateUpdate(); } }
}

void CMap::Render(const HDC& _hdc)
{
	for (auto& pBlock : m_vecBlocks) {
		DO_IF_IS_VALID_OBJ(pBlock) {
			pBlock->Render(_hdc);
		}
	}
}

void CMap::Release(void)
{
}

float CMap::GetConvX(float _fX) const
{
	if (_fX - m_fMapX < GetKillX()) {
		return GetRespawnX() + (_fX - m_fMapX - GetKillX());
	}
	return _fX - m_fMapX;
}

float CMap::GetConvLeft(float _fX) const
{
	return GetConvX(_fX) - (m_iBlockWidth >> 1);
}

float CMap::GetConvRight(float _fX) const
{
	return GetConvX(_fX) + (m_iBlockWidth >> 1);
}

CBlock * CMap::GetBlockUnderPlayer(void) const
{
	return m_pBlockUnderPlayer;
}

void CMap::SetBlockUnderPlayer(CBlock * _pBlock)
{
	if (m_pBlockUnderPlayer) {
		// ����� �ִٸ� ���� �ֻ�� ������� �����Ѵ�.
		if (_pBlock->GetY() < m_pBlockUnderPlayer->GetY())
			m_pBlockUnderPlayer = _pBlock;
	}
	m_pBlockUnderPlayer = _pBlock;
}
