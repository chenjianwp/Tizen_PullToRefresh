/*
 * EmptyViewMethodAccessor.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: seungju
 */

/**
 * Interface that allows PullToRefreshBase to hijack the call to
 * AdapterView.setEmptyView()
 *
 * @author chris
 */

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Controls::ListView;
using namespace Tizen::Base;

class EmptyViewMethodAccessor {

public:
	virtual ~EmptyViewMethodAccessor(){}
	/**
	 * Calls upto AdapterView.setEmptyView()
	 *
	 * @param emptyView - to set as Empty View
	 */
	virtual void setEmptyViewInternal(ListView listview);

	/**
	 * Should call PullToRefreshBase.setEmptyView() which will then
	 * automatically call through to setEmptyViewInternal()
	 *
	 * @param emptyView - to set as Empty View
	 */
	virtual void setEmptyView(ListView listview);

}

