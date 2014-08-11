/*
 * RotateLoadingLayout.h
 *
 *  Created on: Aug 5, 2014
 *      Author: kohun
 */

#ifndef ROTATELOADINGLAYOUT_H_
#define ROTATELOADINGLAYOUT_H_

#include <LoadingLayout.h>

class RotateLoadingLayout
	: public LoadingLayout
{
public:
		RotateLoadingLayout(void);
		virtual ~RotateLoadingLayout(void);
		result Construct(void);
		void onLoadingDrawableSet(const Bitmap& bitmap);

protected:
		void onPullImpl(float scaleOfLayout);
		void refreshingImpl(void);
		void resetImpl(void);
		void resetImageRotation(void);
		void pullToRefreshImpl(void);
		void releaseToRefreshImpl(void);

private:
		Tizen::Ui::Animations::ParallelAnimationGroup* aniGroup;
		Tizen::Ui::Animations::RotateAnimation* ra;
};


#endif /* ROTATELOADINGLAYOUT_H_ */
