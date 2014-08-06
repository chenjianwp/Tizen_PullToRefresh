/*
 * PullToRefreshBase.cpp
 *
 *  Created on: Jul 19, 2014
 *      Author: seungju
 */

#include<FlipLoadingLayout.cpp>
#include<LoadingLayout.h>
#include<RotateLoadingLayout.h>
#include<Utils.cpp>
#include<ViewCompat.cpp>
#include<IPullToRefresh.h>
#include<Enums.cpp>
#include<ListenerInterface.cpp>
#include<PullToRefreshListView.cpp>
#include<cmath>
#include<PullToRefreshBase.h>

using namespace std;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Controls::ListView;
using namespace Tizen::Ui::Graphics;
using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Ui::VerticalBoxLayout;
using namespace Tizen::Media;
using namespace Tizen::Ui::Container;
using namespace Tizen::Ui::Controls::Panel;
using namespace Tizen::Ui::Controls::IScrollEventListener;
using namespace Tizen::Ui::ITouchEventListener;
using namespace Tizen::Ui::TouchEventInfo;
using namespace Tizen::System;
using namespace Tizen::Ui::Animations;
using namespace Tizen::Base::Runtime;

//public abstract class PullToRefreshBase<T extends View> extends LinearLayout implements IPullToRefresh<T>
class PullToRefreshBase : public IPullToRefresh,public PullToRefresh
{

public:

	//Constants
	static const bool DEBUG = true;
	static const bool USE_HW_LAYER = false;
	static const string LOG_TAG = "PullToRefresh";
	static const float FRICTION = 2.0f;  //마찰력
	static const int SMOOTH_SCROLL_DURATION_MS = 200;
	static const int SMOOTH_SCROLL_LONG_DURATION_MS=325;
	static const int DEMO_SCROLL_INTERVAL = 225;

	static const string STATE_STATE = "ptr_state";
	static const string STATE_MODE = "ptr_mode";
	static const string STATE_CURRENT_MODE = "ptr_current_mode";
	static const string STATE_SCROLLING_REFRESHING_ENABLED = "ptr_disable_scrolling";
	static const string STATE_SHOW_REFRESHING_VIEW = "ptr_show_refreshing_view";
	static const string STATE_SUPER = "ptr_super";

	////////////////////////////////////////////////////////////////////////////////////
	//Constructors
	////////////////////////////////////////////////////////////////////////////////////

	//생성자 4개
	//1. context - 어플리케이션 상태에 관한 맥락, 환경을 뜻함. 객체들이 어플안에서 무슨 일을 하고 잇는지 알수 잇음
	//				프로그램의 다른 부분 (activity,package/application)에 대한 정보를 얻기 위해 이용
	//2. context, AttributeSet
	//3. context, Mode  //mode -> public static enum (in here)
	//Mode-> pull-to-refresh gesture 와 refreshing handling 에 대한 모드들 정보

	//4. context, Mode, AnimationStyle -> public static enum (in here)

	//PullToRefreshBase(Context context) {
	PullToRefreshBase() {

		 init();
	}

	PullToRefreshBase(AnimationStyle animStyle) {

		mMode = Mode::PULL_FROM_START;
		mLoadingAnimationStyle = animStyle;
		init();
	}


	//@override
	const Mode getCurrentMode()
	{
		return mCurrnetMode;
	}

	//@override
	const bool getFilterTouchEvents()
	{
		return mFilterTouchEvents;  //얘는 다른 파일에서 안보임. 더 찾아봐야 됨 true 값을 갖고 잇다.
	}

	//@Override
	//
	const Mode getMode()
	{
		return mMode;
	}

	//@override
	//
	const ListView getRefreshableView()
	{
		return mRefreshableView;
	}

	//@Override
	const bool getShowViewWhileRefreshing()
	{
		return mShowViewWhileRefreshing;
	}

	//@Override
	const State getState()
	{
		return mState;
	}

	//@override
	const bool isDisableScrollingWhileRefreshing() {
		return !isScrollingWhileRefreshingEnabled();
	}


	//@Override
	const bool isRefreshing() {
		return mState == State::REFRESHING || mState == State::MANUAL_REFRESHING;
	}

	//@Override
	const bool isScrollingWhileRefreshingEnabled() {
		return mScrollingWhileRefreshingEnabled;
	}

	//@override
	//현재 mode와 state에 따라서 touch Event 의 동작 가능 여부 같은 것을 설정하는
	//함수 같다. //able 상태 같은거 지정
	//param : MotionEvent event ->android 기능이다. 매칭되는 tizen을 찾아야 된다.
	//IscrollEventListener -> OnScrollEndReachd() 함수에서 맨 위에 다다르면 하는 걸로
	const bool onInterceptTouchEvent()
	{
		const TouchStatus action = touchinfo.GetTouchStatus();
		Point position(0,0);
		position = touchinfo.GetCurrentPosition();

		if (action == TouchStatus::TOUCH_CANCELED || flickdirection == "up") {
			mIsBeingDragged = false;
			return false;
		}

		if (flickdirection != "down" && mIsBeingDragged) {
			return true;
		}

		if(action != TouchStatus::TOUCH_MOVED && flickdirection == "down")
		{
			if (isReadyForPull()) {
				mLastMotionY = mInitialMotionY = position.y;
				mIsBeingDragged = false;
			}
		}

		if(action == TouchStatus::TOUCH_MOVED && flickdirection != "down" )
		{
			// If we're refreshing, and the flag is set. Eat all MOVE events
			if (!mScrollingWhileRefreshingEnabled && isRefreshing()) {
				return true;
			}

			if (isReadyForPull()) {
				const float y = position.y, x = position.x;
				const float diff, oppositeDiff, absDiff;

				// We need to use the correct values, based on scroll
				// direction
				diff = y - mLastMotionY;

				absDiff = abs((int)diff);

				if (absDiff > mTouchSlop && (!mFilterTouchEvents || absDiff > abs((int)oppositeDiff))) {
					if ( diff >= (float)1 && isReadyForPullStart()) {
						mLastMotionY = y;
						mIsBeingDragged = true;
					}
				}
			}
		}

		return mIsBeingDragged;
	}

	//@override
	//refreshing 다 되면 state를 reset state 로 바꿔서 정보들 새로 고치고 하는 함수 같다.
	const void onRefreshComplete()
	{
		if (isRefreshing()) {
			mState = State::RESET;
		}

	}

	//@override
	//현재 scrollingEnabled 상태같은 것들을 보고 판단하면서
	//사용자가 넣는 touchevent 가 어떤 event인지 판단하고 그것에 맞게 값을 바꿔나가는 함수인듯
	const bool onTouchEvent()
	{
		Point position(0,0);
		position = touchinfo.GetCurrentPosition();

		// If we're refreshing, and the flag is set. Eat the event
		if (!mScrollingWhileRefreshingEnabled && isRefreshing()) {
			return true;
		}

		if(scrollendreach == false){
			return false;
		}

		if(scrollendreach == true){
			if (isReadyForPull()) {
				mLastMotionY = mInitialMotionY = position.y;
				return true;
			}
		}

		return false;
	}

	//@override
	//filterEvent 받아오는 함수 객체
	//좀 더 알아봐야 한다.
	const void setFilterTouchEvents(bool filterEvents)
	{
		mFilterTouchEvents = filterEvents;
	}

	//@override
	//longClick이 가능한지 설정하는 함수 //android
	/*
	void setLongClickable()
	{
		//getRefreshableView().setLongClickable(longClickable);
	}*/

	///////////////////////////////////////////////////////////////////////////////////////??
	//@override
	//모드 값 설정하고 그것에 맞게 UI update 하는 함수 인듯
	const void setMode(Mode mode)
	{
		if (mode != mMode) {
			mMode = mode;
			updateUIForMode();
		}
	}

	//OnPullEventListener 객체 받아와서 설정하는 함수
	void setOnPullEventListener(OnPullEventListener listener)
	{
		mOnPullEventListener = listener;
	}

	//@overrride
	//OnRefrshListener 객체 받아서 설정하는 것
	const void setOnRefreshListener(OnRefreshListener listener)
	{
		mOnRefreshListener = listener;
	}

	//@override
	//mOverScrollEnabled 값 설정하는 함수
	const void setPullToRefreshOverScrollEnabled(bool enabled)
	{
		mOverScrollEnabled = enabled;
	}

	//@override
	//인자 없음
	const void setRefreshing()
	{
		setRefreshing(true);
	}

	//@override
	//bool doScroll 이라는 인자 하나 있음
	const void setRefreshing(bool doScroll)
	{
		if (!isRefreshing()) {
			setState(State::MANUAL_REFRESHING, doScroll);
		}
	}


	const void setState(State state, const bool...param)
	{
		mState = state;

		if (DEBUG) {
			Log.d(LOG_TAG, "State: " + mState.name());
		}

		switch (mState) {
			case RESET:
				onReset();
				break;
			case PULL_TO_REFRESH:
				onPullToRefresh();
				break;
			case RELEASE_TO_REFRESH:
				onReleaseToRefresh();
				break;
			case REFRESHING:
			case MANUAL_REFRESHING:
				onRefreshing(params[0]);
				break;
			case OVERSCROLLING:
				// NO-OP
				break;
		}

		// Call OnPullEventListener
		if (null != mOnPullEventListener) {
			mOnPullEventListener.onPullEvent(this, mState, mCurrentMode);
		}
	}


	// ===========================================================
	// Inner, Anonymous Classes, and Enumerations
	// ===========================================================

	/**
	 * Simple Listener that allows you to be notified when the user has scrolled
	 * to the end of the AdapterView. See (
	 * {@link PullToRefreshAdapterViewBase#setOnLastItemVisibleListener}.
	 *
	 * @author Chris Banes
	 */



	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Listener interface //////////////////////////////////////

	class OnSmoothScrollFinishedListener { //이건 내
	public:
		virtual ~OnSmoothScrollFinishedListener(){}
		void onSmoothScrollFinished()=0;
	};

	void OnSmoothScrollFinishedListener::onSmoothScrollFinished()
	{
		callRefreshListener();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	class SmoothScrollRunnable : public Thread
	{
	private:
			 const int mScrollToY;
			 const int mScrollFromY;
			 const long mDuration;
			 OnSmoothScrollFinishedListener mListener;

			 bool mContinueRunning = true;
			 int mStartTime = -1;
			 int mCurrentY = -1;
			 Tizen::Base::DateTime currentDateTime;

	public:
			SmoothScrollRunnable(void){}
			virtual ~SmoothScrollRunnable(void){}
			result Construct(void){
				return Thread::Construct();
			}

			SmoothScrollRunnable(int fromY, int toY, long duration, OnSmoothScrollFinishedListener listener) {
				mScrollFromY = fromY;
				mScrollToY = toY;
				mDuration = duration;
				mListener = listener;
			}
			//@Override
			Object* Run() {

				/**
				 * Only set mStartTime if this is the first time we're starting,
				 * else actually calculate the Y delta
				 */
				if (mStartTime == -1) {

					//mStartTime = System.currentTimeMillis();
					SystemTime::GetCurrentTime(currentDateTime);
					mStartTime = currentDateTime.GetMillisecond();
				} else {

					/**
					 * We do do all calculations in long to reduce software float
					 * calculations. We use 1000 as it gives us good accuracy and
					 * small rounding errors
					 */
					SystemTime::GetCurrentTime(currentDateTime);

					long normalizedTime = (1000 * (currentDateTime.GetMillisecond() - mStartTime)) / mDuration;
					normalizedTime = fmax(fmin(normalizedTime, 1000), 0);

					const int deltaY = round((mScrollFromY - mScrollToY) * (pow(((double)(normalizedTime / (float)1000)-1),(double)5.0)+1));
							//* mInterpolator.getInterpolation(normalizedTime / (float)1000));
					mCurrentY = mScrollFromY - deltaY;
					setHeaderScroll(mCurrentY);
				}

				// If we're not at the target Y, keep going...
				if (mContinueRunning && mScrollToY != mCurrentY) {
					//ViewCompat.postOnAnimation(PullToRefreshBase.this, this);
					this->Sleep(16);
				} else {
					if (null != mListener) {
						mListener.onSmoothScrollFinished();
					}
				}
				return null;
			}

			void stop() {
				mContinueRunning = false;
				delete this;
			}
	};


protected:
	/**
	 * Used internally for adding view. Need because we override addView to
	 * pass-through to the Refreshable View
	 */

	//param :Context context, Mode mode, TypedArray attrs
	LoadingLayout createLoadingLayout()
	{
		rotatelayout->RotateLoadingLayout::Construct();
		LoadingLayout *layout = rotatelayout;

		layout->SetShowState(false);
		return layout;
	}


	//param : context, attrs
	virtual ListView createRefreshableView();

	const void disableLoadingLayoutVisibilityChanges() {
		mLayoutVisibilityChangesEnabled = false;
	}

	const LoadingLayout getHeaderLayout() {
		return mHeaderLayout;
	}

	const int getHeaderSize() {
		return mHeaderLayout->LoadingLayout::getContentSize();
	}

	int getPullToRefreshScrollDuration() {
		return SMOOTH_SCROLL_DURATION_MS;
	}

	int getPullToRefreshScrollDurationLonger() {
		return SMOOTH_SCROLL_LONG_DURATION_MS;
	}

	Panel getRefreshableViewWrapper() {
		return mRefreshableViewWrapper;
	}


	 /**
	 * Implemented by derived class to return whether the View is in a state
	 * where the user can Pull to Refresh by scrolling from the end.
	 *
	 * @return true if the View is currently in the correct state (for example,
	 *         bottom of a ListView)
	 */

	 virtual bool isReadyForPullEnd();

	 /**
	 * Implemented by derived class to return whether the View is in a state
	 * where the user can Pull to Refresh by scrolling from the start.
	 *
	 * @return true if the View is currently the correct state (for example, top
	 *         of a ListView)
	 */
	 virtual bool isReadyForPullStart();

	 /**
	 * Called by {@link #onRestoreInstanceState(Parcelable)} so that derivative
	 * classes can handle their saved instance state.
	 *
	 * @param savedInstanceState - Bundle which contains saved instance state.
	 */
	 void onPtrRestoreInstanceState() {}

	 /**
	 * Called by {@link #onSaveInstanceState()} so that derivative classes can
	 * save their instance state.
	 *
	 * @param saveState - Bundle to be updated with saved state.
	 */
	 void onPtrSaveInstanceState() {
	}

	 /**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#PULL_TO_REFRESH} state.
	 */
	 void onPullToRefresh()
	 {
		 mHeaderLayout->LoadingLayout::pullToRefresh();
	 }

	 /**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#REFRESHING} or {@link State#MANUAL_REFRESHING} state.
	 *
	 * @param doScroll - Whether the UI should scroll for this event.
	 */
	 void onRefreshing(const bool doScroll)
	 {
		mHeaderLayout->LoadingLayout::refreshing();

		if (doScroll) {
			if (mShowViewWhileRefreshing) {

				// Call Refresh Listener when the Scroll has finished
				OnSmoothScrollFinishedListener listener = new OnSmoothScrollFinishedListener();
				listener.onSmoothScrollFinished();
				//callRefreshListener();
				smoothScrollTo(-getHeaderSize(), listener);

			} else {
				smoothScrollTo(0);
			}
		} else {
			// We're not scrolling, so just call Refresh Listener now
			callRefreshListener();
		}
	 }

	 /**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#RELEASE_TO_REFRESH} state.
	 */
	void onReleaseToRefresh() {

		mHeaderLayout->LoadingLayout::releaseToRefresh(); //얘는 loading layout 에 있는 함수이다.

		}

	/**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#RESET} state.
	 */
	void onReset() {

		mIsBeingDragged = false;
		mLayoutVisibilityChangesEnabled = true;

		mHeaderLayout->LoadingLayout::reset(); //loadinglayout 에 있는 함수

		smoothScrollTo(0);

	}

	 //@override
	 //header/ footer의 size 변경 하는 함수 .
	 const void onSizeChanged(int w, int h, int oldw, int oldh)
	 {
		 if (DEBUG) {
			Log.d(LOG_TAG, String.format("onSizeChanged. W: %d, H: %d", w, h));
		}

		 mRefreshableViewWrapper->onSizeChanged(w, h, oldw, oldh);

		// We need to update the header/footer when our size changes
		refreshLoadingViewsSize();

		// Update the Refreshable View layout
		refreshRefreshableViewSize(w, h);

	 }


	 /**
	 * Re-measure the Loading Views height, and adjust internal padding as
	 * necessary
	 */
	 //여기서 view는 listView인 것 같다.
	 ///////////////////////////////////////////////////////////////////////////////////////////////padding - spacing
	 const void refreshLoadingViewsSize()
	 {
		const int maximumPullScroll = (int)(getMaximumPullScroll() * (float)1.2);

		int pTop ;

		//원래 setHeight()는 loadinglayout에 잇는 함수이다.
		mHeaderLayout->SetSize(mHeaderLayout->GetWidth(), maximumPullScroll);
		pTop = -maximumPullScroll;

		if (DEBUG) {
			Log.d(LOG_TAG, String.format("Setting Padding. L: %d, T: %d, R: %d, B: %d", pLeft, pTop, pRight, pBottom));
		}

		verticalLayout->SetSpacing(*mRefreshableView,pTop);
	 }

	 const void refreshRefreshableViewSize(int width, int height)
	 {
		// We need to set the Height of the Refreshable View to the same as
		// this layout

		 if (mRefreshableViewWrapper->GetHeight() != height) {
			 mRefreshableViewWrapper->SetSize(mRefreshableViewWrapper->GetWidth(), height);
			 mRefreshableViewWrapper->Invalidate(true);
		}

	 }

	 /**
	 * Helper method which just calls scrollTo() in the correct scrolling
	 * direction.
	 *
	 * @param value - New Scroll value
	 */
	 const void setHeaderScroll(int value)
	 {
		// Clamp value to with pull scroll range
		const int maximumPullScroll = getMaximumPullScroll();
		value = fmin(maximumPullScroll, fmax(-maximumPullScroll, value));

		if (mLayoutVisibilityChangesEnabled) {
			if (value < 0) {
				mHeaderLayout->SetShowState(true);
				mHeaderLayout->Show();
				mHeaderLayout->Invalidate(true);
			} else {
				mHeaderLayout->SetShowState(false);
			}
		}

		scrollTo(0, value);

	 }

	 /**
	 * Smooth Scroll to position using the default duration of
	 * {@value #SMOOTH_SCROLL_DURATION_MS} ms.
	 *
	 * @param scrollValue - Position to scroll to
	 */
	 const void smoothScrollTo(int scrollValue)
	 {
		 smoothScrollTo(scrollValue, getPullToRefreshScrollDuration());
	 }

	 /**
	 * Smooth Scroll to position using the default duration of
	 * {@value #SMOOTH_SCROLL_DURATION_MS} ms.
	 *
	 * @param scrollValue - Position to scroll to
	 * @param listener - Listener for scroll
	 */
	 const void smoothScrollTo(int scrollValue, OnSmoothScrollFinishedListener listener)
	 {
		 smoothScrollTo(scrollValue, getPullToRefreshScrollDuration(), 0, listener);
	 }

	 /**
	 * Smooth Scroll to position using the longer default duration of
	 * {@value #SMOOTH_SCROLL_LONG_DURATION_MS} ms.
	 *
	 * @param scrollValue - Position to scroll to
	 */
	 const void smoothScrollToLonger(int scrollValue) {

		smoothScrollTo(scrollValue, getPullToRefreshScrollDurationLonger());
	 }

	 /**
	 * Updates the View State when the mode has been set. This does not do any
	 * checking that the mode is different to current state so always updates.
	 */
	 void updateUIForMode()
	 {
		 /*
		// Remove Header, and then add Header Loading View again if needed
		if (this == mHeaderLayout->GetLayoutN()) {

			mRefreshableViewWrapper->RemoveControl(mHeaderLayout);

		}else{

			mRefreshableViewWrapper->AddControl(mHeaderLayout);
		}*/

		// Hide Loading Views
		refreshLoadingViewsSize();

	 }


private:

	int mTouchSlop;
	float mLastMotionX, mLastMotionY;
	float mInitialMotionX, mInitialMotionY;

	bool mIsBeingDragged = false;
	State mState = State::RESET;
	Mode mMode = Mode::PULL_FROM_START;

	Orientation mOrientation;
	Mode mCurrentMode;
	ListView* mRefreshableView;
	VerticalBoxLayout* verticalLayout;
	Panel* mRefreshableViewWrapper;

	bool scrollendreach = false;
	String flickdirection;
	int scrollPosition;

	bool mShowViewWhileRefreshing = true;
	bool mScrollingWhileRefreshingEnabled = false;
	bool mFilterTouchEvents = true;
	bool mOverScrollEnabled = true;
	bool mLayoutVisibilityChangesEnabled = true;

	Interpolator mScrollAnimationInterpolator;
	AnimationStyle mLoadingAnimationStyle = AnimationStyle::ROTATE;

	LoadingLayout *mHeaderLayout;
	RotateLoadingLayout *rotatelayout;

	OnRefreshListener *mOnRefreshListener; //이것은 한방향으로 pull 되는거
	//OnRefreshListener2 mOnRefreshListener2; //이거는 아래위 양방향 pull 되는거
	OnPullEventListener *mOnPullEventListener;

	SmoothScrollRunnable mCurrentSmoothScrollRunnable;
	ITouchEventListener toucheventlistener;
	IScrollEventListener scrollevetlistener;
	TouchEventInfo touchinfo;
	TouchFlickGestureDetector flickgesture;
	AddFlickGestureEventListener flickgesturelistener;

	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void OnScrollEndReached(ListView& listview, ScrollEndEvent type){

		if(type == SCROLL_END_EVENT_END_TOP )
		{
			scrollendreach = true;
		}
	}

	void OnFlickGestureDetected(Tizen::Ui::TouchFlickGestureDetector& gestureDetector){

		FlickDirection direction = gestureDetector.GetDirection();
		switch(direction)
		{
		case FLICK_DIRECTION_UP:
			flickdirection = "up";
			break;
		case FLICK_DIRECTION_DOWN:
			flickdirection = "down";
			break;
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void addRefreshableView(ListView refreshableView)
	 {
		 mRefreshableViewWrapper->AddControl(refreshableView);
	 }

	 //현재 어떤 mOnRefreshListener가 사용중인가에 따라
	 //또 mCurrentMode에 따라 적합한 mOnRefreshListener의 onRefresh 함수를 부르기 위한 설정
	 void callRefreshListener()
	 {
		 if (null != mOnRefreshListener) {
			mOnRefreshListener.onRefresh(this);
		}
	 }


	//인자 : Context, AttributeSet
	 //AttributeSet : xml문서의 태그에서 찾아서 쓰는 tag attributeSet !! xml 문서가 있어야 된다.
	#pragma warning(disable) //@SuppressWarnings("deprecation")   //경고 안뜨게 하려고
	void init()
	{
		//orientation = Vertical 하나만 지원하기로 함

		//android layout setGravity()임 가운데 정렬 이라는 뜻임
		//setGravity(Gravity.CENTER);
		//SetHorizontalAlignment(&control, LayoutHorizontalAlignment &LAYOUT_HORIZONTAL_ALIGN_CENTER);

		//ViewConfiguration.get(context)->return configuration of context
		//					dimension of the display or density of the display..
		//getScaledTouchSlop()- 스크롤인지 아닌지 판단하는 함수- pixel 값들 리턴 받음
		//ViewConfiguration config = ViewConfiguration.get(context);
		//mTouchSlop = config.getScaledTouchSlop();
		//여기서는 mTouchSlop 값을 받아서 얼마 이상이면 mode 바꾸고 막 그런식인데 우리는 그러면 안될듯, 대신 나중에 scrollEventListener를
		//붙여서 scroll에 따른 모드를 확인받아야 될 것 같다.

		//mode는 어차피 하나 할거니까 일단 보류
		mMode = mMode.getMode();
		mLoadingAnimationStyle = mLoadingAnimationStyle.getDefault;

		mRefreshableViewWrapper = new Panel();
		mRefreshableViewWrapper->Construct(FORM_STYLE_NORMAL);

		// We need to create now layouts now
		//param: context, Mode.PULL_FROM_START, a
		mHeaderLayout = createLoadingLayout();
		verticalLayout->SetHorizontalAlignment(*mHeaderLayout, LAYOUT_HORIZONTAL_ALIGN_CENTER);
		mRefreshableViewWrapper->AddControl(mHeaderLayout);

		// Refreshable View
		// By passing the attrs, we can add ListView/GridView params via XML
		//여기서 ListView로 만들어야 되는데 attrs에서 어떤 것들을 넣어야 될지 더 자세히 알아보자
		//param: attrs
		mRefreshableView = createRefreshableView(); //PullToRefreshListView 에 있다.
		addRefreshableView(mRefreshableView);
		verticalLayout->SetHorizontalAlignment(*mRefreshableView, LAYOUT_HORIZONTAL_ALIGN_CENTER);

		mRefreshableView->AddTouchEventListener(toucheventlistener);
		mRefreshableView->AddScrollEventListener(scrollevetlistenr);

		mRefreshableView->AddGestureDetector(flickgesture);
		mRefreshableView->AddFlickGestureEventListener(flickgesturelistener);

		//얘네는 아래에 구현
		//Color background = Color(0,0,0,0);

		//얘네는 아래에 구현
		mOverScrollEnabled = true;
		mScrollingWhileRefreshingEnabled =false;

		// Finally update the UI for the modes
		updateUIForMode();
	}

	bool isReadyForPull() {

		return isReadyForPullStart();
	}

	/**
	 * Actions a Pull Event
	 *
	 * @return true if the Event has been handled, false if there has been no
	 *         change
	 */
	 void pullEvent() {

		const int newScrollValue;
		const int itemDimension;
		const float initialMotionValue, lastMotionValue;

		initialMotionValue = mInitialMotionY;
		lastMotionValue = mLastMotionY;

		newScrollValue = round(fmin(initialMotionValue - lastMotionValue, 0) / FRICTION);
		itemDimension = getHeaderSize();

		setHeaderScroll(newScrollValue);

		if (newScrollValue != 0 && !isRefreshing()) {
			float scale = abs(newScrollValue) / (float) itemDimension;

			mHeaderLayout->LoadingLayout::onPull(scale);

			if (mState != State::PULL_TO_REFRESH && itemDimension >= abs(newScrollValue)) {
				setState(State::PULL_TO_REFRESH);
			} else if (mState == State::PULL_TO_REFRESH && itemDimension < abs(newScrollValue)) {
				setState(State::RELEASE_TO_REFRESH);
			}
		}

	}


	int getMaximumPullScroll() {

		return round(mHeaderLayout->GetHeight() / FRICTION);
	}

	/**
	 * Smooth Scroll to position using the specific duration
	 *
	 * @param scrollValue - Position to scroll to
	 * @param duration - Duration of animation in milliseconds
	 */


	void OnScrollPositionChanged(ListView& listview, int ScrollPosition)
	{
		scrollPosition = ScrollPosition;
	}

	const void smoothScrollTo(int scrollValue, long duration) {
		smoothScrollTo(scrollValue, duration, 0, null);
	}


	const void smoothScrollTo(int newScrollValue, long duration, long delayMillis,
			OnSmoothScrollFinishedListener listener) {

		if (null != mCurrentSmoothScrollRunnable) {

			mCurrentSmoothScrollRunnable.stop();
		}

		const int oldScrollValue;

		oldScrollValue = scrollPosition;

		if (oldScrollValue != newScrollValue) {

			mCurrentSmoothScrollRunnable = new SmoothScrollRunnable(oldScrollValue, newScrollValue, duration, listener);

			if (delayMillis > 0) {
				mCurrentSmoothScrollRunnable.Sleep(delayMillis);
			} else {
				mCurrentSmoothScrollRunnable.Start();
			}
		}
	}


};

