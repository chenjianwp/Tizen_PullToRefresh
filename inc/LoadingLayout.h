/*
 * LoadingLayout.h
 *
 *  Created on: Jul 23, 2014
 *      Author: kohun
 */

#ifndef LOADINGLAYOUT_H_
#define LOADINGLAYOUT_H_

#include <ILoadingLayout.h>
#include <FUi.h>

class LoadingLayout
	: public ILoadingLayout
	, public Tizen::Ui::Controls::TableView
{
public:
	LoadingLayout(void);
	virtual ~LoadingLayout(void);

	result Construct(Tizen::Ui::Animations::ParallelAnimationGroup* aniGroup);

	int getContentSize();
	void hideAllViews();

	void onPull(float scaleOfLayout);
	void pullToRefresh();
	void refreshing();
	void releaseToRefresh();
	void reset();

	//ILoadingLayout
	void setLastUpdatedLabel(const String& label);
	void setLoadingDrawable(Bitmap& imageBitmap);

	void setPullLabel(const String& pullLabel);
	void setRefreshingLabel(const String& refreshingLabel);
	void setReleaseLabel(const String& releaseLabel);
	void setTextFont(Font& font);

	void showInvisibleViews();

	/**
	 * Callbacks for derivative Layouts
	 */
protected:
	virtual void onLoadingDrawableSet(const Bitmap& imageBitmap)=0;
	virtual void onPullImpl(float scaleOfLayout=0)=0;
	virtual void pullToRefreshImpl(void)=0;
	virtual void refreshingImpl(void)=0;
	virtual void releaseToRefreshImpl(void)=0;
	virtual void resetImpl(void)=0;

	void setSubHeaderText(String& label);
	void setSubTextColor(Color& color);
	void setTextAppearance(int value);
	void setTextColor(Color& color);

protected:
	Tizen::Base::String mPullLabel;
	Tizen::Base::String mRefreshingLabel;
	Tizen::Base::String mReleaseLabel;

	Tizen::Ui::Controls::TableView* __pTableView;

	Tizen::Ui::Controls::Label* mHeaderText;
	Tizen::Ui::Controls::Label* mSubHeaderText;

	//Tizen::Ui::Animations::VisualElement* mHeaderImage;
	Tizen::Ui::Controls::Label* mHeaderImage;
	bool isAnimation;
};


#endif /* LOADINGLAYOUT_H_ */
