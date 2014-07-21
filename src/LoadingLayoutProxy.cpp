/*
package com.handmark.pulltorefresh.library;

import java.util.HashSet;

import android.graphics.Typeface;
import android.graphics.drawable.Drawable;

import com.handmark.pulltorefresh.library.internal.LoadingLayout;
*/

class LoadingLayoutProxy :public ILoadingLayout {

	/**
	 * This allows you to add extra LoadingLayout instances to this proxy. This
	 * is only necessary if you keep your own instances, and want to have them
	 * included in any
	 * {@link PullToRefreshBase#createLoadingLayoutProxy(boolean, boolean)
	 * createLoadingLayoutProxy(...)} calls.
	 *
	 * @param layout - LoadingLayout to have included.
	 */

private:

	const HashSet<LoadingLayout> mLoadingLayouts;

public:

	LoadingLayoutProxy() {
			mLoadingLayouts = new HashSet<LoadingLayout>();
		}

	void addLayout(LoadingLayout layout) {
		if (null != layout) {
			mLoadingLayouts.add(layout);
		}
	}

	//@Override
	void setLastUpdatedLabel(CharSequence label) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setLastUpdatedLabel(label);
			}
		}

	//@Override
	void setLoadingDrawable(Drawable drawable) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setLoadingDrawable(drawable);
			}
		}

		//@Override
	void setRefreshingLabel(CharSequence refreshingLabel) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setRefreshingLabel(refreshingLabel);
			}
		}

		//@Override
	void setPullLabel(CharSequence label) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setPullLabel(label);
			}
		}

	//@Override
	void setReleaseLabel(CharSequence label) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setReleaseLabel(label);
			}
		}

	 void setTextTypeface(Typeface tf) {
			for (LoadingLayout layout : mLoadingLayouts) {
				layout.setTextTypeface(tf);
			}
		}

};
