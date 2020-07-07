#pragma once

// DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END
const char ccDirKey[OBJ::DIR_END] = { 'A', 'D', 'W', 'S' };
const float cfDeltaX[OBJ::DIR_END] = { -1.f, 1.f, 0.f, 0.f };
const float cfDeltaY[OBJ::DIR_END] = { 0.f, 0.f, -1.f, 1.f };

// 플레이어 변수
const size_t ciPlayerSize = 50;
const float cfPlayerSpeed = 5.f;

// 총알 변수
const size_t ciBulletSize = 8;
const float cfBulletSpeed = 5.f;
const float cfBulletDamage = 5.f;

// 몬스터 변수
const size_t ciMonsterSize = 40;
const float cfMonsterSpeed = 1.f;
const float cfMonsterHp = 200.f;