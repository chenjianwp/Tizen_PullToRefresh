/*
 * OnRefreshListener.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONREFRESHLISTENER_H_
#define ONREFRESHLISTENER_H_

#include <FUi.h>
#include <IPullToRefresh.h>
using namespace Tizen::Ui::Controls;

class PullToRefreshBase<ListView>;

class OnRefreshListener{
	virtual void ~OnRefreshListener(void);

	virtual void onRefresh(const PullToRefreshBase<ListView>& refreshView);
};


#endif /* ONREFRESHLISTENER_H_ */
