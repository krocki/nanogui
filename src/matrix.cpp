/*
* @Author: kmrocki@us.ibm.com
* @Date:   2017-03-03 17:21:53
* @Last Modified by:   kmrocki@us.ibm.com
* @Last Modified time: 2017-03-03 21:12:18
*/

#include <nanogui/matrix.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/serializer/core.h>

NAMESPACE_BEGIN ( nanogui )

MatrixPlot::MatrixPlot ( Widget *parent, const std::string &caption )
	: Widget ( parent ), mCaption ( caption ) {

	Widget::setSize ( {180, 45} );
	mBackgroundColor = Color ( 20, 128 );
	// mShadeColor =  Color ( 255, 192, 0, 128 );
	// mTextColor = mShadeColor;
	// mForegroundColor = mShadeColor;
}

Vector2i MatrixPlot::preferredSize ( NVGcontext * ) const {
	return mSize;
}

void MatrixPlot::draw ( NVGcontext *ctx ) {
	Widget::draw ( ctx );

	nvgBeginPath ( ctx );
	nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	nvgFillColor ( ctx, mBackgroundColor );
	nvgFill ( ctx );

	// to be taken from 2d vector dims
	int w = mValues.cols();
	int h = mValues.rows();

	float margin_left = 1;
	float margin_right = 1;
	float margin_top = 1;
	float margin_bottom = 1;
	float spacing = 1;

	float begin_x = mPos.x() + margin_left;
	float begin_y = mPos.y() + margin_top;

	float total_width = mSize.x() - margin_right - margin_left;
	float total_height = mSize.y() - margin_top - margin_bottom;

	float rect_width = ( total_width - ( spacing * (w - 2) ) ) / ( float ) w;
	float rect_height = ( total_height - ( spacing * (h - 2) ) ) / ( float ) h;

	// TODO: do this in parallel
	for ( int i = 0; i < w; i++ ) {
		for ( int j = 0; j < h; j++ ) {

			nvgBeginPath ( ctx );

			float current_val = mValues(i, j);
			nvgRect ( ctx, begin_x + i * ( spacing + rect_width ), begin_y + j * ( spacing + rect_height ),
			          rect_width, rect_height );

			Color v = mForegroundColor;
			v.w() = current_val;
			nvgFillColor ( ctx, v );
			nvgFill ( ctx );

		}
	}

	nvgFontFace ( ctx, "sans" );

	if ( !mCaption.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 2, mCaption.c_str(), NULL );
	}

	if ( !mHeader.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 2, mHeader.c_str(), NULL );
	}

	if ( !mFooter.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + 2, mFooter.c_str(), NULL );
	}

	nvgBeginPath ( ctx );
	nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	nvgStrokeColor ( ctx, Color ( 100, 255, 0, 64 ) );
	nvgStroke ( ctx );
}

void MatrixPlot::save ( Serializer & s ) const {
	Widget::save ( s );
	s.set ( "caption", mCaption );
	s.set ( "header", mHeader );
	s.set ( "footer", mFooter );
	s.set ( "backgroundColor", mBackgroundColor );
	s.set ( "foregroundColor", mForegroundColor );
	s.set ( "textColor", mTextColor );
	s.set ( "values", mValues );
}

bool MatrixPlot::load ( Serializer & s ) {
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
