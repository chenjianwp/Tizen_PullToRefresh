/*
 * RotateLoadingLayout.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: kohun
 */

#include <FApp.h>
#include <FGraphics.h>
#include "RotateLoadingLayout.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Animations;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui;

RotateLoadingLayout::RotateLoadingLayout(void)
{

}

RotateLoadingLayout::~RotateLoadingLayout(void)
{

}

result
RotateLoadingLayout::Construct()
{
	result r = E_SUCCESS;

	VerticalBoxLayout mInnerLayout;
	mInnerLayout.Construct(VERTICAL_DIRECTION_DOWNWARD);

	__pLoadingLayout = new LoadingLayout();

	__pLoadingLayout->Construct(mInnerLayout, Rectangle(0,0,200, 350));
	{
		mHeaderText = new Label();
		mHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Header Text");
		mHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		//TextBox에 초기화 될 내용 HeaderText라던지 있어야됨
		//LoadingLayout에서 위치를 잡아줄 부분이 필요함.
		__pLoadingLayout->AddControl(mHeaderText);

		mSubHeaderText = new Label();
		mSubHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Sub Header Text");
		mSubHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		//TextBox에 초기화 될 내용 HeaderText라던지 있어야됨
		//LoadingLayout에서 위치를 잡아줄 부분이 필요함.
		__pLoadingLayout->AddControl(mSubHeaderText);

//		VisualElement를 이용해서 초기화 하는 부분. VisualElement 애니메이션을 사용. -> Bitmap으로 초기화하는 부분이 없는듯.
//		mHeaderImage = new VisualElement();
//		mHeaderImage->Construct();
//		mHeaderImage->SetBounds(FloatRectangle(10.0f, 10.0f, 10.0f, 10.0f));
//		mHeaderImage->SetShowState(true);
//		mHeaderImage->SetContentProvider(this);
//
//		GetVisualElement()->AttachChild(mHeaderImage);
//		mHeaderImage->InvalidateRectangle(null);


//		Label을 사용해서 BackGround Image를 넣어 초기화 하는 방법. UI Control 애니메이션을 사용.
		mHeaderImage = new Label();

		AppResource* pAppResource = Application::GetInstance()->GetAppResource();

		Bitmap* bitmap = pAppResource->GetBitmapN(L"mainmenu.png");

		mHeaderImage->Construct(Rectangle(0,0, bitmap->GetWidth(), bitmap->GetHeight()),L"");
		mHeaderImage->SetBackgroundBitmap(*bitmap);


		ra = new RotateAnimation(1, 360, 1000, ANIMATION_INTERPOLATOR_LINEAR);
		aniGroup->AddAnimation(ANIMATION_TARGET_ROTATION, *ra);
		mHeaderImage->GetControlAnimator()->SetAnimation(ANIMATION_TRIGGER_SHOW_STATE_CHANGE, aniGroup);
		//mHeaderImage->GetControlAnimator()->StartUserAnimation(aniGroup);
	}


	//attrs에 저장되있던 속성들을 비교해서 같으면 초기값 설정하는 부분.
	//setBackground()
	//setTextApperance()
	//setSubTextAppearnace()

	//setTextColor()
	//setSubTextColor()
	//getDrawable()

	//user defined 이미지가 없으면 디폴트를 불러옴

	//이미지를 세팅하고 높이/너비저장.
	//setLoadingDrawable()

	//AddControl(__pPanel);
	AddControl(__pLoadingLayout);
	return r;
}

void
RotateLoadingLayout::onLoadingDrawableSet(const Bitmap& bitmap)
{
	mHeaderImage->SetBackgroundBitmap(bitmap);
	mHeaderImage->SetBounds(Rectangle(0,0,bitmap.GetWidth(),bitmap.GetHeight()));
}

void
RotateLoadingLayout::onPullImpl(float scaleOfLayout)
{

}

void
RotateLoadingLayout::refreshingImpl(void)
{
	mHeaderImage->GetControlAnimator()->StartUserAnimation(*aniGroup);
}

void
RotateLoadingLayout::resetImpl(void)
{
	mHeaderImage->GetControlAnimator()->StopAllAnimations();
	mHeaderImage->GetControlAnimator()->StartUserAnimation(*aniGroup);
}

void
RotateLoadingLayout::pullToRefreshImpl(void)
{

}

void
RotateLoadingLayout::releaseToRefreshImpl(void)
{

}

























