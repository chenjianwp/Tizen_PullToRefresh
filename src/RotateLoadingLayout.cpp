/*
 * RotateLoadingLayout.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: kohun
 */

#include <FApp.h>
#include <FGraphics.h>
#include <RotateLoadingLayout.h>

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

	ra = new RotateAnimation(1, 360, 1000, ANIMATION_INTERPOLATOR_LINEAR);
	aniGroup->AddAnimation(ANIMATION_TARGET_ROTATION, *ra);

	LoadingLayout::Construct(aniGroup);

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


















