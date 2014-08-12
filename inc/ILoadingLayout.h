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

	virtual void setLastUpdatedLabel(const String& label) = 0;


	virtual void setLoadingDrawable(Bitmap& bitmap) = 0;


	virtual void setPullLabel(const String& pullLabel) = 0;


	virtual void setRefreshingLabel(const String& refreshingLabel) = 0;


	virtual void setReleaseLabel(const String& releaseLabel) = 0;


	virtual void setTextFont(Font& font) = 0;
};



