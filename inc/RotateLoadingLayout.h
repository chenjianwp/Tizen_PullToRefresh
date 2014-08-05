/*
 * RotateLoadingLayout.h
 *
 *  Created on: Aug 5, 2014
 *      Author: kohun
 */

#ifndef ROTATELOADINGLAYOUT_H_
#define ROTATELOADINGLAYOUT_H_

#include "LoadingLayout.h"

class RotateLoadingLayout
	: public LoadingLayout
{
public:
		RotateLoadingLayout(void);
		~RotateLoadingLayout(void);
		result Construct(void);
		void onLoadingDrawableSet(const Bitmap& bitmap);

protected:
		void onPullImpl(float scaleOfLayout);
		void refreshingImpl();
		void resetImpl();
		void resetImageRotation();
		void pullToRefreshImpl();
		void releaseToRefreshImpl();

private:
		Tizen::Ui::Animations::ParallelAnimationGroup* aniGroup;
		Tizen::Ui::Animations::RotateAnimation* ra;
};


#endif /* ROTATELOADINGLAYOUT_H_ */
