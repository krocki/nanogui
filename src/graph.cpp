/*
    src/graph.cpp -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/graph.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/serializer/core.h>

NAMESPACE_BEGIN ( nanogui )

Graph::Graph ( Widget *parent, const std::string &caption, GraphType gt, Color gColor,
			   std::function<Color ( const float )> cmap, Vector2i sz )
	: Widget ( parent ), mCaption ( caption ), mGraphColor ( gColor ), gtype ( gt ), mColormap ( cmap ) {
	
	Widget::setSize ( sz );
	mBackgroundColor = Color ( 16, 16, 16, 64 );
	mTextColor = mGraphColor;
	// mForegroundColor = mGraphColor;
	mFill = false;
	mBezier = false;
	
	if ( !mColormap )
		mColormap = [&] ( float ) { return mGraphColor; };
		
}

Vector2i Graph::preferredSize ( NVGcontext * ) const {
	return mSize;
}

void Graph::draw ( NVGcontext *ctx ) {
	Widget::draw ( ctx );
	
	nvgBeginPath ( ctx );
	nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	nvgFillColor ( ctx, mBackgroundColor );
	nvgFill ( ctx );
	
	if ( extValues )
		mValues = *extValues;
		
	if ( gtype == GraphType::GRAPH_NANOGUI || gtype == GraphType::GRAPH_NANOGUI_NOFILL ) {
	
		if ( mValues.size() < 2 )
			return;
			
		nvgBeginPath ( ctx );
		
		if ( gtype == GraphType::GRAPH_NANOGUI )
			nvgMoveTo ( ctx, mPos.x(), mPos.y() + mSize.y() );
			
		for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
		
			float value = mValues[i];
			float vx = mPos.x() + i * mSize.x() / ( float ) ( mValues.size() - 1 );
			float vy = mPos.y() + ( 1 - value ) * mSize.y();
			
			if ( i == 0 && gtype == GraphType::GRAPH_NANOGUI_NOFILL )
				nvgMoveTo ( ctx, vx, vy );
			else
				nvgLineTo ( ctx, vx, vy );
		}
		
		if ( gtype == GraphType::GRAPH_NANOGUI )
			nvgLineTo ( ctx, mPos.x() + mSize.x(), mPos.y() + mSize.y() );
			
		nvgStrokeColor ( ctx, mColormap ( mValues.mean() ) );
		nvgStroke ( ctx );
		
		if ( gtype == GraphType::GRAPH_NANOGUI ) {
		
			nvgFillColor ( ctx, mForegroundColor );
			nvgFill ( ctx );
			
		}
		
	}
	else
	
		if ( gtype == GraphType::GRAPH_LEGEND ) {
		
			float barwidth = ( float ) mSize.x() / ( float ) mValues.size() - 1;
			size_t top_margin = 0;
			
			// nvgBeginPath ( ctx );
			// nvgRect ( ctx, mPos.x() + 1, mPos.y() + 1, mSize.x() - 2, top_margin - 2 );
			// nvgFillColor ( ctx, mBackgroundColor );
			// nvgFill ( ctx );
			
			// nvgBeginPath ( ctx );
			// nvgRect ( ctx, mPos.x(), mPos.y() + top_margin, mSize.x(), mSize.y() - top_margin - 2 );
			// nvgStrokeColor ( ctx, Color ( 100, 100, 100, 32 ) );
			// nvgStroke ( ctx );
			
			char str[64];
			
			for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
			
				nvgBeginPath ( ctx );
				
				float value = mValues[i];
				
				nvgRect ( ctx, mPos.x() + i * barwidth + i, mPos.y() + top_margin + ( 1.0f - value ) * ( mSize.y() - top_margin - 2 ),
						  barwidth, value * ( mSize.y() - top_margin - 2 ) );
				nvgStrokeColor ( ctx, parula_lut[i] );
				nvgStroke ( ctx );
				nvgFillColor ( ctx, Color ( parula_lut[i].x(), parula_lut[i].y(), parula_lut[i].z(), parula_lut[i].a() * 0.7 ) );
				nvgFill ( ctx );
				
			}
			
			for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
			
				nvgFontSize ( ctx, 14.0f );
				nvgTextAlign ( ctx, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE );
				nvgFillColor ( ctx, Color ( 255, 255, 255, 255 ) );
				sprintf ( str, "%zu", i );
				nvgText ( ctx, mPos.x() + i * barwidth + i + barwidth / 2 - 3, mPos.y() + 20, str, NULL );
			}
			
			
		}
		
	if ( gtype == GraphType::GRAPH_COLORBARS ) {
	
		float barwidth = ( float ) mSize.x() / ( float ) mValues.size() - 1;
		
		size_t top_margin = 17;
		
		//TODO: move somewhere else
		std::vector<size_t> idx ( mValues.size() );
		iota ( idx.begin(), idx.end(), 0 );
		sort ( idx.begin(), idx.end(), [&] ( size_t i1, size_t i2 ) {return mValues[i1] > mValues[i2];} );
		
		char str[64];
		
		nvgBeginPath ( ctx );
		nvgRect ( ctx, mPos.x() + 1, mPos.y() + 1, mSize.x() - 2, top_margin - 2 );
		nvgFillColor ( ctx, mBackgroundColor );
		nvgFill ( ctx );
		
		nvgBeginPath ( ctx );
		nvgRect ( ctx, mPos.x(), mPos.y() + top_margin, mSize.x(), mSize.y() - top_margin - 2 );
		nvgStrokeColor ( ctx, Color ( 100, 100, 100, 32 ) );
		nvgStroke ( ctx );
		
		for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
		
			nvgBeginPath ( ctx );
			
			float value = mValues[i];
			
			nvgRect ( ctx, mPos.x() + i * barwidth + i, mPos.y() + top_margin + ( 1.0f - value ) * ( mSize.y() - top_margin - 2 ),
					  barwidth, value * ( mSize.y() - top_margin - 2 ) );
			nvgStrokeColor ( ctx, parula_lut[i] );
			nvgStroke ( ctx );
			nvgFillColor ( ctx, Color ( parula_lut[i].x(), parula_lut[i].y(), parula_lut[i].z(), parula_lut[i].a() * 0.7 ) );
			nvgFill ( ctx );
			
		}
		
		size_t top_k = 2;
		
		for ( size_t k = 0; k < top_k; k++ ) {
		
			nvgFontSize ( ctx, 9.0f );
			nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE );
			nvgFillColor ( ctx, parula_lut[idx[k]] );
			sprintf ( str, "%zu: %.2f", idx[k], mValues[idx[k]] );
			nvgText ( ctx, mPos.x() + mSize.x() - 2, mPos.y() + 5 + k * 7, str, NULL );
			
		}
		
	}
	else
		if ( gtype == GraphType::GRAPH_DEFAULT ) {
		
			if ( mValues.size() < 2 )
				return;
				
			float scale = 1.0f;
			float offset = 0.0f;
			
			//auto-scale
			float data_max = mValues.maxCoeff();
			float data_min = mValues.minCoeff();
			
			offset = data_min;
			scale = 1.0f / ( ( data_max - data_min ) * ( 1.6f ) );
			
			nvgBeginPath ( ctx );
			nvgMoveTo ( ctx, mPos.x(), mPos.y() + mSize.y() );
			
			if ( mBezier ) {
			
				// bezier curves
				std::vector<float> sx ( mValues.size() );
				std::vector<float> sy ( mValues.size() );
				
				float dx = mSize.x() / ( mValues.size() - 1 );
				int i;
				
				for ( i = 0; i < mValues.size(); i++ ) {
					sx[i] = mPos.x() + i * dx;
					sy[i] = mPos.y() + mSize.y() * ( 1. - ( mValues[i] - offset ) * scale );
				}
				
				nvgLineTo ( ctx, mPos.x(), sy[0] );
				
				for ( i = 1; i < mValues.size(); i++ )
					nvgBezierTo ( ctx, sx[i - 1] + dx * 0.5f, sy[i - 1] + 2, sx[i] - dx * 0.5f, sy[i] + 2, sx[i], sy[i] + 2 );
					
				nvgLineTo ( ctx, mPos.x() + mSize.x(), sy.back() );
				
			}
			
			else {
			
				for ( size_t i = 0; i < ( size_t ) mValues.size(); i++ ) {
					float value = ( mValues[i] - offset ) * scale;
					float vx = mPos.x() + i * mSize.x() / ( float ) ( mValues.size() - 1 );
					float vy = mPos.y() + ( 1 - value ) * mSize.y();
					nvgLineTo ( ctx, vx, vy );
				}
				
			}
			
			if ( !mFill ) {
			
				NVGpaint line_grad = nvgLinearGradient ( ctx, mPos.x(), mPos.y(), mPos.x() + mSize.x(), mPos.y(),
														 nvgRGBA ( mForegroundColor.r(), mForegroundColor.g(), mForegroundColor.b(), 0 ), mGraphColor );
														 
				nvgStrokePaint ( ctx, line_grad );
				nvgStrokeWidth ( ctx, 1.0f );
				nvgStroke ( ctx );
			}
			
			else {
			
				nvgLineTo ( ctx, mPos.x() + mSize.x(), mPos.y() + mSize.y() );
				nvgStrokeColor ( ctx, mGraphColor );
				nvgStroke ( ctx );
				
				// solid
				// nvgFillColor ( ctx, mForegroundColor );
				// end solid
				
				// gradient
				NVGpaint shadowPaint = nvgLinearGradient ( ctx, mPos.x(), mPos.y(), mPos.x(), mPos.y() + mSize.y(),
														   mGraphColor,
														   nvgRGBA ( mForegroundColor.r(), mForegroundColor.g(), mForegroundColor.b(), 0 ) );
														   
														   
				nvgFillPaint ( ctx, shadowPaint );
				// end gradient
				
				nvgFill ( ctx );
				
			}
		}
		
	nvgFontFace ( ctx, "sans" );
	
	if ( !mCaption.empty() ) {
		nvgFontSize ( ctx, 9.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 2, mCaption.c_str(), NULL );
	}
	
	int last_avg = 10;
	if ( mValues.size() > last_avg ) {
	
		// set header
		char str[256];
		sprintf ( str, "%3.3f\n", mValues.tail ( last_avg ).mean() );
		setHeader ( str );
		
	}
	
	if ( !mHeader.empty() ) {
		nvgFontSize ( ctx, 9.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + 3, mPos.y() + 2, mHeader.c_str(), NULL );
	}
	
	/* kmrocki */
	// if ( !mFooter.empty() ) {
	//  nvgFontSize ( ctx, 15.0f );
	//  nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM );
	//  nvgFillColor ( ctx, mTextColor );
	//  nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + mSize.y() - 1, mFooter.c_str(), NULL );
	// }
	
	if ( !mFooter.empty() ) {
		nvgFontSize ( ctx, 9.0f );
		nvgTextAlign ( ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP );
		nvgFillColor ( ctx, mTextColor );
		nvgText ( ctx, mPos.x() + mSize.x() - 3, mPos.y() + 2, mFooter.c_str(), NULL );
	}
	
	nvgBeginPath ( ctx );
	nvgRect ( ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y() );
	nvgStrokeColor ( ctx, Color ( 100, 255, 0, 32 ) );
	nvgStroke ( ctx );
}

void Graph::save ( Serializer &s ) const {
	Widget::save ( s );
	s.set ( "caption", mCaption );
	s.set ( "header", mHeader );
	s.set ( "footer", mFooter );
	s.set ( "backgroundColor", mBackgroundColor );
	s.set ( "foregroundColor", mForegroundColor );
	s.set ( "textColor", mTextColor );
	s.set ( "values", mValues );
}

bool Graph::load ( Serializer &s ) {
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
