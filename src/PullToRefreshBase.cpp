/*
 * PullToRefreshBase.cpp
 *
 *  Created on: Jul 19, 2014
 *      Author: seungju
 */

#include<PullToRefreshBase.h>

using namespace std;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Media;
using namespace Tizen::System;
using namespace Tizen::Ui::Animations;
using namespace Tizen::Base::Runtime;


	////////////////////////////////////////////////////////////////////////////////////
	//Constructors
	////////////////////////////////////////////////////////////////////////////////////

	PullToRefreshBase::PullToRefreshBase(void)
		:mRefreshableView(0)
		,verticalLayout(0)
		,mRefreshableViewWrapper(0)
		,mHeaderLayout(0)
		,rotatelayout(0)
		,mOnRefreshListener(0)
		,mOnPullEventListener(0){}

	PullToRefreshBase::~PullToRefreshBase(void){}

	//form의 크기
	void
	PullToRefreshBase::Construct(Tizen::Ui::Controls::Form& FormInstance)
	{
		mMode = Mode(0);
		mLoadingAnimationStyle = AnimationSytle(0);

		int formheight = FormInstance.GetHeight();
		int formwidth = FormInstance.GetWidth();

		init(formwidth, formheight);
	}

	//@override
	const Mode
	PullToRefreshBase::getCurrentMode(void)
	{
		return mCurrentMode;
	}

	//@override
	const bool
	PullToRefreshBase::getFilterTouchEvents(void)
	{
		return mFilterTouchEvents;  //얘는 다른 파일에서 안보임. 더 찾아봐야 됨 true 값을 갖고 잇다.
	}

	//@Override
	//
	const Mode
	PullToRefreshBase::getMode(void)
	{
		return mMode;
	}

	//@override
	//
	const Tizen::Ui::Controls::ListView*
	PullToRefreshBase::getRefreshableView(void)
	{
		return mRefreshableView;
	}

	//@Override
	const bool
	PullToRefreshBase::getShowViewWhileRefreshing(void)
	{
		return mShowViewWhileRefreshing;
	}

	//@Override
	const State
	PullToRefreshBase::getState(void)
	{
		return mState;
	}

	//@override
	const bool
	PullToRefreshBase::isDisableScrollingWhileRefreshing(void) {
		return !isScrollingWhileRefreshingEnabled();
	}


	//@Override
	const bool
	PullToRefreshBase::isRefreshing(void) {

		return mState == State(3) || mState == State(4);
	}

	//@Override
	const bool
	PullToRefreshBase::isScrollingWhileRefreshingEnabled(void) {
		return mScrollingWhileRefreshingEnabled;
	}

	//@override
	//현재 mode와 state에 따라서 touch Event 의 동작 가능 여부 같은 것을 설정하는
	//함수 같다. //able 상태 같은거 지정
	//param : MotionEvent event ->android 기능이다. 매칭되는 tizen을 찾아야 된다.
	//IscrollEventListener -> OnScrollEndReachd() 함수에서 맨 위에 다다르면 하는 걸로
	const bool
	PullToRefreshBase::onInterceptTouchEvent(void)
	{
		Tizen::Ui::TouchStatus action = _touchinfo;
		Tizen::Graphics::Point position;

		if (action == TOUCH_CANCELED || action == TOUCH_RELEASED  ) {
			mIsBeingDragged = false;
			return false;
		}

		if (action == TOUCH_PRESSED && mIsBeingDragged) {
			return true;
		}

		if(action == TOUCH_PRESSED)
		{
			if (isReadyForPull()) {
				position = _touchposition;
				mLastMotionY = mInitialMotionY = position.y;
				mIsBeingDragged = false;
			}
		}

		if(action == TOUCH_MOVED)
		{
			// If we're refreshing, and the flag is set. Eat all MOVE events
			if (!mScrollingWhileRefreshingEnabled && isRefreshing()) {
				return true;
			}

			if (isReadyForPull()) {
				position = _touchposition;
				const float y = position.y;
				const float x = position.x;

				float diff, oppositeDiff, absDiff;

				// We need to use the correct values, based on scroll
				// direction
				diff = y - mLastMotionY;
				oppositeDiff = x - mLastMotionX;

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
	const void
	PullToRefreshBase::onRefreshComplete(void)
	{
		if (isRefreshing()) {
			setState(State(0),false);
		}

	}

	//@override
	//현재 scrollingEnabled 상태같은 것들을 보고 판단하면서
	//사용자가 넣는 touchevent 가 어떤 event인지 판단하고 그것에 맞게 값을 바꿔나가는 함수인듯
	const bool
	PullToRefreshBase::onTouchEvent(void)
	{
		Tizen::Ui::TouchStatus action = _touchinfo;
		Tizen::Graphics::Point position;

		// If we're refreshing, and the flag is set. Eat the event
		if (!mScrollingWhileRefreshingEnabled && isRefreshing()) {
			return true;
		}

		if(action==TOUCH_PRESSED){
			return false;
		}

		if(action == TOUCH_MOVED)
		{
			if(mIsBeingDragged)
			{
				position = _touchposition;
				mLastMotionY = position.y;
				mLastMotionX = position.x;
				pullEvent();
				return true;
			}
		}
		if(action == TOUCH_PRESSED)
		{
			if (isReadyForPull()) {
				position = _touchposition;
				mLastMotionY = mInitialMotionY = position.y;
				mLastMotionX = mInitialMotionX = position.x;
				return true;
			}
		}

		if(action == TOUCH_CANCELED || action == TOUCH_RELEASED)
		{
			if(mIsBeingDragged)
			{
				mIsBeingDragged = false;

				if(mState == State(2) && (null != mOnRefreshListener))
				{
					setState(State(3),true);
					return true;
				}

				if(isRefreshing())
				{
					smoothScrollTo(0);
					return true;
				}

				setState(State(0), false);
				return true;
			}
		}

		return false;
	}

	//@override
	//filterEvent 받아오는 함수 객체
	//좀 더 알아봐야 한다.
	const void
	PullToRefreshBase::setFilterTouchEvents(bool filterEvents)
	{
		mFilterTouchEvents = filterEvents;
	}

	///////////////////////////////////////////////////////////////////////////////////////??
	//@override
	//모드 값 설정하고 그것에 맞게 UI update 하는 함수 인듯
	const void
	PullToRefreshBase::setMode(Mode mode)
	{
		if (mode != mMode) {
			mMode = mode;
			updateUIForMode();
		}
	}

	//OnPullEventListener 객체 받아와서 설정하는 함수
	void
	PullToRefreshBase::setOnPullEventListener(OnPullEventListener listener)
	{
		mOnPullEventListener = &listener;
	}

	//@overrride
	//OnRefrshListener 객체 받아서 설정하는 것
	const void
	PullToRefreshBase::setOnRefreshListener(OnRefreshListener listener)
	{
		mOnRefreshListener = &listener;
	}

	//@override
	//mOverScrollEnabled 값 설정하는 함수
	const void
	PullToRefreshBase::setPullToRefreshOverScrollEnabled(bool enabled)
	{
		mOverScrollEnabled = enabled;
	}

	//@override
	//인자 없음
	const void
	PullToRefreshBase::setRefreshing(void)
	{
		setRefreshing(true);
	}

	//@override
	//bool doScroll 이라는 인자 하나 있음
	const void
	PullToRefreshBase::setRefreshing(bool doScroll)
	{
		if (!isRefreshing()) {
			setState(State(4), doScroll);
		}
	}


	const void
	PullToRefreshBase::setState(State state, const bool param)
	{
		mState = state;

		if(mState == State(0))
		{
			onReset();
		}else if(mState == State(1))
		{
			onPullToRefresh();
		}else if(mState == State(2))
		{
			onReleaseToRefresh();
		}else if(mState == State(3) || mState == State(4))
		{
			onRefreshing(param);
		}else if(mState == State(5))
		{

		}

		// Call OnPullEventListener
		if (null != mOnPullEventListener) {
			mOnPullEventListener->onPullEvent(*this);
		}
	}

	void
	PullToRefreshBase::SmoothScrollRunnable::onSmoothScrollFinished(void)
	{
		pulltorefreshbase->callRefreshListener();
	}

	PullToRefreshBase::SmoothScrollRunnable::SmoothScrollRunnable(int fromY, int toY, long duration, OnSmoothScrollFinishedListener* listener) {
		mScrollFromY = fromY;
		mScrollToY = toY;
		mDuration = duration;
		mListener = listener;
	}

	Object*
	PullToRefreshBase::SmoothScrollRunnable::Run(void) {

		/**
		 * Only set mStartTime if this is the first time we're starting,
		 * else actually calculate the Y delta
		 */
		if (mStartTime == -1) {

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
			pulltorefreshbase->setHeaderScroll(mCurrentY);
		}

		// If we're not at the target Y, keep going...
		if (mContinueRunning && mScrollToY != mCurrentY) {
			//ViewCompat.postOnAnimation(PullToRefreshBase.this, this);
			this->Sleep(16);
		} else {
			if (null != mListener) {
				mListener->onSmoothScrollFinished();
			}
		}
		return null;
	}

	void
	PullToRefreshBase::SmoothScrollRunnable::stop(void)
	{
		mContinueRunning = false;
		delete this;
	}


	/**
	 * Used internally for adding view. Need because we override addView to
	 * pass-through to the Refreshable View
	 */

	//param :Context context, Mode mode, TypedArray attrs

	LoadingLayout*
	PullToRefreshBase::createLoadingLayout(void)
	{
		rotatelayout->RotateLoadingLayout::Construct();
		LoadingLayout *layout = rotatelayout;

		layout->SetShowState(false);
		return layout;
	}

	const void
	PullToRefreshBase::disableLoadingLayoutVisibilityChanges(void) {
		mLayoutVisibilityChangesEnabled = false;
	}

	LoadingLayout*
	PullToRefreshBase::getHeaderLayout(void) {
		return mHeaderLayout;
	}

	const int
	PullToRefreshBase::getHeaderSize(void) {
		return mHeaderLayout->LoadingLayout::getContentSize();
	}

	int
	PullToRefreshBase::getPullToRefreshScrollDuration(void) {
		return SMOOTH_SCROLL_DURATION_MS;
	}

	int
	PullToRefreshBase::getPullToRefreshScrollDurationLonger(void) {
		return SMOOTH_SCROLL_LONG_DURATION_MS;
	}

	Tizen::Ui::Controls::ScrollPanel*
	PullToRefreshBase::getRefreshableViewWrapper(void) {
		return mRefreshableViewWrapper;
	}

	 /**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#PULL_TO_REFRESH} state.
	 */
	 void
	 PullToRefreshBase::onPullToRefresh(void)
	 {
		 mHeaderLayout->LoadingLayout::pullToRefresh();
	 }

	 /**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#REFRESHING} or {@link State#MANUAL_REFRESHING} state.
	 *
	 * @param doScroll - Whether the UI should scroll for this event.
	 */
	 void
	 PullToRefreshBase::onRefreshing(const bool doScroll)
	 {
		mHeaderLayout->LoadingLayout::refreshing();

		if (doScroll) {
			if (mShowViewWhileRefreshing) {

				// Call Refresh Listener when the Scroll has finished
				OnSmoothScrollFinishedListener *listener = new OnSmoothScrollFinishedListener();
				listener->onSmoothScrollFinished();
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
	void
	PullToRefreshBase::onReleaseToRefresh(void) {

		mHeaderLayout->LoadingLayout::releaseToRefresh(); //얘는 loading layout 에 있는 함수이다.

		}

	/**
	 * Called when the UI has been to be updated to be in the
	 * {@link State#RESET} state.
	 */
	void
	PullToRefreshBase::onReset(void) {

		mIsBeingDragged = false;
		mLayoutVisibilityChangesEnabled = true;

		mHeaderLayout->LoadingLayout::reset(); //loadinglayout 에 있는 함수

		smoothScrollTo(0);

	}

	 //@override
	 //header/ footer의 size 변경 하는 함수 .
	 const void
	 PullToRefreshBase::onSizeChanged(int w, int h)
	 {
		 mRefreshableViewWrapper->SetSize(w, h);

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
	 const void
	 PullToRefreshBase::refreshLoadingViewsSize(void)
	 {
		const int maximumPullScroll = (int)(getMaximumPullScroll() * (float)1.2);

		int pTop ;

		//원래 setHeight()는 loadinglayout에 잇는 함수이다.
		mHeaderLayout->SetSize(mHeaderLayout->GetWidth(), maximumPullScroll);
		pTop = -maximumPullScroll;

		verticalLayout->SetSpacing(*mRefreshableView,pTop);
	 }

	 const void
	 PullToRefreshBase::refreshRefreshableViewSize(int width, int height)
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
	 const void
	 PullToRefreshBase::setHeaderScroll(int value)
	 {
		// Clamp value to with pull scroll range
		int maximumPullScroll = getMaximumPullScroll();
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

		mRefreshableViewWrapper->SetScrollPosition(value);

	 }

	 /**
	 * Smooth Scroll to position using the default duration of
	 * {@value #SMOOTH_SCROLL_DURATION_MS} ms.
	 *
	 * @param scrollValue - Position to scroll to
	 */
	 const void
	 PullToRefreshBase::smoothScrollTo(int scrollValue)
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
	 const void
	 PullToRefreshBase::smoothScrollTo(int scrollValue, OnSmoothScrollFinishedListener* listener)
	 {
		 smoothScrollTo(scrollValue, getPullToRefreshScrollDuration(), 0, listener);
	 }

	 /**
	 * Smooth Scroll to position using the longer default duration of
	 * {@value #SMOOTH_SCROLL_LONG_DURATION_MS} ms.
	 *
	 * @param scrollValue - Position to scroll to
	 */
	 const void
	 PullToRefreshBase::smoothScrollToLonger(int scrollValue) {

		smoothScrollTo(scrollValue, getPullToRefreshScrollDurationLonger());
	 }

	 /**
	 * Updates the View State when the mode has been set. This does not do any
	 * checking that the mode is different to current state so always updates.
	 */
	 void
	 PullToRefreshBase::updateUIForMode(void)
	 {
		refreshLoadingViewsSize();
	 }


	void
	PullToRefreshBase::addRefreshableView(Tizen::Ui::Controls::ListView* refreshableView)
	 {
		 mRefreshableViewWrapper->AddControl(refreshableView);
	 }

	 //현재 어떤 mOnRefreshListener가 사용중인가에 따라
	 //또 mCurrentMode에 따라 적합한 mOnRefreshListener의 onRefresh 함수를 부르기 위한 설정
	 void
	 PullToRefreshBase::callRefreshListener(void)
	 {
		 if (null != mOnRefreshListener) {
			mOnRefreshListener->onRefresh(*this);
		}
	 }


	void
	PullToRefreshBase::init(int formwidth, int formheight)
	{
		mMode = Enums::getMode();
		mLoadingAnimationStyle = Enums::getAnimationStyle();

		mRefreshableViewWrapper = new ScrollPanel();
		mRefreshableViewWrapper->Construct(Rectangle(0,0,formwidth, formheight));

		int panelHeight;
		int panelWidth;
		panelHeight = mRefreshableViewWrapper->GetHeight();
		panelWidth = mRefreshableViewWrapper->GetWidth();

		// We need to create now layouts now
		//param: context, Mode.PULL_FROM_START, a
		mHeaderLayout = createLoadingLayout();
		verticalLayout->SetHorizontalAlignment(*mHeaderLayout, LAYOUT_HORIZONTAL_ALIGN_CENTER);
		mRefreshableViewWrapper->AddControl(mHeaderLayout);

		// Refreshable View
		// By passing the attrs, we can add ListView/GridView params via XML
		//여기서 ListView로 만들어야 되는데 attrs에서 어떤 것들을 넣어야 될지 더 자세히 알아보자
		//param: attrs
		mRefreshableView = createRefreshableView(panelWidth, panelHeight); //PullToRefreshListView 에 있다.
		addRefreshableView(mRefreshableView);
		verticalLayout->SetHorizontalAlignment(*mRefreshableView, LAYOUT_HORIZONTAL_ALIGN_CENTER);

		mRefreshableViewWrapper->AddTouchEventListener(*this);

		//얘네는 아래에 구현
		//Color background = Color(0,0,0,0);

		//얘네는 아래에 구현
		mOverScrollEnabled = true;
		mScrollingWhileRefreshingEnabled =false;

		// Finally update the UI for the modes
		updateUIForMode();
	}

	bool
	PullToRefreshBase::isReadyForPull(void) {

		return isReadyForPullStart();
	}

	/**
	 * Actions a Pull Event
	 *
	 * @return true if the Event has been handled, false if there has been no
	 *         change
	 */
	 void
	 PullToRefreshBase::pullEvent(void) {

		int newScrollValue ;
		int itemDimension;
		float initialMotionValue, lastMotionValue;

		initialMotionValue = mInitialMotionY;
		lastMotionValue = mLastMotionY;

		newScrollValue = round(fmin(initialMotionValue - lastMotionValue, 0) / FRICTION);
		itemDimension = getHeaderSize();

		setHeaderScroll(newScrollValue);

		if (newScrollValue != 0 && !isRefreshing()) {
			float scale = abs(newScrollValue) / (float) itemDimension;

			mHeaderLayout->LoadingLayout::onPull(scale);

			if (mState != State(1) && itemDimension >= abs(newScrollValue)) {
				setState(State(1),false);
			} else if (mState == State(1) && itemDimension < abs(newScrollValue)) {
				setState(State(2),false);
			}
		}

	}


	int
	PullToRefreshBase::getMaximumPullScroll(void) {

		return round(mHeaderLayout->GetHeight() / FRICTION);
	}

	/**
	 * Smooth Scroll to position using the specific duration
	 *
	 * @param scrollValue - Position to scroll to
	 * @param duration - Duration of animation in milliseconds
	 */


	const void
	PullToRefreshBase::smoothScrollTo(int scrollValue, long duration) {
		smoothScrollTo(scrollValue, duration, 0, null);
	}


	const void
	PullToRefreshBase::smoothScrollTo(int newScrollValue, long duration, long delayMillis,OnSmoothScrollFinishedListener* listener) {

		if (null != mCurrentSmoothScrollRunnable) {

			mCurrentSmoothScrollRunnable->stop();
		}

		int oldScrollValue;

		oldScrollValue = getScrollY();

		if (oldScrollValue != newScrollValue) {

			mCurrentSmoothScrollRunnable = new SmoothScrollRunnable(oldScrollValue, newScrollValue, duration, listener);

			if (delayMillis > 0) {
				mCurrentSmoothScrollRunnable->Sleep(delayMillis);
			} else {
				mCurrentSmoothScrollRunnable->Start();
			}
		}
	}

	int
	PullToRefreshBase::getScrollY(void)
	{
		int y = mRefreshableViewWrapper->GetScrollPosition();
		return y;
	}

	void
	PullToRefreshBase::OnTouchCanceled (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
	{
		_touchinfo = touchInfo.GetTouchStatus();
		_touchposition = touchInfo.GetCurrentPosition();
	}

	void
	PullToRefreshBase::OnTouchMoved (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
	{
		_touchinfo = touchInfo.GetTouchStatus();
		_touchposition = touchInfo.GetCurrentPosition();

	}
	void
	PullToRefreshBase::OnTouchPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
	{
		_touchinfo = touchInfo.GetTouchStatus();
		_touchposition = touchInfo.GetCurrentPosition();

	}

	void
	PullToRefreshBase::OnTouchReleased (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)
	{
		_touchinfo = touchInfo.GetTouchStatus();
		_touchposition = touchInfo.GetCurrentPosition();
	}



