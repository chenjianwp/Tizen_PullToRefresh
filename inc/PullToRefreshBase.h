/*
 * PullToRefreshBase.h
 *
 *  Created on: Aug 6, 2014
 *      Author: seungju
 */

#ifndef PULLTOREFRESHBASE_H_
#define PULLTOREFRESHBASE_H_

#include<OnLastItemVisibleListener.h>
#include<OnPullEventListener.h>
#include<OnRefreshListener.h>
#include<OnSmoothScrollFinishedListener.h>
#include<LoadingLayout.h>
#include<RotateLoadingLayout.h>
#include<IPullToRefresh.h>
#include<Enums.h>
#include<PullToRefreshListView.cpp>
#include<cmath>

class PullToRefreshBase
	:public Tizen::Ui::Controls::IscrollEventListener
	, public Tizen::Ui::ITouchFlickGestureEventListener
	, public Tizen::Ui::Controls::Panel
	, public IPullToRefresh
	, public Tizen::Ui::ITouchEventListener
	, public OnLastItemVisibleListener
	, public OnPullEventListener
	, public OnRefreshListener
	, public OnSmoothScrollFinishedListener
{
public:

	static const bool DEBUG = true;
	static const bool USE_HW_LAYER = false;
	static const string LOG_TAG = "PullToRefresh";
	static const float FRICTION = 2.0f;  //마찰력
	static const int SMOOTH_SCROLL_DURATION_MS = 200;
	static const int SMOOTH_SCROLL_LONG_DURATION_MS=325;
	static const int DEMO_SCROLL_INTERVAL = 225;

	PullToRefreshBase(void);
	~PullToRefreshBase(void);
	virtual void OnScrollEndReached(Tizen::Ui::Control& control, Tizen::Ui::Controls::ScrollEndEvent type);
	virtual void  OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnScrollPositionChanged(Tizen::Ui::Control& source, int scrollPosition);
	const Mode getCurrentMode();
	const bool getFilterTouchEvents();
	const Mode	Mode::getMode();
	const ListView getRefreshableView();
	const bool getShowViewWhileRefreshing();
	const State getState();
	const bool isDisableScrollingWhileRefreshing();
	const bool isRefreshing();
	const bool isScrollingWhileRefreshingEnabled();
	const bool onInterceptTouchEvent();
	const void onRefreshComplete();
	const bool onTouchEvent();
	const void setFilterTouchEvents(bool filterEvents);
	const void setMode(Mode mode);
	void setOnPullEventListener(OnPullEventListener listener);
	const void setOnRefreshListener(OnRefreshListener listener);
	const void setPullToRefreshOverScrollEnabled(bool enabled);
	const void setRefreshing();
	const void setRefreshing(bool doScroll);
	const void setState(State state, const bool...param);

protected:

	LoadingLayout createLoadingLayout();
	virtual ListView createRefreshableView(int width, int height);
	const void disableLoadingLayoutVisibilityChanges();
	const LoadingLayout getHeaderLayout();
	const int getHeaderSize();
	int getPullToRefreshScrollDuration();
	int getPullToRefreshScrollDurationLonger();
	Panel getRefreshableViewWrapper();
	virtual bool isReadyForPullEnd();
	virtual bool isReadyForPullStart();
	void onPtrRestoreInstanceState() {}
	void onPtrSaveInstanceState() {}
	void onPullToRefresh();
	void onRefreshing(const bool doScroll);
	void onReleaseToRefresh();
	void onReset();
	const void onSizeChanged(int w, int h, int oldw, int oldh);
	const void refreshLoadingViewsSize();
	const void refreshRefreshableViewSize(int width, int height);
	const void setHeaderScroll(int value);
	const void smoothScrollTo(int scrollValue);
	const void smoothScrollTo(int scrollValue, OnSmoothScrollFinishedListener listener);
	const void smoothScrollToLonger(int scrollValue);
	void updateUIForMode();


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

	AnimationStyle mLoadingAnimationStyle;

	LoadingLayout *mHeaderLayout;
	RotateLoadingLayout *rotatelayout;

	OnRefreshListener *mOnRefreshListener; //이것은 한방향으로 pull 되는거
	OnPullEventListener *mOnPullEventListener;

	SmoothScrollRunnable mCurrentSmoothScrollRunnable;
	ITouchEventListener toucheventlistener;
	IScrollEventListener scrollevetlistener;
	TouchEventInfo touchinfo;
	TouchFlickGestureDetector flickgesture;
	AddFlickGestureEventListener flickgesturelistener;


	void OnScrollEndReached(ListView& listview, ScrollEndEvent type);
	void OnFlickGestureDetected(Tizen::Ui::TouchFlickGestureDetector& gestureDetector);
	void addRefreshableView(ListView refreshableView);
	void callRefreshListener();
	void init(int formwidth, int formheight);
	bool isReadyForPull();
	void pullEvent();
	int getMaximumPullScroll();
	void OnScrollPositionChanged(ListView& listview, int ScrollPosition);
	const void smoothScrollTo(int scrollValue, long duration);
	const void smoothScrollTo(int newScrollValue, long duration, long delayMillis,OnSmoothScrollFinishedListener listener);


};

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

			SmoothScrollRunnable(int fromY, int toY, long duration, OnSmoothScrollFinishedListener listener);

			Object* Run();
			void stop();
	};


#endif /* PULLTOREFRESHBASE_H_ */
