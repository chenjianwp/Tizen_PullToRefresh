/*
 * PullToRefreshListView.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: kohun
 */

#include <FApp.h>
#include <FGraphics.h>
#include "PullToRefreshListView.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Animations;
using namespace Tizen::Ui::Controls;

PullToRefreshListView::PullToRefreshListView()
	: mHeaderLoadingView(0)
	, mPullToRefreshListView(0)
{

}

PullToRefreshListView::~PullToRefreshListView()
{

}

result
PullToRefreshListView::Construct()
{
	r = E_SUCCESS;
	PullToRefreshBase::Construct();
	return r;
}

void
PullToRefreshListView::onRefreshing(const bool doScroll)
{
	PullToRefreshBase::onRefreshing(doScroll);

	LoadingLayout origLoadingView, listViewloadingView, oppositeListViewLoadingView;

	int selection, scrollToY;

	origLoadingView = getHeaderLayout();
	listViewLoadingLayout = mHeaderLoadingView;

	selection = 0;

	scrollToY = getScrollY() + getHeaderSize();

	//Hide our original Loading View
	origLoadingView.reset();
	origLoadingView.hideAllViews();

	//Show the ListView Loading View and set it to refresh.
	listViewloadingView.SetShowState(true);
	listViewloadingView.Invalidate(true);
	listViewloadingView.refreshing();

	if(doScroll)
	{
		//We need to disable the automatic visibility changes for now
		disableLoadingLayoutVisibilityChanges();

		//We scroll slightly so that the ListView's Header is at the same Y position
		//as our normal header
		setHeaderScroll(scrollToY);

		//Make sure the ListView is scrolled to show the loading header
		mRefreshableView.setSelection(selection);

		//Smooth scroll as normal
		smoothScrollTo(0);
	}
}

void
PullToRefreshListView::onReset()
{
	PullToRefreshBase::onReset();
}

Tizen::Ui::Controls::ListView
PullToRefreshListView::createListView(int width, int height)
{
	Tizen::Ui::Controls::ListView lv = new ListView();
	lv.Construct(Rectangle(0,0,width,height),SCROLL_STYLE_THUMB);

	return lv;
}

Tizen::Ui::Controls::ListView
PullToRefreshListView::createRefreshableView(int width, int height)
{
	Tizen::Ui::Controls::ListView lv = createListView(width, height);

	return lv;
}

void
PullToRefreshListView::onPullToRefresh(void)
{
	PullToRefreshBase::onPullToRefresh();
}

void
PullToRefreshListView::onReleaseToRefresh(void)
{
	PullToRefreshBase::onReleaseToRefresh();
}

void
PullToRefreshListView::onReset(void)
{
	PullToRefreshBase::onReset();
}

bool
PullToRefreshListView::isReadyForPullStart(void)
{
	return isFirstItemVisible();
}

bool
PullToRefreshListView::isFirstItemVisible(void)
{
	if(mRefreshableView->GetItemCount() == 0)
	{
		return true;
	} else {
		return false;
	}
}

















