/*
* @Author: Kamil Rocki
* @Date:   2017-03-01 18:31:12
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-01 19:37:13
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
	
	// nvgBeginPath ( ctx );
	// nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	// nvgFillColor ( ctx, mBackgroundColor );
	// nvgFill ( ctx );
	
	// if ( mValues.size() < 2 )
	// 	return;
	
	// float scale = 1.0f;
	// float offset = 0.0f;
	
	// //auto-scale
	// float data_max = mValues.maxCoeff();
	// float data_min = mValues.minCoeff();
	
	// offset = data_min;
	// scale = 1.0f / ( ( data_max - data_min ) * ( 1.6f ) );
	
	// nvgBeginPath ( ctx );
	// nvgMoveTo ( ctx, mPos.x(), mPos.y() + mSize.y() );
	
	// if ( mBezier ) {
	
	// 	// bezier curves
	// 	std::vector<float> sx ( mValues.size() );
	// 	std::vector<float> sy ( mValues.size() );
	
	// 	float dx = mSize.x() / ( mValues.size() - 1 );
	// 	int i;
	
	// 	for ( i = 0; i < mValues.size(); i++ ) {
	// 		sx[i] = mPos.x() + i * dx;
	// 		sy[i] = mPos.y() + mSize.y() * ( 1. - ( mValues[i] - offset ) * scale );
	// 	}
	
	// 	nvgLineTo ( ctx, mPos.x(), sy[0] );
	
	// 	for ( i = 1; i < mValues.size(); i++ )
	// 		nvgBezierTo ( ctx, sx[i - 1] + dx * 0.5f, sy[i - 1] + 2, sx[i] - dx * 0.5f, sy[i] + 2, sx[i], sy[i] + 2 );
	
	// 	nvgLineTo ( ctx, mPos.x() + mSize.x(), sy.back() );
	
	// }
	
	// else {
	
	// 	for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
	// 		float value = ( mValues[i] - offset ) * scale;
	// 		float vx = mPos.x() + i * mSize.x() / ( float ) ( mValues.size() - 1 );
	// 		float vy = mPos.y() + ( 1 - value ) * mSize.y();
	// 		nvgLineTo ( ctx, vx, vy );
	// 	}
	
	// }
	
	// if ( !mFill ) {
	
	// 	nvgStrokeColor ( ctx, mGraphColor );
	// 	nvgStrokeWidth ( ctx, 1.0f );
	// 	nvgStroke ( ctx );
	// }
	
	// else {
	
	// 	nvgLineTo ( ctx, mPos.x() + mSize.x(), mPos.y() + mSize.y() );
	// 	nvgStrokeColor ( ctx, mGraphColor );
	// 	nvgStroke ( ctx );
	
	// 	// solid
	// 	// nvgFillColor ( ctx, mForegroundColor );
	// 	// end solid
	
	// 	// gradient
	// 	NVGpaint shadowPaint = nvgLinearGradient ( ctx, mPos.x(), mPos.y(), mPos.x(), mPos.y() + mSize.y(),
	// 						   mGraphColor,
	// 						   nvgRGBA ( mForegroundColor.r(), mForegroundColor.g(), mForegroundColor.b(), 0 ) );
	
	
	// 	nvgFillPaint ( ctx, shadowPaint );
	// 	// end gradient
	
	// 	nvgFill ( ctx );
	
	// }
	
	nvgFontFace ( ctx, "sans" );
	
	if ( !mCaption.empty() ) {
		nvgFontSize ( ctx, 12.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 2, mCaption.c_str(), NULL );
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
	nvgBeginPath ( ctx );
	nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	nvgStrokeColor ( ctx, Color ( 100, 255 ) );
	nvgStroke ( ctx );
	
	// squares
	int num_squares = 10;
	float margin_left = 25;
	float margin_right = 35;
	float margin_y = 4;
	float spacing = 6;
	float begin = mPos.x() + margin_left;
	float end = mPos.x() + mSize.x() - margin_left - margin_right;
	
	float total_width = end - begin - spacing * ( num_squares - 2 );
	float rect_width = total_width / num_squares;
	
	nvgBeginPath ( ctx );
	
	for ( int i = 0; i < num_squares; i++ ) {
	
		nvgRect ( ctx, begin + i * ( spacing + rect_width ), mPos.y() + margin_y,
				  5, mSize.y() - margin_y * 2.0f );
				  
	}
	
	nvgStrokeColor ( ctx, mGraphColor );
	nvgStroke ( ctx );
	// nvgFillColor ( ctx, mGraphColor );
	// nvgFill ( ctx );
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
