/*
 * OnRefreshListener.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONREFRESHLISTENER_H_
#define ONREFRESHLISTENER_H_

class PullToRefreshBase;

class OnRefreshListener {

	public:
		virtual ~OnRefreshListener(){}
		virtual void onRefresh(PullToRefreshBase* refreshView)=0;

	};


#endif /* ONREFRESHLISTENER_H_ */
