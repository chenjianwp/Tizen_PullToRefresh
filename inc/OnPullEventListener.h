/*
 * OnPullEventListener.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONPULLEVENTLISTENER_H_
#define ONPULLEVENTLISTENER_H_

class PullToRefreshBase;

class OnPullEventListener{

public:

	virtual ~OnPullEventListener(void){}

	virtual void onPullEvent(PullToRefreshBase& refreshView);
};



#endif /* ONPULLEVENTLISTENER_H_ */
