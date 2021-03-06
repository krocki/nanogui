/*
* @Author: Kamil Rocki
* @Date:   2017-03-01 18:30:18
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-01 18:58:41
*/

/** \file */

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN ( nanogui )

/**
 * \class SQGraph graph.h nanogui/graph.h
 *
 * \brief Simple graph widget for showing a function plot.
 */
class NANOGUI_EXPORT SQGraph : public Widget {
	public:
		SQGraph ( Widget *parent, const std::string &caption = "Untitled" );
		
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
		
		const Color &graphColor() const { return mGraphColor; }
		
		void setGraphColor ( const Color &graphColor ) {
		
			mGraphColor = graphColor;
			mTextColor = graphColor;
			mForegroundColor = Color ( graphColor.r(), graphColor.g(), graphColor.b(), graphColor.a() / 4.0f );
			
		}
		
		void setFill ( const bool f ) { mFill = f; }
		
		const Color &textColor() const { return mTextColor; }
		void setTextColor ( const Color &textColor ) { mTextColor = textColor; }
		
		const VectorXf &values() const { return mValues; }
		VectorXf &values() { return mValues; }
		void setValues ( const VectorXf &values ) { mValues = values; }
		
		virtual Vector2i preferredSize ( NVGcontext *ctx ) const override;
		virtual void draw ( NVGcontext *ctx ) override;
		
		virtual void save ( Serializer &s ) const override;
		virtual bool load ( Serializer &s ) override;
	protected:
		std::string mCaption, mHeader, mFooter;
		Color mBackgroundColor, mForegroundColor, mTextColor, mGraphColor;
		VectorXf mValues;
		bool mFill;
		bool mBezier;
		
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

NAMESPACE_END ( nanogui )
