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
				 bool mContinueRunning;
				 int mStartTime;
				 int mCurrentY;
				 OnSmoothScrollFinishedListener *mListener;
				 PullToRefreshBase *pulltorefreshbase;
				 Tizen::Base::DateTime currentDateTime;

		public:
				~SmoothScrollRunnable(void){}

				void onSmoothScrollFinished(void);

				result Construct(void){
					return Thread::Construct();
				}

				SmoothScrollRunnable(int fromY, int toY, long duration, OnSmoothScrollFinishedListener* listener);

				Object* Run(void);
				void stop(void);
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
	Tizen::Ui::TouchStatus _touchinfo;
	Tizen::Graphics::Point _touchposition;

	static const bool DEBUG = true;
	static const bool USE_HW_LAYER = false;
	//static const Tizen::Base::String LOG_TAG = "PullToRefresh";
	static const float FRICTION = 2.0f;  //마찰력
	static const int SMOOTH_SCROLL_DURATION_MS = 200;
	static const int SMOOTH_SCROLL_LONG_DURATION_MS=325;
	static const int DEMO_SCROLL_INTERVAL = 225;

	bool mShowViewWhileRefreshing ;
	bool mScrollingWhileRefreshingEnabled ;
	bool mFilterTouchEvents ;
	bool mOverScrollEnabled ;
	bool mLayoutVisibilityChangesEnabled;

	bool mIsBeingDragged ;

	//Life-Cycle
	result Construct(Tizen::Ui::Controls::Form& FormInstance);
	PullToRefreshBase(void);
	PullToRefreshBase(const PullToRefreshBase& rhs){};
	~PullToRefreshBase(void);

	//IPullToRefresh
	virtual const Mode getCurrentMode(void);
	virtual const bool getFilterTouchEvents(void);
	virtual const Mode getMode(void);
	virtual const Tizen::Ui::Controls::ListView* getRefreshableView(void);
	virtual const bool getShowViewWhileRefreshing(void);
	virtual const void onRefreshComplete(void);
	virtual const void setFilterTouchEvents(bool filterEvents);
	virtual const void setRefreshing(void);
	virtual const void setRefreshing(bool doScroll);
	virtual const void setOnRefreshListener(OnRefreshListener* listener);
	virtual const void setPullToRefreshOverScrollEnabled(bool enabled);
	virtual const void setMode(Mode mode);
	void setOnPullEventListener(OnPullEventListener listener);

	//ITouchEventListener
	virtual void OnTouchCanceled (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo);
	virtual void OnTouchFocusIn (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void OnTouchFocusOut (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo){}
	virtual void OnTouchMoved (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo);
	virtual void OnTouchPressed (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo);
	virtual void OnTouchReleased (const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo);

	const State getState(void);
	const bool isDisableScrollingWhileRefreshing(void);
	const bool isRefreshing(void);
	const bool isScrollingWhileRefreshingEnabled(void);
	const bool onInterceptTouchEvent(void);
	const bool onTouchEvent(void);
	const void setState(State state, const bool param);
	const void setHeaderScroll(int value);
	void callRefreshListener(void);
	int getMaximumPullScroll(void);
	int getScrollY(void);

protected:

	LoadingLayout* createLoadingLayout(void);
	virtual Tizen::Ui::Controls::ListView* createRefreshableView(int width, int height) = 0;
	const void disableLoadingLayoutVisibilityChanges(void);
	LoadingLayout* getHeaderLayout(void);
	const int getHeaderSize(void);
	int getPullToRefreshScrollDuration(void);
	int getPullToRefreshScrollDurationLonger(void);
	Tizen::Ui::Controls::ScrollPanel* getRefreshableViewWrapper(void);
	virtual bool isReadyForPullStart(void) = 0;
	void onPtrRestoreInstanceState(void) {}
	void onPtrSaveInstanceState(void) {}
	void onPullToRefresh(void);
	void onRefreshing(const bool doScroll);
	void onReleaseToRefresh(void);
	void onReset(void);
	const void onSizeChanged(int w, int h);
	const void refreshLoadingViewsSize();
	const void refreshRefreshableViewSize(int width, int height);
	const void smoothScrollTo(int scrollValue);
	const void smoothScrollTo(int scrollValue, OnSmoothScrollFinishedListener* listener);
	const void smoothScrollToLonger(int scrollValue);
	void updateUIForMode(void);


private:

	int mTouchSlop;
	float mLastMotionX, mLastMotionY;
	float mInitialMotionX, mInitialMotionY;

	State mState ;
	Mode mMode ;

	Orientation mOrientation;
	Mode mCurrentMode;

	Tizen::Base::String flickdirection;
	int scrollPosition;

	void addRefreshableView(Tizen::Ui::Controls::ListView* refreshableView);
	void init(int formwidth, int formheight);
	bool isReadyForPull(void);
	void pullEvent(void);
	const void smoothScrollTo(int scrollValue, long duration);
	const void smoothScrollTo(int newScrollValue, long duration, long delayMillis,OnSmoothScrollFinishedListener* listener);


};

#endif /* PULLTOREFRESHBASE_H_ */
