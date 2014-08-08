/*
 * PullToRefreshListView.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef PULLTOREFRESHLISTVIEW_H_
#define PULLTOREFRESHLISTVIEW_H_

#include "PullToRefreshBase.h"

using namespace Tizen::Ui::Controls;

class PullToRefreshListView
	: public PullToRefreshBase
{
public:
	PullToRefreshListView(void);
	PullToRefreshListView(const PullToRefreshListView& rhs){};
	virtual ~PullToRefreshListView(void);
	result Construct(Tizen::Ui::Control& FormInstance);

protected:
	Tizen::Ui::Controls::ListView* createListView(int width, int height);
	virtual Tizen::Ui::Controls::ListView* createRefreshableView(int width, int height);

	void onRefreshing(const bool doScroll);
	void onReset(void);
	void onPullToRefresh(void);
	void onReleaseToRefresh(void);

	bool isReadyForPullStart(void);

private:
	bool isFirstItemVisible(void);
	RotateLoadingLayout* mHeaderLoadingView;
	PullToRefreshListView* mPullToRefreshListView;
};

#endif /* PULLTOREFRESHLISTVIEW_H_ */
