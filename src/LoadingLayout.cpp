/*
 * LoadingLayout.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: kohun
 */

#include <FApp.h>
#include <FGraphics.h>
#include "LoadingLayout.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Animations;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui;
using namespace Tizen::Base;

LoadingLayout::LoadingLayout(void)
	: __pPanel(0)
	, mHeaderText(0)
	, mSubHeaderText(0)
	, mHeaderImage(0)
	, isAnimation(0)
{

}

LoadingLayout::~LoadingLayout(void)
{

}

result
LoadingLayout::Construct(Tizen::Ui::Animations::ParallelAnimationGroup* aniGroup)
{
	result r = E_SUCCESS;

	VerticalBoxLayout mInnerLayout;
	mInnerLayout.Construct(VERTICAL_DIRECTION_DOWNWARD);

	//__pLoadingLayout = new LoadingLayout();
	__pPanel = new Panel();

	//__pLoadingLayout->Construct(mInnerLayout, Rectangle(0,0,200, 350));
	__pPanel->Construct(mInnerLayout, Rectangle(0,0,200, 350));
	{
		mHeaderText = new Label();
		mHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Header Text");
		mHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		//TextBox에 초기화 될 내용 HeaderText라던지 있어야됨
		//LoadingLayout에서 위치를 잡아줄 부분이 필요함.
		__pPanel->AddControl(mHeaderText);

		mSubHeaderText = new Label();
		mSubHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Sub Header Text");
		mSubHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		//TextBox에 초기화 될 내용 HeaderText라던지 있어야됨
		//LoadingLayout에서 위치를 잡아줄 부분이 필요함.
		__pPanel->AddControl(mSubHeaderText);

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
		mHeaderImage->GetControlAnimator()->SetAnimation(ANIMATION_TRIGGER_SHOW_STATE_CHANGE, aniGroup);
		__pPanel->AddControl(mHeaderImage);
	}

	AddControl(__pPanel);
	return r;
}

int
LoadingLayout::getContentSize()
{
	return __pPanel->GetHeight();
}

void
LoadingLayout::hideAllViews()
{
	__pPanel->SetShowState(false);
	mHeaderText->SetShowState(false);
	mSubHeaderText->SetShowState(false);
	mHeaderImage->SetShowState(false);
	__pPanel->Invalidate(true);
}

void
LoadingLayout::onPull(float scaleOfLayout)
{
	if(!isAnimation)
	{
		onPullImpl(scaleOfLayout);
	}
}

void
LoadingLayout::pullToRefresh()
{
	if(null != mHeaderText)
	{
		if(!mPullLabel.IsEmpty())
		{
			mHeaderText->SetText(mPullLabel);
		}else{
			mHeaderText->SetText(L"Pull to refresh");
		}
	}

	pullToRefreshImpl();
}

void
LoadingLayout::refreshing()
{
	if(null != mHeaderText)
	{
		if(!mRefreshingLabel.IsEmpty())
		{
			mHeaderText->SetText(mRefreshingLabel);
		}else{
			mHeaderText->SetText(L"Loading");
		}
	}

	if(isAnimation)
	{
		//애니메이션 시작
	}else{
		refreshingImpl();
	}

	if(null != mSubHeaderText)
	{
		if( 0 == (mSubHeaderText->GetText().GetLength()))
		{
			mSubHeaderText->SetShowState(false);
		}else{
			mSubHeaderText->SetShowState(true);
		}
	}
}

void
LoadingLayout::releaseToRefresh()
{
	if(null != mHeaderText)
	{
		if(!mReleaseLabel.IsEmpty())
		{
			mHeaderText->SetText(mReleaseLabel);
		}else{
			mHeaderText->SetText(L"Release to refresh");
		}
	}

	releaseToRefreshImpl();
}

void
LoadingLayout::reset()
{
	if(null != mHeaderText)
	{
		if(!mPullLabel.IsEmpty())
		{
			mHeaderText->SetText(mPullLabel);
		}else{
			mHeaderText->SetText(L"Pull to refresh");
		}
	}
	mHeaderImage->SetShowState(true);

	if(isAnimation)
	{
		//애니메이션 스탑.
	}else{
		resetImpl();
	}

	if(null != mSubHeaderText)
	{
		if( 0 == (mSubHeaderText->GetText().GetLength()))
		{
			mSubHeaderText->SetShowState(false);
		}else{
			mSubHeaderText->SetShowState(true);
		}
	}
}

void
LoadingLayout::setLastUpdatedLabel(const String& label)
{
	mSubHeaderText->SetText(label);
}

void
LoadingLayout::setLoadingDrawable(Bitmap& imageBitmap)
{
	//image를 저장해 주고
	//애니메이션일 경우 isAnimation을 true로 만들어준다.

	onLoadingDrawableSet(imageBitmap);
}

void
LoadingLayout::setPullLabel(const String& pullLabel)
{
	mPullLabel = pullLabel;
}

void
LoadingLayout::setRefreshingLabel(const String& refreshingLabel)
{
	mRefreshingLabel = refreshingLabel;
}

void
LoadingLayout::setReleaseLabel(const String& releaseLabel)
{
	mReleaseLabel = releaseLabel;
}


/*
 * setTextTypeface
 */
void
LoadingLayout::setTextFont(Font& font)
{
	mHeaderText->SetFont(font.GetFaceName());
}

void
LoadingLayout::showInvisibleViews()
{
	if(!mHeaderText->GetShowState())
	{
		mHeaderText->SetShowState(true);
	}
	if(!mSubHeaderText->GetShowState())
	{
		mSubHeaderText->SetShowState(true);
	}
	if(!mHeaderImage->GetShowState())
	{
		mHeaderImage->SetShowState(true);
	}
	Invalidate(true);
}

// XML을 사용한 SubText의 스타일설정. 없어도 될 듯.
//void
//LoadingLayout::setSubTextAppearance(int value)
//{
//	if(null != mSubHeaderText)
//	{
//		mSubHeaderText->
//	}
//}

void
LoadingLayout::setSubTextColor(Color& color)
{
	if(null != mSubHeaderText)
	{
		mSubHeaderText->SetTextColor(color);
	}
}

// XML을 사용한 Text의 스타일 설정.
//void
//LoadingLayout::setTextAppearance(int value)
//{
//
//}

void
LoadingLayout::setTextColor(Color& color)
{
	if(null != mHeaderText)
	{
		mHeaderText->SetTextColor(color);
	}
}












































