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
	static Orientation getOrientation();
};

class AnimationStyle {

public:

	static const AnimationStyle ROTATE = 0x0;

	static AnimationStyle getDefault();
	static AnimationStyle mapIntToValue(int modeInt) ;
	LoadingLayout createLoadingLayout(Mode mode, Orientation scrollDirection);

};

class State {

public:

	static const State RESET =0x0;
	static const State PULL_TO_REFRESH =0x1;
	static const State RELEASE_TO_REFRESH =0x2;
	static const State REFRESHING =0x8;
	static const State MANUAL_REFRESHING =0x9;
	static const State OVERSCROLLING =0x10;

	static State mapIntToValue(const int stateInt);
	int getIntValue();

private:
	int mIntValue;

};

class Mode {

public:

	static const Mode PULL_FROM_START =0x1;

	static Mode getMode() ;
	int getIntValue();

private:
	int mIntValue;


};

#endif /* ENUMS_H_ */
