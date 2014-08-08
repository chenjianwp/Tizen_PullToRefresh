/*
 * OnSmoothScrollFinishedListener.h
 *
 *  Created on: Jul 22, 2014
 *      Author: kohun
 */

#ifndef ONSMOOTHSCROLLFINISHEDLISTENER_H_
#define ONSMOOTHSCROLLFINISHEDLISTENER_H_


class OnSmoothScrollFinishedListener{

public:
	OnSmoothScrollFinishedListener(void);
	virtual ~OnSmoothScrollFinishedListener(void){}
	virtual void onSmoothScrollFinished(void);
};

#endif /* ONSMOOTHSCROLLFINISHEDLISTENER_H_ */
