/*
* @Author: kmrocki@us.ibm.com
* @Date:   2017-03-03 17:21:53
* @Last Modified by:   kmrocki@us.ibm.com
* @Last Modified time: 2017-03-03 20:47:10
*/

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN ( nanogui )

/**
 * \class MatrixPlot MatrixPlot.h nanogui/MatrixPlot.h
 *
 * \brief Simple MatrixPlot widget for showing a function plot.
 */

class NANOGUI_EXPORT MatrixPlot : public Widget {
  public:
	MatrixPlot ( Widget *parent, const std::string &caption = "Untitled" );

	const std::string &caption() const { return mCaption; }
	void setCaption ( const std::string &caption ) { mCaption = caption; }

	const std::string &header() const { return mHeader; }
	void setHeader ( const std::string &header ) { mHeader = header; }

	const std::string &footer() const { return mFooter; }
	void setFooter ( const std::string &footer ) { mFooter = footer; }

	const Color &backgroundColor() const { return mBackgroundColor; }
	void setBackgroundColor ( const Color &backgroundColor ) { mBackgroundColor = backgroundColor; }

	const Color &foregroundColor() const { return mForegroundColor; }
	void setForegroundColor ( const Color &foregroundColor ) { mForegroundColor = foregroundColor; }

	const Color &ShadeColor() const { return mShadeColor; }

	void setShadeColor ( const Color &ShadeColor ) {

		mShadeColor = ShadeColor;
		mTextColor = ShadeColor;
		mForegroundColor = Color ( ShadeColor.r(), ShadeColor.g(), ShadeColor.b(), ShadeColor.a() / 4.0f );

	}

	const Color &textColor() const { return mTextColor; }
	void setTextColor ( const Color &textColor ) { mTextColor = textColor; }

	const MatrixXf &values() const { return mValues; }
	MatrixXf &values() { return mValues; }
	void setValues ( const MatrixXf &values ) { mValues = values; }

	virtual Vector2i preferredSize ( NVGcontext *ctx ) const override;
	virtual void draw ( NVGcontext *ctx ) override;

	virtual void save ( Serializer &s ) const override;
	virtual bool load ( Serializer &s ) override;

  protected:
	std::string mCaption, mHeader, mFooter;
	Color mBackgroundColor, mForegroundColor, mTextColor, mShadeColor;
	MatrixXf mValues;

  public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

NAMESPACE_END ( nanogui )