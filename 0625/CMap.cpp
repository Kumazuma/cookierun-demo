#include "stdafx.h"
#include "CMap.h"
#include "CBlock.h"
#include "MainApp.h"

CMap::CMap(CGameWorld & _rGameWorld, const char * _szDataDirectory)
	:
	m_rGameWorld(_rGameWorld),
	m_fMapX(0.f),					// Map의 전체적인 X좌표 // Y 좌표는 고정
	m_fMapSpeed(0.f),				// X축 이동에 대한 Map의 이동속도
	m_fFirstBlockX(0.f),			// 첫 번째 블록의 X좌표 (기준이 되는 Block이다.)
	m_fFirstBlockY(WINCY >> 1),		// 첫 번재 블록의 Y좌표 (기준이 되는 Block이다.)
	m_iBlockWidth(100.f),			// Block의 너비
	m_iBlockHeight(100.f)			// Block의 높이
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
				pBlock = new CBlock(_rGameWorld, *this, fCurrentX, m_fFirstBlockY + m_iBlockHeight * iHeightIndex, m_iBlockWidth, m_iBlockHeight);
				m_vecBlocks.emplace_back(pBlock);
				fCurrentX += m_iBlockWidth;
			}
		}
	}
	else abort();
}

CMap::~CMap()
{
}

void CMap::Update(void)
{
	// TODO : 여기서 MapX를 변경한다.
	m_fMapX += m_fMapSpeed;
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Update(); } }
}

void CMap::LateUpdate(void)
{
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
