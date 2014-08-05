/*
 * OnRefreshListener2.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONREFRESHLISTENER2_H_
#define ONREFRESHLISTENER2_H_

#include <FUi.h>
#include <IPullToRefresh.h>
using namespace Tizen::Ui::Controls;

class PullToRefreshBase<ListView>;

class OnRefreshListener{
	virtual ~OnRefreshListener(void);

	/**
	 * onPullDownToRefresh will be called only when the user has Pulled from
	 * the start, and released.
	 */
	virtual void onPullDownToRefresh(PullToRefreshBase<ListView>& refreshView);

	/**
	 * onPullUpToRefresh will be called only when the user has Pulled from
	 * the end, and released.
	 */
	virtual void onPullUpToRefresh(PullToRefreshBase<ListView>& refreshView);
};


#endif /* ONREFRESHLISTENER2_H_ */
