/*
 * ListenerInterface.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: seungju
 */

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Controls::ListView;

#include<PullToRefreshBase.cpp>

	class OnLastItemVisibleListener {
		/*
		/**
		 * Called when the user has scrolled to the end of the list
		*/
	public:
		virtual ~OnLastItemVisibleListener(){}
		virtual void onLastItemVisible();

	};

	/**
	 * Listener that allows you to be notified when the user has started or
	 * finished a touch event. Useful when you want to append extra UI events
	 * (such as sounds). See (
	 * {@link PullToRefreshAdapterViewBase#setOnPullEventListener}.
	 *
	 * @author Chris Banes
	 * */

	class OnPullEventListener { //이건 외부? 따로?

		/**
		 * Called when the internal state has been changed, usually by the user
		 * pulling.
		 *
		 * @param refreshView - View which has had it's state change.
		 * @param state - The new state of View.
		 * @param direction - One of {@link Mode#PULL_FROM_START} or
		 *            {@link Mode#PULL_FROM_END} depending on which direction
		 *            the user is pulling. Only useful when <var>state</var> is
		 *            {@link State#PULL_TO_REFRESH} or
		 *            {@link State#RELEASE_TO_REFRESH}.
		*/
	public:
		virtual ~OnPullEventListener(){}
		virtual void onPullEvent(const PullToRefreshBase<ListView> refreshView, State state, Mode direction);

	};

	/**
	 * Simple Listener to listen for any callbacks to Refresh.
	 *
	 * @author Chris Banes
	*/
	class OnRefreshListener { //이런건 여기있어도 될듯

		/**
		 * onRefresh will be called for both a Pull from start, and Pull from
		 * end
		*/
	public:
		virtual ~OnRefreshListener(){}
		virtual void onRefresh(const PullToRefreshBase<ListView> refreshView);

	};

	/**
	 * An advanced version of the Listener to listen for callbacks to Refresh.
	 * This listener is different as it allows you to differentiate between Pull
	 * Ups, and Pull Downs.
	 *
	 * @author Chris Banes
	*/
	class OnRefreshListener2 {  //이건 내부
		// TODO These methods need renaming to START/END rather than DOWN/UP

		/**
		 * onPullDownToRefresh will be called only when the user has Pulled from
		 * the start, and released.
		*/
	public:
		virtual ~OnRefreshListener2(){}
		virtual void onPullDownToRefresh(const PullToRefreshBase<ListView> refreshView);

		/**
		 * onPullUpToRefresh will be called only when the user has Pulled from
		 * the end, and released.
		*/
		virtual void onPullUpToRefresh(const PullToRefreshBase<ListView> refreshView);

	};

	class OnSmoothScrollFinishedListener { //이건 내
	public:
		virtual ~OnSmoothScrollFinishedListener(){}
		virtual void onSmoothScrollFinished();
	};




//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////Listener interface //////////////////////////////////////
	/*
	static interface OnLastItemVisibleListener {
		/*
		/**
		 * Called when the user has scrolled to the end of the list

		//public void onLastItemVisible();

	}

	/**
	 * Listener that allows you to be notified when the user has started or
	 * finished a touch event. Useful when you want to append extra UI events
	 * (such as sounds). See (
	 * {@link PullToRefreshAdapterViewBase#setOnPullEventListener}.
	 *
	 * @author Chris Banes

	public static interface OnPullEventListener<V extends View> { //이건 외부? 따로?

		/**
		 * Called when the internal state has been changed, usually by the user
		 * pulling.
		 *
		 * @param refreshView - View which has had it's state change.
		 * @param state - The new state of View.
		 * @param direction - One of {@link Mode#PULL_FROM_START} or
		 *            {@link Mode#PULL_FROM_END} depending on which direction
		 *            the user is pulling. Only useful when <var>state</var> is
		 *            {@link State#PULL_TO_REFRESH} or
		 *            {@link State#RELEASE_TO_REFRESH}.

		public void onPullEvent(final PullToRefreshBase<V> refreshView, State state, Mode direction);

	}

	/**
	 * Simple Listener to listen for any callbacks to Refresh.
	 *
	 * @author Chris Banes

	public static interface OnRefreshListener<V extends View> { //이런건 여기있어도 될듯

		/**
		 * onRefresh will be called for both a Pull from start, and Pull from
		 * end

		public void onRefresh(final PullToRefreshBase<V> refreshView);

	}

	/**
	 * An advanced version of the Listener to listen for callbacks to Refresh.
	 * This listener is different as it allows you to differentiate between Pull
	 * Ups, and Pull Downs.
	 *
	 * @author Chris Banes

	public static interface OnRefreshListener2<V extends View> {  //이건 내부
		// TODO These methods need renaming to START/END rather than DOWN/UP

		/**
		 * onPullDownToRefresh will be called only when the user has Pulled from
		 * the start, and released.

		public void onPullDownToRefresh(final PullToRefreshBase<V> refreshView);

		/**
		 * onPullUpToRefresh will be called only when the user has Pulled from
		 * the end, and released.

		public void onPullUpToRefresh(final PullToRefreshBase<V> refreshView);

	}

	static interface OnSmoothScrollFinishedListener { //이건 내
		void onSmoothScrollFinished();
	}
	*/
	/////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////



