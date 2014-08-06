/*
 * AnimationStyle.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: seungju
 */

#include <LoadingLayout.h>
#include<ListenerInterface.cpp>


	class Orientation
	{

	public:
		static const Orientation VERTICAL;

		static Orientation getOrientation()
		{
			return VERTICAL;
		}

	};


	class AnimationStyle {

	public:

		static const AnimationStyle ROTATE = 0x0;

		static AnimationStyle getDefault() {
			return ROTATE;
		}

		static AnimationStyle mapIntToValue(int modeInt) {
			switch (modeInt) {
				case 0x0:
				default:
					return ROTATE;
			}
		}

		LoadingLayout createLoadingLayout(Mode mode, Orientation scrollDirection) {
			switch (this) {
				case ROTATE:
				default:
					return new RotateLoadingLayout(context, mode, scrollDirection);
				}
		}

	};

	class State {

	public:

		static const State RESET =0x0;
		static const State PULL_TO_REFRESH =0x1;
		static const State RELEASE_TO_REFRESH =0x2;
		static const State REFRESHING =0x8;
		static const State MANUAL_REFRESHING =0x9;
		static const State OVERSCROLLING =0x10;

		static State mapIntToValue(const int stateInt) {

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


		int getIntValue() {
			return mIntValue;
		}

	private:
		int mIntValue;

	};

	class Mode {

	public:

		static const Mode PULL_FROM_START =0x1;

		static Mode getMode() {
			return PULL_FROM_START;
		}


		int getIntValue() {
			return mIntValue;
		}

	private:
		int mIntValue;


	};

