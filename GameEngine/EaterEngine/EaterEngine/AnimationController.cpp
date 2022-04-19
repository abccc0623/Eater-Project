#include "AnimationController.h"
#include "Animator.h"
#include "GameObject.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"
#include "Animation.h"
#include "TimeManager.h"

AnimationController::AnimationController()
{
	ChangeAnimation = false;

	mStop = false;
	mLoop = true;

	mNowFrame = 0;
	mNextFrame = 1;

	mPlayTime = 1.0f;
	mTime = 0.0f;
	mFrameTime = 0.0f;
}

AnimationController::~AnimationController()
{

}

void AnimationController::StartUpdate()
{
	ChangeAnime();
}

void AnimationController::Update()
{
	if (NowAnimationName != "" && mAnimation != nullptr)
	{
		Play(1, true);
	}
}

void AnimationController::SetSkin(GameObject* obj)
{
	SkinObject = obj;
}

void AnimationController::SetBoneList(std::vector<GameObject*>* m_ObjList)
{
	//게임오브젝트리스트를 애니메이터 리스트로 변경시켜줌
	int AnimatorCount = (int)m_ObjList->size();
	AnimatorList.resize(AnimatorCount);
	for (int i = 0; i < AnimatorCount; i++)
	{
		//오브젝트가 없는경우
		GameObject* obj = (*m_ObjList)[i];
		if (obj == nullptr) { continue; }

		//컨퍼넌트가 없는경우
		Animator* anime = obj->GetComponent<Animator>();
		if (anime == nullptr) { continue; }

		AnimatorList[i] = anime;
	}
}

void AnimationController::SetAnimation(Animation* animation)
{
	// 해당 Animation 설정..
	mAnimation = animation;

	// 해당 Animation Buffer 설정..
	mAnimationData = new AnimationData();

	// 해당 Skin Mesh에 Animation Data 삽입..
	SkinObject->OneMeshData->Animation_Data = mAnimationData;
	SkinObject->OneMeshData->Animation_Buffer = mAnimation->m_AnimationBuffer;

	NowAnimationBuffer = mAnimation->m_AnimationBuffer;
}

void AnimationController::ChangeAnime()
{
	if (ChangeAnimation == true)
	{
		NowAnimation = mAnimation->m_AnimationData->AnimList[NowAnimationName];

		mAnimationData->PrevAnimationIndex = NowAnimationBuffer->AnimationOffset[NowAnimation->m_Index];
		mAnimationData->NextAnimationIndex = NowAnimationBuffer->AnimationOffset[NowAnimation->m_Index];

		// Animation Data 초기화..
		Reset();

		//본의 애니메이션을 넣어준다
		int Count = (int)AnimatorList.size();
		for (int i = 0; i < Count; i++)
		{
			if (AnimatorList[i] == nullptr) { continue; }

			AnimatorList[i]->SetAnimation(NowAnimation->m_AnimationList[i]);
		}

		ChangeAnimation = false;
	}
}

void AnimationController::AnimationFrameIndex()
{
	if (NowAnimation == nullptr) return;

	if (mStop == false)
	{
		mTime += (mTimeManager->DeltaTime() * mPlayTime);
		mFrameTime += (mTimeManager->DeltaTime() * mPlayTime) / NowAnimation->m_TicksPerFrame;

		mNowFrame = (int)(mTime / NowAnimation->m_TicksPerFrame);

		if (mPrevFrame != mNowFrame)
		{
			mPrevFrame = mNowFrame;
			mFrameTime = 0.0f;

			if (mPrevFrame >= NowAnimation->m_EndFrame)
			{
				if (mLoop == false)
				{
					mPrevFrame = NowAnimation->m_EndFrame;
					mTime = 0.0f;
				}
				else
				{
					mPrevFrame = 0;
					mTime = 0.0f;
				}
			}

			mNextFrame = mPrevFrame + 1;

			if (mNextFrame >= NowAnimation->m_EndFrame)
			{
				if (mLoop == false)
				{
					mNextFrame = NowAnimation->m_EndFrame;
				}
				else
				{
					mNextFrame = 0;
				}
			}

			// 해당 Animation Frame 설정..
			mAnimationData->PrevFrameIndex = mPrevFrame * NowAnimationBuffer->FrameOffset;
			mAnimationData->NextFrameIndex = mNextFrame * NowAnimationBuffer->FrameOffset;
		}

		// 해당 Animation Frame Time 설정..
		mAnimationData->FrameTime = mFrameTime;
	}
}

void AnimationController::Reset()
{
	mPrevFrame = 0;
	mNextFrame = 1;

	mTime = 0.0f;
	mFrameTime = 0.0f;

	mAnimationData->PrevFrameIndex = mPrevFrame * NowAnimationBuffer->FrameOffset;
	mAnimationData->NextFrameIndex = mNextFrame * NowAnimationBuffer->FrameOffset;
}

void AnimationController::Choice(std::string Name)
{
	//나의 애니메이션 리스트에서 선택한 애니메이션을 본에게 넘겨준다
	if (NowAnimationName != Name)
	{
		ChangeAnimation = true;
		NowAnimationName = Name;
	}
}

void AnimationController::Play(float Speed, bool Loop)
{
	// 현재 재생중일 경우에만 Play..
	if (mStop) return;

	// 현재 Animation Frame 재설정..
	AnimationFrameIndex();

	//Animator 컨퍼넌트들의 Play함수를 실행시킨다
	int Size = AnimatorList.size();
	for (int i = 0; i <Size; i++)
	{
		if (AnimatorList[i] == nullptr) { continue;}
		
		AnimatorList[i]->Play(mPrevFrame, mNextFrame, mFrameTime, Loop);
	} 
}

void AnimationController::Stop()
{
	mStop = true;
}

int AnimationController::GetNowFrame()
{
	if (NowAnimation == nullptr)
	{
		return 0;
	}

	return mNowFrame;
}

int AnimationController::GetEndFrame()
{
	if (NowAnimation == nullptr)
	{
		return 0;
	}

	return NowAnimation->m_EndFrame - 1;
}

int AnimationController::GetAnimationCount()
{
	if (mAnimation == nullptr)
	{
		return 0;
	}
	else
	{
		return (int)mAnimation->m_AnimationData->AnimList.size();
	}
}

void AnimationController::GetAnimationList(std::vector<std::string>* NameList)
{
	if (mAnimation == nullptr) { return; }
	auto it_Start = mAnimation->m_AnimationData->AnimList.begin();
	for (it_Start; it_Start != mAnimation->m_AnimationData->AnimList.end(); it_Start++)
	{
		NameList->push_back(it_Start->first);
	}
}

std::string AnimationController::GetNowAnimationName()
{
	return NowAnimationName;
}
