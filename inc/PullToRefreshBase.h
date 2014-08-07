/*
 * PullToRefreshBase.h
 *
 *  Created on: Aug 6, 2014
 *      Author: seungju
 */

#ifndef PULLTOREFRESHBASE_H_
#define PULLTOREFRESHBASE_H_

#include<FBase.h>
#include<FUi.h>
#include<FUiControl.h>
#include<FUiTouch.h>
#include<FSystem.h>
#include<OnLastItemVisibleListener.h>
#include<OnPullEventListener.h>
#include<OnRefreshListener.h>
#include<OnSmoothScrollFinishedListener.h>
#include<LoadingLayout.h>
#include<RotateLoadingLayout.h>
#include<IPullToRefresh.h>
#include<Enums.h>
#include<cmath>

class SmoothScrollRunnable;
class PullToRefreshListView;

class PullToRefreshBase
	: public Tizen::Ui::Controls::ScrollPanel
	, public Tizen::Ui::TouchEventInfo
	, public IPullToRefresh
	, public Tizen::Ui::ITouchEventListener
	, public OnRefreshListener
	, public OnPullEventListener
{
public:

	class SmoothScrollRunnable
		: public Tizen::Base::Runtime::Thread
		, public OnSmoothScrollFinishedListener
		{
		private:
				 int mScrollToY;
				 int mScrollFromY;
				 long mDuration;
				 OnSmoothScrollFinishedListener *mListener;
				 PullToRefreshBase *pulltorefreshbase;
				 bool mContinueRunning = true;
				 int mStartTime = -1;
				 int mCurrentY = -1;
				 Tizen::Base::DateTime currentDateTime;

		public:
				virtual ~SmoothScrollRunnable(void){}

				virtual void onSmoothScrollFinished(void);
				result Construct(void){
					return Thread::Construct();
				}

				SmoothScrollRunnable(int fromY, int toY, long duration, OnSmoothScrollFinishedListener* listener);

				Object* Run();
				void stop();
		};

	Tizen::Ui::Controls::ListView* mRefreshableView;
	Tizen::Ui::VerticalBoxLayout* verticalLayout;
	Tizen::Ui::Controls::ScrollPanel* mRefreshableViewWrapper;

	int mLoadingAnimationStyle;

	LoadingLayout *mHeaderLayout;
	RotateLoadingLayout *rotatelayout;

	OnRefreshListener *mOnRefreshListener; //이것은 한방향으로 pull 되는거
	OnPullEventListener *mOnPullEventListener;

	SmoothScrollRunnable *mCurrentSmoothScrollRunnable;
	Tizen::Ui::ITouchEventListener *toucheventlistener;
	Tizen::Ui::TouchEventInfo touchinfo;

	static const bool DEBUG = true;
	static const bool USE_HW_LAYER = false;
	//static const Tizen::Base::String LOG_TAG = "PullToRefresh";
	static const float FRICTION = 2.0f;  //마찰력
	static const int SMOOTH_SCROLL_DURATION_MS = 200;
	static const int SMOOTH_SCROLL_LONG_DURATION_MS=325;
	static const int DEMO_SCROLL_INTERVAL = 225;

	bool mShowViewWhileRefreshing = true;
	bool mScrollingWhileRefreshingEnabled = false;
	bool mFilterTouchEvents = true;
	bool mOverScrollEnabled = true;
	bool mLayoutVisibilityChangesEnabled = true;

	void Construct(Tizen::Ui::Control& FormInstance);
	PullToRefreshBase(void);
	PullToRefreshBase(const PullToRefreshBase& rhs){};
	~PullToRefreshBase(void);

	virtual const Mode getCurrentMode();
	virtual const bool getFilterTouchEvents();
	virtual const Mode getMode();
	virtual const Tizen::Ui::Controls::ListView* getRefreshableView();
	virtual const bool getShowViewWhileRefreshing();
	const State getState();
	const bool isDisableScrollingWhileRefreshing();
	const bool isRefreshing();
	const bool isScrollingWhileRefreshingEnabled();
	const bool onInterceptTouchEvent();
	virtual const void onRefreshComplete();
	const bool onTouchEvent();
	virtual const void setFilterTouchEvents(bool filterEvents);
	virtual const void setMode(Mode mode);
	void setOnPullEventListener(OnPullEventListener listener);
	virtual const void setOnRefreshListener(OnRefreshListener listener);
	virtual const void setPullToRefreshOverScrollEnabled(bool enabled);
	virtual const void setRefreshing();
	virtual const void setRefreshing(bool doScroll);
	const void setState(State state, const bool param);
	const void setHeaderScroll(int value);
	void callRefreshListener();
	int getMaximumPullScroll();

protected:

	LoadingLayout* createLoadingLayout();
	virtual Tizen::Ui::Controls::ListView* createRefreshableView(int width, int height);
	const void disableLoadingLayoutVisibilityChanges();
	const LoadingLayout* getHeaderLayout();
	const int getHeaderSize();
	int getPullToRefreshScrollDuration();
	int getPullToRefreshScrollDurationLonger();
	Tizen::Ui::Controls::ScrollPanel* getRefreshableViewWrapper();
	virtual bool isReadyForPullEnd();
	virtual bool isReadyForPullStart();
	void onPtrRestoreInstanceState() {}
	void onPtrSaveInstanceState() {}
	void onPullToRefresh();
	void onRefreshing(const bool doScroll);
	void onReleaseToRefresh();
	void onReset();
	const void onSizeChanged(int w, int h);
	const void refreshLoadingViewsSize();
	const void refreshRefreshableViewSize(int width, int height);
	const void smoothScrollTo(int scrollValue);
	const void smoothScrollTo(int scrollValue, OnSmoothScrollFinishedListener* listener);
	const void smoothScrollToLonger(int scrollValue);
	void updateUIForMode();


private:

	int mTouchSlop;
	float mLastMotionX, mLastMotionY;
	float mInitialMotionX, mInitialMotionY;

	bool mIsBeingDragged = false;
	State mState ;
	Mode mMode ;

	Orientation mOrientation;
	Mode mCurrentMode;

	bool scrollendreach = false;
	Tizen::Base::String flickdirection;
	int scrollPosition;

	void addRefreshableView(Tizen::Ui::Controls::ListView* refreshableView);
	void init(int formwidth, int formheight);
	bool isReadyForPull();
	void pullEvent();
	const void smoothScrollTo(int scrollValue, long duration);
	const void smoothScrollTo(int newScrollValue, long duration, long delayMillis,OnSmoothScrollFinishedListener* listener);


};

#endif /* PULLTOREFRESHBASE_H_ */
