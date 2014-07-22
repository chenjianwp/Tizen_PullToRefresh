/*
 * OnPullEventListener.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONPULLEVENTLISTENER_H_
#define ONPULLEVENTLISTENER_H_
#include <FUi.h>
#include <IPullToRefresh.h>
using namespace Tizen::Ui::Controls;

class PullToRefreshBase<ListView>;

class OnPullEventListener{
	virtual ~OnPullEventListener(void);

	virtual void onPullEvent(const PullToRefreshBase<ListView>& refreshView, State state, Mode direction);
};



#endif /* ONPULLEVENTLISTENER_H_ */
