
#include<Enums.h>
#include<OnPullEventListener.h>
#include<OnRefreshListener.h>
#include<OnSmoothScrollFinishedListener.h>


class IPullToRefresh
{
public:
	IPullToRefresh(){}
	virtual ~IPullToRefresh(){}
	virtual const Mode getCurrentMode()=0;
	virtual const bool getFilterTouchEvents()=0;
	virtual const Mode getMode()=0;
	virtual const Tizen::Ui::Controls::ListView* getRefreshableView()=0;
	virtual const bool getShowViewWhileRefreshing()=0;
	virtual const void onRefreshComplete()=0;
	virtual const void setFilterTouchEvents(bool filterEvents)=0;
	virtual const void setMode(Mode mode)=0;
	virtual void setOnPullEventListener(OnPullEventListener listener)=0;
	virtual const void setOnRefreshListener(OnRefreshListener listener)=0;
	virtual const void setPullToRefreshOverScrollEnabled(bool enabled)=0;
	virtual const void setRefreshing()=0;
	virtual const void setRefreshing(bool doScroll)=0;
	virtual void setScrollingWhileRefreshingEnabled(bool scrollingWhileRefreshingEnabled)=0;
	virtual void setShowViewWhileRefreshing(bool showView)=0;
};

