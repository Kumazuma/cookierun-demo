#include "stdafx.h"
#include "State.h"
#include "Define.h"
#include "CObj.h"
constexpr float GROUND_HEIGHT = static_cast<float>(static_cast<int>(WINCY / 1.2f));
constexpr float JUMP_SPPED = 600.f;
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
    float speed = JUMP_SPPED;
    pObject->SetSpeed(speed);
}

IPlayerState* State::Jump::Update(CObj* const pObject, float fTimedelta)
{
    float fSpeed = pObject->GetSpeed();
    float fY = pObject->GetY();
    fY -= fSpeed * fTimedelta;
    pObject->SetY(fY);
    return this;
}

void State::Jump::GravityMove(CObj* const pObject, float fTimedelta)
{
    //�÷��̾��� ��ǥ Y�� ���麸�� ���ٸ� ���ٴ����� ǫ ���� ���̴�.
    //�÷��̾��� ��ǥ�� ����� ���� ��ġ�� �����ش�.
    float fSpeed = pObject->GetSpeed();
    bool bResult = false;
    float fY = pObject->GetY();
    fSpeed += -800.0f * fTimedelta;
    pObject->SetSpeed(fSpeed);

}

bool State::Jump::IsReturn(CObj* const pObject)
{
    float fY = pObject->GetY();
    if (fY >= GROUND_HEIGHT - 100 && pObject->GetSpeed() <= 0.f)
    {
        pObject->SetY(GROUND_HEIGHT);
        return true;
    }
    return false;
}

State::SingleJump::SingleJump():
    m_bReachedTop{ false }
{

}
IPlayerState* State::SingleJump::Update(CObj* const pObject, float fTimedelta)
{
    Jump::Update(pObject, fTimedelta);
    Jump::GravityMove(pObject, fTimedelta);
    if (IsReturn(pObject))
    {
        return new Default{};
    }
    if (m_bReachedTop == false && pObject->GetSpeed() <= 0.f)
    {
        m_bReachedTop = true;
    }
    if (m_bReachedTop == false && pObject->GetSpeed() < JUMP_SPPED / 2.f && IS_PRESS_KEY_(VK_SPACE))
    {
        //TODO: double Jump
        return new DoubleJump {};
    }
    //�ƹ��� ��ȭ�� ������ �ڽ��� ��ȯ
    return this;
}

IPlayerState* State::DoubleJump::Update(CObj* const pObject, float fTimedelta)
{
    Jump::Update(pObject, fTimedelta);
    Jump::GravityMove(pObject, fTimedelta);
    if (IsReturn(pObject))
    {
        //TODO: Default State
        return new Default{};
    }
    //�ƹ��� ��ȭ�� ������ �ڽ��� ��ȯ
    return this;
}

void State::Slide::OnLoaded(CObj* const pObject)
{
    m_fPrevHeight = pObject->GetHeight();
    m_fPrevPosY = pObject->GetY();
    //�����̵� �� ���� ���̸� 1/2���� �����.
    float fY = m_fPrevPosY + m_fPrevHeight / 4;

    pObject->SetHeight(m_fPrevHeight / 2);
    pObject->SetY(fY);
}

IPlayerState* State::Slide::Update(CObj* const pObject, float fTimedelta)
{
    IPlayerState* pNextState = nullptr;
    //���� ��ɾ �켱�Ѵ�.
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
