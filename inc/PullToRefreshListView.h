/*
 * PullToRefreshListView.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef PULLTOREFRESHLISTVIEW_H_
#define PULLTOREFRESHLISTVIEW_H_

#include "RotateLoadingLayout.h"
#include "PullToRefreshBase.h"

using namespace Tizen::Ui::Controls;

class PullToRefreshListView
	: public PullToRefreshBase
{
public:
	PullToRefreshListView(void);
	virtual ~PullToRefreshListView(void);
	result Construct(void);

	virtual void onRefreshing(const boolean doScroll);
	virtual void onReset();

protected:
	Tizen::Ui::Controls::ListView createListView(int width, int height);
	virtual Tizen::Ui::Controls::ListView createRefreshableView(int width, int height);

private:
	RotateLoadingLayout mHeaderLoadingView;
	PullToRefreshListView mPullToRefreshListView;
};

#endif /* PULLTOREFRESHLISTVIEW_H_ */
