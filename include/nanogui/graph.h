/*
    nanogui/graph.h -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>
#include <nanogui/colormaps.h>

//to sort indices of colorbars
#include <numeric>
#include <vector>

NAMESPACE_BEGIN ( nanogui )

/**
 * \class Graph graph.h nanogui/graph.h
 *
 * \brief Simple graph widget for showing a function plot.
 */

enum class GraphType {

	GRAPH_DEFAULT = 0,
	GRAPH_NANOGUI = 1,
	GRAPH_COLORBARS = 2,
	GRAPH_NANOGUI_NOFILL = 3,

};

#define GRAPH_DEF_WIDTH 180
#define GRAPH_DEF_HEIGHT 45

class NANOGUI_EXPORT Graph : public Widget {
  public:
	Graph ( Widget * parent, const std::string &caption = "",
	        GraphType type = GraphType::GRAPH_DEFAULT,
	        Color gcolor = Color ( 127, 127, 127, 64 ),
	        std::function<Color(const float)> cmap = (std::function<Color(const float)>()),
	        Vector2i sz = Vector2i(GRAPH_DEF_WIDTH, GRAPH_DEF_HEIGHT));

	const std::string & caption() const { return mCaption; }
	void setCaption ( const std::string & caption ) { mCaption = caption; }

	const std::string & header() const { return mHeader; }
	void setHeader ( const std::string & header ) { mHeader = header; }

	const std::string & footer() const { return mFooter; }
	void setFooter ( const std::string & footer ) { mFooter = footer; }

	const Color & backgroundColor() const { return mBackgroundColor; }
	void setBackgroundColor ( const Color & backgroundColor ) { mBackgroundColor = backgroundColor; }

	const Color & foregroundColor() const { return mForegroundColor; }
	void setForegroundColor ( const Color & foregroundColor ) { mForegroundColor = foregroundColor; }

	const Color & graphColor() const { return mGraphColor; }

	void setGraphColor ( const Color & graphColor ) {

		mGraphColor = graphColor;
		mTextColor = graphColor;
		mForegroundColor = Color ( graphColor.r(), graphColor.g(), graphColor.b(), graphColor.a() / 4.0f );

	}

	void setFill ( const bool f ) { mFill = f; }

	const Color & textColor() const { return mTextColor; }
	void setTextColor ( const Color & textColor ) { mTextColor = textColor; }

	const VectorXf & values() const { return mValues; }
	VectorXf & values() { return mValues; }

	const VectorXf* values_ptr() const { return &mValues; }
	VectorXf* values_ptr() { return &mValues; }

	void setValues ( const VectorXf & values ) { mValues = values; }

	virtual Vector2i preferredSize ( NVGcontext * ctx ) const override;
	virtual void draw ( NVGcontext * ctx ) override;

	virtual void save ( Serializer & s ) const override;
	virtual bool load ( Serializer & s ) override;

  protected:

	std::string mCaption, mHeader, mFooter;
	Color mBackgroundColor, mForegroundColor, mTextColor, mGraphColor;

	GraphType gtype;
	std::function<Color(const float)> mColormap;

  public:
	VectorXf mValues;
	bool mFill;
	bool mBezier;

  public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

NAMESPACE_END ( nanogui )
