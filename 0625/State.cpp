#include "stdafx.h"
#include "State.h"
#include "Define.h"
#include "CObj.h"
constexpr float GROUND_HEIGHT = WINCY / 1.2f;
void State::Default::OnLoaded(CObj* const pObject)
{
    pObject->SetY(GROUND_HEIGHT);
}
IPlayerState* State::Default::Update(CObj* const, float fTimedelta)
{
    if (IS_PRESS_KEY_(VK_SPACE))
    {
        return new SingleJump{};
    }
    else if (IS_PRESS_KEY_(VK_CONTROL))
    {
        return new Slide{};
    }
    return this;
}

void State::Jump::OnLoaded(CObj* const pObject)
{
    float speed =  600.f;
    pObject->SetSpeed(speed);
}

IPlayerState* State::Jump::Update(CObj* const pObject, float fTimedelta)
{
    float fSpeed = pObject->GetSpeed();
    float fY = pObject->GetY();
    fY -= fSpeed * fTimedelta;
    pObject->SetSpeed(fSpeed);
    pObject->SetY(fY);
    return this;
}

bool State::Jump::GravityMove(CObj* const pObject, float fTimedelta)
{
    //플레이어의 좌표 Y가 지면보다 낮다면 땅바닥으로 푹 꺼진 것이다.
    //플레이어의 좌표를 지면과 같은 위치로 맞춰준다.
    float fSpeed = pObject->GetSpeed();
    bool bResult = false;

    if (pObject->GetY() >= GROUND_HEIGHT)
    {
        fSpeed = 0.f;
        bResult = true;
        pObject->SetY(GROUND_HEIGHT);
    }
    else if (pObject->GetY() < GROUND_HEIGHT)
    {
        //스피드는 중력 가속도를 받아 줄어든다.
        fSpeed += -800.0f * fTimedelta;
    }
    pObject->SetSpeed(fSpeed);
    return bResult;
}

State::SingleJump::SingleJump():
    m_bReachedTop{ false },
    m_fDelay{0.1f}
{

}

void State::SingleJump::OnLoaded(CObj* const pObject)
{
    Jump::OnLoaded(pObject);
    m_fPrevSpeed = pObject->GetSpeed();
}

IPlayerState* State::SingleJump::Update(CObj* const pObject, float fTimedelta)
{
    Jump::Update(pObject, fTimedelta);
    if (Jump::GravityMove(pObject, fTimedelta))
    {
        //TODO: Default State
        return new Default{};
    }
    if (m_bReachedTop == false && m_fPrevSpeed * pObject->GetSpeed() <= 0.f)
    {
        m_bReachedTop = true;
    }
    if (m_bReachedTop == false && m_fDelay < 0.f && IS_PRESS_KEY_(VK_SPACE))
    {
        //TODO: double Jump
        return new DoubleJump {};
    }
    m_fDelay -= fTimedelta;
    m_fPrevSpeed = pObject->GetSpeed();
    //아무런 변화가 없으면 자신을 반환
    return this;
}

IPlayerState* State::DoubleJump::Update(CObj* const pObject, float fTimedelta)
{
    Jump::Update(pObject, fTimedelta);
    if (Jump::GravityMove(pObject, fTimedelta))
    {
        //TODO: Default State
        return new Default{};
    }
    //아무런 변화가 없으면 자신을 반환
    return this;
}

void State::Slide::OnLoaded(CObj* const pObject)
{
    m_fPrevHeight = pObject->GetHeight();
    m_fPrevPosY = pObject->GetY();
    //슬라이딩 할 때는 높이를 1/2으로 만든다.
    float fY = m_fPrevPosY + m_fPrevHeight / 4;

    pObject->SetHeight(m_fPrevHeight / 2);
    pObject->SetY(fY);
}

IPlayerState* State::Slide::Update(CObj* const pObject, float fTimedelta)
{
    IPlayerState* pNextState = nullptr;
    //점프 명령어를 우선한다.
    if (IS_PRESS_KEY_(VK_SPACE))
    {
        pNextState = new SingleJump{};
    }
    else if (IS_PRESS_KEY_(VK_CONTROL) == false)
    {
        pNextState = new Default{};
    }
    if (pNextState->IsNull() == false)
    {
        pObject->SetHeight(m_fPrevHeight);
        pObject->SetY(m_fPrevPosY);
        return pNextState;
    }
    return this;
}
