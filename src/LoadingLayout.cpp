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
	: __pTableView(0)
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

	__pTableView = new TableView();

	__pTableView->Construct(Rectangle(0,0,200, 350),false,TABLE_VIEW_SCROLL_BAR_STYLE_THUMB);
	{
		mHeaderText = new Label();
		mHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Header Text");
		mHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		
		__pTableView->AddControl(mHeaderText);

		mSubHeaderText = new Label();
		mSubHeaderText->Construct(Rectangle(0,0,GetParent()->GetBounds().width, GetParent()->GetBounds().height), L"Sub Header Text");
		mSubHeaderText->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);
		
		__pTableView->AddControl(mSubHeaderText);
		
		mHeaderImage = new Label();

		AppResource* pAppResource = Application::GetInstance()->GetAppResource();

		Bitmap* bitmap = pAppResource->GetBitmapN(L"mainmenu.png");

		mHeaderImage->Construct(Rectangle(0,0, bitmap->GetWidth(), bitmap->GetHeight()),L"");
		mHeaderImage->SetBackgroundBitmap(*bitmap);
		mHeaderImage->GetControlAnimator()->SetAnimation(ANIMATION_TRIGGER_SHOW_STATE_CHANGE, aniGroup);
		__pTableView->AddControl(mHeaderImage);
	}

	__pTableView->SetScrollEnabled(false);
	AddControl(__pTableView);
	return r;
}

int
LoadingLayout::getContentSize()
{
	return __pTableView->GetHeight();
}

void
LoadingLayout::hideAllViews()
{
	__pTableView->SetShowState(false);
	mHeaderText->SetShowState(false);
	mSubHeaderText->SetShowState(false);
	mHeaderImage->SetShowState(false);
	__pTableView->Invalidate(true);
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


void
LoadingLayout::setSubTextColor(Color& color)
{
	if(null != mSubHeaderText)
	{
		mSubHeaderText->SetTextColor(color);
	}
}


void
LoadingLayout::setTextColor(Color& color)
{
	if(null != mHeaderText)
	{
		mHeaderText->SetTextColor(color);
	}
}












































