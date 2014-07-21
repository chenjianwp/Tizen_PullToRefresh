/*
 * ILoadingLayout.h
 *
 *  Created on: Jul 21, 2014
 *      Author: seungju
 */
#include <FBase.h>
#include <FGraphics.h>

using namespace Tizen::Base;
using namespace Tizen::Graphics;


class ILoadingLayout{

public:
	virtual ~ILoadingLayout(void) {} 
	/**
	 * Set the Last Updated Text. This displayed under the main label when
	 * Pulling
	 *
	 * @param label - Label to set
	 */
	virtual void setLastUpdatedLabel(const String& label);

	/**
	 * Set the drawable used in the loading layout. This is the same as calling
	 * <code>setLoadingDrawable(drawable, Mode.BOTH)</code>
	 *
	 * @param drawable - Drawable to display
	 */
	virtual void setLoadingDrawable(Bitmap* bitmap);

	/**
	 * Set Text to show when the Widget is being Pulled
	 * <code>setPullLabel(releaseLabel, Mode.BOTH)</code>
	 *
	 * @param pullLabel - CharSequence to display
	 */
	virtual void setPullLabel(const String& pullLabel);

	/**
	 * Set Text to show when the Widget is refreshing
	 * <code>setRefreshingLabel(releaseLabel, Mode.BOTH)</code>
	 *
	 * @param refreshingLabel - CharSequence to display
	 */
	virtual void setRefreshingLabel(const String& refreshingLabel);

	/**
	 * Set Text to show when the Widget is being pulled, and will refresh when
	 * released. This is the same as calling
	 * <code>setReleaseLabel(releaseLabel, Mode.BOTH)</code>
	 *
	 * @param releaseLabel - CharSequence to display
	 */
	virtual void setReleaseLabel(const String& releaseLabel);

	/**
	 * Set's the Sets the typeface and style in which the text should be
	 * displayed. Please see
	 * {@link android.widget.TextView#setTypeface(Typeface)
	 * TextView#setTypeface(Typeface)}. 
	 */
	virtual void setTextFont(Font font);
};



