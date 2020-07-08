#pragma once

// DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END
const char ccDirKey[OBJ::DIR_END] = { 'A', 'D', 'W', 'S' };
const float cfDeltaX[OBJ::DIR_END] = { -1.f, 1.f, 0.f, 0.f };
const float cfDeltaY[OBJ::DIR_END] = { 0.f, 0.f, -1.f, 1.f };

// �÷��̾� ����
constexpr size_t ciPlayerWidth = 30;
constexpr size_t ciPlayerHeight = 50;
const float cfPlayerSpeed = 5.f;

// �Ѿ� ����
const size_t ciBulletSize = 8;
const float cfBulletSpeed = 5.f;
const float cfBulletDamage = 5.f;

// ���� ����
const size_t ciMonsterSize = 40;
const float cfMonsterSpeed = 1.f;
const float cfMonsterHp = 200.f;