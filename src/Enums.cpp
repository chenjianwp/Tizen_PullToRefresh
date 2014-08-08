/*
 * AnimationStyle.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: seungju
 */

#include<Enums.h>

Orientation
Enums::getOrientation()
{
	enum Orientation ori;
	ori = _VERTICAL;
	return ori;
}

AnimationSytle
Enums::getAnimationStyle()
{
	enum AnimationSytle ani;
	ani = _ROTATE;
	return ani;
}

LoadingLayout*
Enums::createLoadingLayout() {

	RotateLoadingLayout *rotatelayout = new RotateLoadingLayout();
	rotatelayout->Construct();
	LoadingLayout *layout = rotatelayout;

	return layout;
}

Mode
Enums::getMode() {
	enum Mode mode;
	mode = PULL_FROM_START;
	return mode;
}




