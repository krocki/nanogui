/*
* @Author: Kamil Rocki
* @Date:   2017-03-01 18:31:12
* @Last Modified by:   kmrocki@us.ibm.com
* @Last Modified time: 2017-03-01 20:46:27
*/

#include <nanogui/sq_graph.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/serializer/core.h>

NAMESPACE_BEGIN ( nanogui )

SQGraph::SQGraph ( Widget *parent, const std::string &caption )
	: Widget ( parent ), mCaption ( caption ) {

	Widget::setSize ( {100, 16} );
	mBackgroundColor = Color ( 20, 128 );
	// mGraphColor =  Color ( 255, 192, 0, 128 );
	// mTextColor = mGraphColor;
	// mForegroundColor = mGraphColor;
	mFill = false;
	mBezier = true;
}

Vector2i SQGraph::preferredSize ( NVGcontext * ) const {
	return mSize;
}

void SQGraph::draw ( NVGcontext *ctx ) {
	Widget::draw ( ctx );

	nvgFontFace ( ctx, "sans" );

	if ( !mCaption.empty() ) {
		nvgFontSize ( ctx, 11.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 3, mCaption.c_str(), NULL );
	}

	if ( !mHeader.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + 2, mHeader.c_str(), NULL );
	}

	/* kmrocki */
	// if ( !mFooter.empty() ) {
	//  nvgFontSize ( ctx, 15.0f );
	//  nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM );
	//  nvgFillColor ( ctx, mTextColor );
	//  nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + mSize.y() - 1, mFooter.c_str(), NULL );
	// }

	if ( !mFooter.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + 2, mFooter.c_str(), NULL );
	}

	// border
	// nvgBeginPath ( ctx );
	// nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	// nvgStrokeColor ( ctx, Color ( 100, 255 ) );
	// nvgStroke ( ctx );

	// squares
	int num_squares = 10;
	float margin_left = 25;
	float margin_right = 25;
	float margin_top = 5;
	float margin_bottom = 4;
	float spacing = 3;
	float begin = mPos.x() + margin_left;

	float total_width = mSize.x() - margin_right - margin_left;
	float rect_width = (total_width - (spacing * num_squares)) / (float)num_squares;

	float data_max = mValues.maxCoeff();
	size_t num_vals = mValues.size();
	size_t bin_size = num_vals / num_squares;

	for ( int i = 0; i < num_squares; i++ ) {

		nvgBeginPath ( ctx );

		float current_val = mValues.block ( i * bin_size, 0, bin_size, 1 ).mean() / data_max;
		nvgRect ( ctx, begin + i * ( spacing + rect_width ), mPos.y() + margin_top,
		          rect_width, mSize.y() - margin_top - margin_bottom );

		Color v = mForegroundColor;
		v.w() = current_val;
		nvgFillColor ( ctx, v);
		nvgFill ( ctx );
	}


}

void SQGraph::save ( Serializer &s ) const {
	Widget::save ( s );
	s.set ( "caption", mCaption );
	s.set ( "header", mHeader );
	s.set ( "footer", mFooter );
	s.set ( "backgroundColor", mBackgroundColor );
	s.set ( "foregroundColor", mForegroundColor );
	s.set ( "textColor", mTextColor );
	s.set ( "values", mValues );
}

bool SQGraph::load ( Serializer &s ) {
	if ( !Widget::load ( s ) ) return false;

	if ( !s.get ( "caption", mCaption ) ) return false;

	if ( !s.get ( "header", mHeader ) ) return false;

	if ( !s.get ( "footer", mFooter ) ) return false;

	if ( !s.get ( "backgroundColor", mBackgroundColor ) ) return false;

	if ( !s.get ( "foregroundColor", mForegroundColor ) ) return false;

	if ( !s.get ( "textColor", mTextColor ) ) return false;

	if ( !s.get ( "values", mValues ) ) return false;

	return true;
}

NAMESPACE_END ( nanogui )
