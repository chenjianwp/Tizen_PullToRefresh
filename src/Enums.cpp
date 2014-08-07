/*
 * AnimationStyle.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: seungju
 */

#include<Enums.h>

static Orientation
Orientation::getOrientation()
{
	return VERTICAL;
}

static AnimationStyle
AnimationStyle::getDefault()
{
	return ROTATE;
}

static AnimationStyle
AnimationStyle::mapIntToValue(int modeInt) {
	switch (modeInt) {
		case 0x0:
		default:
			return ROTATE;
	}
}

LoadingLayout
AnimationStyle::createLoadingLayout(Mode mode, Orientation scrollDirection) {
	switch (this) {
		case ROTATE:
		default:
			return new RotateLoadingLayout(context, mode, scrollDirection);
		}
}

static State
State::mapIntToValue(const int stateInt) {

		switch (stateInt) {
			case 0x0:
			default:
				mIntValue = stateInt;
				return RESET;
			case 0x1:
				mIntValue = stateInt;
				return PULL_TO_REFRESH;
			case 0x2:
				mIntValue = stateInt;
				return RELEASE_TO_REFRESH;
			case 0x8:
				mIntValue = stateInt;
				return REFRESHING;
			case 0x9:
				mIntValue = stateInt;
				return MANUAL_REFRESHING;
			case 0x10:
				mIntValue = stateInt;
				return OVERSCROLLING;
		}
}

int
State::getIntValue() {
		return mIntValue;
}

static Mode
Mode::getMode() {
	return PULL_FROM_START;
}

int
Mode::getIntValue() {
	return mIntValue;
}


