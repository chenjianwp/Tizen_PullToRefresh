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
	virtual ~OnPullEventListener(void);

	virtual void onPullEvent(const PullToRefreshBase& refreshView, State state, Mode direction);
};



#endif /* ONPULLEVENTLISTENER_H_ */
