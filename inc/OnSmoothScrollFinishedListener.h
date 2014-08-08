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
	virtual ~OnSmoothScrollFinishedListener(void);
	OnSmoothScrollFinishedListener(void);
	virtual void onSmoothScrollFinished(void);
};

#endif /* ONSMOOTHSCROLLFINISHEDLISTENER_H_ */
