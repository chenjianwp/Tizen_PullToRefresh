/*
 * Enums.h
 *
 *  Created on: Aug 7, 2014
 *      Author: seungju
 */

#ifndef ENUMS_H_
#define ENUMS_H_

#include <RotateLoadingLayout.h>

enum Orientation
{
	_VERTICAL = 0
};

enum AnimationSytle
{
	_ROTATE = 0
};

enum State
{
	RESET =0,
	PULL_TO_REFRESH =1,
	RELEASE_TO_REFRESH =2,
	REFRESHING =3,
	MANUAL_REFRESHING=4,
	OVERSCROLLING=5
};

enum Mode
{
	PULL_FROM_START=0
};

class Enums
{
public:
	static Orientation getOrientation();
	static AnimationSytle getAnimationStyle();
	LoadingLayout* createLoadingLayout();
	static Mode getMode() ;
};


#endif /* ENUMS_H_ */
