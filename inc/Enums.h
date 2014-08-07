/*
 * Enums.h
 *
 *  Created on: Aug 7, 2014
 *      Author: seungju
 */

#ifndef ENUMS_H_
#define ENUMS_H_

#include <RotateLoadingLayout.h>

class Orientation
{

public:
	static const Orientation VERTICAL;
	 Orientation getOrientation();
};

class AnimationStyle {

public:

	static const AnimationStyle ROTATE;

	 AnimationStyle getDefault();
	 AnimationStyle mapIntToValue(int modeInt) ;
	LoadingLayout* createLoadingLayout();

};

class State {

public:

	static const State RESET ;
	static const State PULL_TO_REFRESH ;
	static const State RELEASE_TO_REFRESH ;
	static const State REFRESHING ;
	static const State MANUAL_REFRESHING;
	static const State OVERSCROLLING ;

	State mapIntToValue(const int stateInt);
	int getIntValue();

private:
	int mIntValue;

};

class Mode {

public:

	static const Mode PULL_FROM_START ;

	Mode getMode() ;
	int getIntValue();

private:
	int mIntValue;


};

#endif /* ENUMS_H_ */
