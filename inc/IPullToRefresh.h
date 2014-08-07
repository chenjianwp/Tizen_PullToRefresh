
#include<Enums.h>
#include<OnPullEventListener.h>
#include<OnRefreshListener.h>
#include<OnSmoothScrollFinishedListener.h>

using namespace Tizen::Ui::Controls::ListView;

class IPullToRefresh
{
public:
	virtual ~IPullToRefresh(){}
	virtual Mode getCurrentMode()=0;
	virtual bool getFilterTouchEvents()=0;
	virtual Mode getMode()=0;
	virtual ListView getRefreshableView()=0;
	virtual bool getShowViewWhileRefreshing()=0;
	virtual void onRefreshComplete()=0;
	virtual void onRefreshComplete()=0;
	virtual void setFilterTouchEvents(bool filterEvents)=0;
	virtual void setMode(Mode mode)=0;
	virtual void setOnPullEventListener(OnPullEventListener listener)=0;
	virtual void setOnRefreshListener(OnRefreshListener listener)=0;
	virtual void setPullToRefreshOverScrollEnabled(bool enabled)=0;
	virtual void setRefreshing()=0;
	virtual void setRefreshing(bool doScroll)=0;
	virtual void setScrollingWhileRefreshingEnabled(bool scrollingWhileRefreshingEnabled)=0;
	virtual void setShowViewWhileRefreshing(bool showView)=0;
};
