/*
* @Author: Kamil Rocki
* @Date:   2017-03-01 09:57:12
* @Last Modified by:   kmrocki@us.ibm.com
* @Last Modified time: 2017-03-08 16:14:01
*/

#include <nanogui/window.h>
#include <nanogui/screen.h>
#include <nanogui/console.h>
#include <nanogui/opengl.h>
#include <nanogui/theme.h>
#include <nanogui/entypo.h>
#include <nanogui/serializer/core.h>
#include <regex>
#include <iostream>

NAMESPACE_BEGIN ( nanogui )

Console::Console ( Widget *parent, const std::string &value )
	: Widget ( parent ),
	  mEditable ( false ),
	  mSpinnable ( false ),
	  mCommitted ( true ),
	  mValue ( value ),
	  mDefaultValue ( "" ),
	  mAlignment ( Alignment::Center ),
	  mUnits ( "" ),
	  mFormat ( "" ),
	  mUnitsImage ( -1 ),
	  mValidFormat ( true ),
	  mValueTemp ( value ),
	  mCursorPos ( -1 ),
	  mSelectionPos ( -1 ),
	  mMousePos ( Vector2i ( -1, -1 ) ),
	  mMouseDownPos ( Vector2i ( -1, -1 ) ),
	  mMouseDragPos ( Vector2i ( -1, -1 ) ),
	  mMouseDownModifier ( 0 ),
	  mTextOffset ( 0 ),
	  mLastClick ( 0 ) {
	if ( mTheme ) mFontSize = mTheme->mConsoleFontSize;
}

void Console::setEditable ( bool editable ) {
	mEditable = editable;
	setCursor ( editable ? Cursor::IBeam : Cursor::Arrow );
}

void Console::setTheme ( Theme *theme ) {
	Widget::setTheme ( theme );

	if ( mTheme )
		mFontSize = mTheme->mConsoleFontSize;
}

Vector2i Console::preferredSize ( NVGcontext *ctx ) const {

	Vector2i size ( {mSize.x(), mSize.y() } );

	// don't adjust!
	NVG_NOTUSED ( ctx );
	return size;
}

//static float minf(float a, float b) { return a < b ? a : b; }
static float maxf ( float a, float b ) { return a > b ? a : b; }
//static float absf(float a) { return a >= 0.0f ? a : -a; }
static float clampf ( float a, float mn, float mx ) { return a < mn ? mn : ( a > mx ? mx : a ); }

void Console::draw ( NVGcontext *vg ) {
	Widget::draw ( vg );

	std::string str =
	    "This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.🎉";


	float mx = mMousePos[0];
	float my = mMousePos[1];
	float x = mPos.x();
	float y = mPos.y();
	float width = mSize.x();
	float height = mSize.y();

	// printf ( "mx %f my %f x %f y %f w %f h %f\n", mx, my, x, y, width, height );

	NVGtextRow rows[3];
	NVGglyphPosition glyphs[100];
	const char *text = mValue.c_str();
	const char *start;
	const char *end;
	int nrows, i, nglyphs, j, lnum = 0;
	float lineh;
	float caretx, px;
	float bounds[4];
	float a;
	float gx, gy;
	int gutter = 0;

	nvgSave ( vg );

	nvgFontSize ( vg, fontSize() );
	nvgFontFace ( vg, "sans" );
	nvgTextAlign ( vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
	nvgTextMetrics ( vg, NULL, NULL, &lineh );

	// The text break API can be used to fill a large buffer of rows,
	// or to iterate over the text just few lines (or just one) at a time.
	// The "next" variable of the last returned item tells where to continue.
	start = text;
	end = text + strlen ( text );

	float local_y = 0.0f;

	while ( ( nrows = nvgTextBreakLines ( vg, start, end, width, rows, 3 ) ) ) {
		for ( i = 0; i < nrows; i++ ) {
			NVGtextRow *row = &rows[i];
			int hit = mx > x && mx < ( x + width ) && my >= y && my < ( y + lineh );

			nvgBeginPath ( vg );
			nvgFillColor ( vg, nvgRGBA ( 255, 255, 255, hit ? 64 : 16 ) );
			nvgRect ( vg, x, y, row->width, lineh );
			nvgFill ( vg );

			nvgFillColor ( vg, nvgRGBA ( 255, 255, 255, 255 ) );

			//check if window is big enough
			if (local_y + lineh > height) {
				mSize[1] = local_y + lineh + 5;
			}

			nvgText ( vg, x, y, row->start, row->end );

			if ( hit ) {
				caretx = ( mx < x + row->width / 2 ) ? x : x + row->width;
				px = x;
				nglyphs = nvgTextGlyphPositions ( vg, x, y, row->start, row->end, glyphs, 100 );

				for ( j = 0; j < nglyphs; j++ ) {
					float x0 = glyphs[j].x;
					float x1 = ( j + 1 < nglyphs ) ? glyphs[j + 1].x : x + row->width;
					float gx = x0 * 0.3f + x1 * 0.7f;

					if ( mx >= px && mx < gx )
						caretx = glyphs[j].x;

					px = gx;
				}

				nvgBeginPath ( vg );
				nvgFillColor ( vg, nvgRGBA ( 255, 192, 0, 255 ) );
				nvgRect ( vg, caretx, y, 1, lineh );
				nvgFill ( vg );

				gutter = lnum + 1;
				gx = x - 10;
				gy = y + lineh / 2;
			}

			lnum++;
			y += lineh;
			local_y += lineh;
		}

		// Keep going...
		start = rows[nrows - 1].next;
	}

	if ( gutter ) {
		char txt[16];
		snprintf ( txt, sizeof ( txt ), "%d", gutter );
		nvgFontSize ( vg, fontSize() );
		nvgTextAlign ( vg, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE );

		nvgTextBounds ( vg, gx, gy, txt, NULL, bounds );

		nvgBeginPath ( vg );
		nvgFillColor ( vg, nvgRGBA ( 255, 192, 0, 255 ) );
		nvgRoundedRect ( vg, ( int ) bounds[0] - 4, ( int ) bounds[1] - 2, ( int ) ( bounds[2] - bounds[0] ) + 8,
		                 ( int ) ( bounds[3] - bounds[1] ) + 4, ( ( int ) ( bounds[3] - bounds[1] ) + 4 ) / 2 - 1 );
		nvgFill ( vg );

		nvgFillColor ( vg, nvgRGBA ( 32, 32, 32, 255 ) );
		nvgText ( vg, gx, gy, txt, NULL );
	}

	y += 20.0f;

	// nvgFontSize ( vg, 13.0f );
	// nvgTextAlign ( vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP );
	// nvgTextLineHeight ( vg, 1.2f );

	// nvgTextBoxBounds ( vg, x, y, 150, "Hover your mouse over the text to see calculated caret position.", NULL, bounds );

	// // Fade the tooltip out when close to it.
	// gx = fabsf ( ( mx - ( bounds[0] + bounds[2] ) * 0.5f ) / ( bounds[0] - bounds[2] ) );
	// gy = fabsf ( ( my - ( bounds[1] + bounds[3] ) * 0.5f ) / ( bounds[1] - bounds[3] ) );
	// a = maxf ( gx, gy ) - 0.5f;
	// a = clampf ( a, 0, 1 );
	// nvgGlobalAlpha ( vg, a );

	// nvgBeginPath ( vg );
	// nvgFillColor ( vg, nvgRGBA ( 220, 220, 220, 255 ) );
	// nvgRoundedRect ( vg, bounds[0] - 2, bounds[1] - 2, ( int ) ( bounds[2] - bounds[0] ) + 4,
	//               ( int ) ( bounds[3] - bounds[1] ) + 4, 3 );
	// px = ( int ) ( ( bounds[2] + bounds[0] ) / 2 );
	// nvgMoveTo ( vg, px, bounds[1] - 10 );
	// nvgLineTo ( vg, px + 7, bounds[1] + 1 );
	// nvgLineTo ( vg, px - 7, bounds[1] + 1 );
	// nvgFill ( vg );

	// nvgFillColor ( vg, nvgRGBA ( 0, 0, 0, 220 ) );
	// nvgTextBox ( vg, x, y, 150, "Hover your mouse over the text to see calculated caret position.", NULL );

	nvgRestore ( vg );
}

bool Console::mouseButtonEvent ( const Vector2i &p, int button, bool down,
                                 int modifiers ) {

	if ( button == GLFW_MOUSE_BUTTON_1 && down && !mFocused ) {
		if ( !mSpinnable || spinArea ( p ) == SpinArea::None ) /* not on scrolling arrows */
			requestFocus();
	}

	if ( mEditable && focused() ) {
		if ( down ) {
			mMouseDownPos = p;
			mMouseDownModifier = modifiers;

			double time = glfwGetTime();

			if ( time - mLastClick < 0.25 ) {
				/* Double-click: select all text */
				mSelectionPos = 0;
				mCursorPos = ( int ) mValueTemp.size();
				mMouseDownPos = Vector2i ( -1, -1 );
			}

			mLastClick = time;
		} else {
			mMouseDownPos = Vector2i ( -1, -1 );
			mMouseDragPos = Vector2i ( -1, -1 );
		}

		return true;
	} else if ( mSpinnable && !focused() ) {
		if ( down ) {
			if ( spinArea ( p ) == SpinArea::None ) {
				mMouseDownPos = p;
				mMouseDownModifier = modifiers;

				double time = glfwGetTime();

				if ( time - mLastClick < 0.25 ) {
					/* Double-click: reset to default value */
					mValue = mDefaultValue;

					if ( mCallback )
						mCallback ( mValue );

					mMouseDownPos = Vector2i ( -1, -1 );
				}

				mLastClick = time;
			} else {
				mMouseDownPos = Vector2i ( -1, -1 );
				mMouseDragPos = Vector2i ( -1, -1 );
			}
		} else {
			mMouseDownPos = Vector2i ( -1, -1 );
			mMouseDragPos = Vector2i ( -1, -1 );
		}

		return true;
	}

	return false;
}

bool Console::mouseMotionEvent ( const Vector2i &p, const Vector2i & /* rel */,
                                 int /* button */, int /* modifiers */ ) {
	mMousePos = p;

	if ( !mEditable )
		setCursor ( Cursor::Arrow );
	else if ( mSpinnable && !focused() && spinArea ( mMousePos ) != SpinArea::None ) /* scrolling arrows */
		setCursor ( Cursor::Hand );
	else
		setCursor ( Cursor::IBeam );

	if ( mEditable && focused() )
		return true;

	return false;
}

bool Console::mouseDragEvent ( const Vector2i &p, const Vector2i &/* rel */,
                               int /* button */, int /* modifiers */ ) {
	mMousePos = p;
	mMouseDragPos = p;

	if ( mEditable && focused() )
		return true;

	return false;
}

bool Console::focusEvent ( bool focused ) {
	Widget::focusEvent ( focused );

	std::string backup = mValue;

	if ( mEditable ) {
		if ( focused ) {
			mValueTemp = mValue;
			mCommitted = false;
			mCursorPos = 0;
		} else {
			if ( mValidFormat ) {
				if ( mValueTemp == "" )
					mValue = mDefaultValue;
				else
					mValue = mValueTemp;
			}

			if ( mCallback && !mCallback ( mValue ) )
				mValue = backup;

			mValidFormat = true;
			mCommitted = true;
			mCursorPos = -1;
			mSelectionPos = -1;
			mTextOffset = 0;
		}

		mValidFormat = ( mValueTemp == "" ) || checkFormat ( mValueTemp, mFormat );
	}

	return true;
}

bool Console::keyboardEvent ( int key, int /* scancode */, int action, int modifiers ) {
	if ( mEditable && focused() ) {
		if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
			if ( key == GLFW_KEY_LEFT ) {
				if ( modifiers == GLFW_MOD_SHIFT ) {
					if ( mSelectionPos == -1 )
						mSelectionPos = mCursorPos;
				} else
					mSelectionPos = -1;

				if ( mCursorPos > 0 )
					mCursorPos--;
			} else if ( key == GLFW_KEY_RIGHT ) {
				if ( modifiers == GLFW_MOD_SHIFT ) {
					if ( mSelectionPos == -1 )
						mSelectionPos = mCursorPos;
				} else
					mSelectionPos = -1;

				if ( mCursorPos < ( int ) mValueTemp.length() )
					mCursorPos++;
			} else if ( key == GLFW_KEY_HOME ) {
				if ( modifiers == GLFW_MOD_SHIFT ) {
					if ( mSelectionPos == -1 )
						mSelectionPos = mCursorPos;
				} else
					mSelectionPos = -1;

				mCursorPos = 0;
			} else if ( key == GLFW_KEY_END ) {
				if ( modifiers == GLFW_MOD_SHIFT ) {
					if ( mSelectionPos == -1 )
						mSelectionPos = mCursorPos;
				} else
					mSelectionPos = -1;

				mCursorPos = ( int ) mValueTemp.size();
			} else if ( key == GLFW_KEY_BACKSPACE ) {
				if ( !deleteSelection() ) {
					if ( mCursorPos > 0 ) {
						mValueTemp.erase ( mValueTemp.begin() + mCursorPos - 1 );
						mCursorPos--;
					}
				}
			} else if ( key == GLFW_KEY_DELETE ) {
				if ( !deleteSelection() ) {
					if ( mCursorPos < ( int ) mValueTemp.length() )
						mValueTemp.erase ( mValueTemp.begin() + mCursorPos );
				}
			} else if ( key == GLFW_KEY_ENTER ) {
				if ( !mCommitted )
					focusEvent ( false );
			} else if ( key == GLFW_KEY_A && modifiers == SYSTEM_COMMAND_MOD ) {
				mCursorPos = ( int ) mValueTemp.length();
				mSelectionPos = 0;
			} else if ( key == GLFW_KEY_X && modifiers == SYSTEM_COMMAND_MOD ) {
				copySelection();
				deleteSelection();
			} else if ( key == GLFW_KEY_C && modifiers == SYSTEM_COMMAND_MOD )
				copySelection();
			else if ( key == GLFW_KEY_V && modifiers == SYSTEM_COMMAND_MOD ) {
				deleteSelection();
				pasteFromClipboard();
			}

			mValidFormat =
			    ( mValueTemp == "" ) || checkFormat ( mValueTemp, mFormat );
		}

		return true;
	}

	return false;
}

bool Console::keyboardCharacterEvent ( unsigned int codepoint ) {
	if ( mEditable && focused() ) {
		std::ostringstream convert;
		convert << ( char ) codepoint;

		deleteSelection();
		mValueTemp.insert ( mCursorPos, convert.str() );
		mCursorPos++;

		mValidFormat = ( mValueTemp == "" ) || checkFormat ( mValueTemp, mFormat );

		return true;
	}

	return false;
}

bool Console::checkFormat ( const std::string &input, const std::string &format ) {
	if ( format.empty() )
		return true;

	try {
		std::regex regex ( format );
		return regex_match ( input, regex );
	} catch ( const std::regex_error & ) {
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
		std::cerr <<
		          "Warning: cannot validate text field due to lacking regular expression support. please compile with GCC >= 4.9" <<
		          std::endl;
		return true;
#else
		throw;
#endif
	}
}

bool Console::copySelection() {
	if ( mSelectionPos > -1 ) {
		Screen *sc = dynamic_cast<Screen *> ( this->window()->parent() );

		int begin = mCursorPos;
		int end = mSelectionPos;

		if ( begin > end )
			std::swap ( begin, end );

		glfwSetClipboardString ( sc->glfwWindow(),
		                         mValueTemp.substr ( begin, end ).c_str() );
		return true;
	}

	return false;
}

void Console::pasteFromClipboard() {
	Screen *sc = dynamic_cast<Screen *> ( this->window()->parent() );
	const char *cbstr = glfwGetClipboardString ( sc->glfwWindow() );

	if ( cbstr )
		mValueTemp.insert ( mCursorPos, std::string ( cbstr ) );
}

bool Console::deleteSelection() {
	if ( mSelectionPos > -1 ) {
		int begin = mCursorPos;
		int end = mSelectionPos;

		if ( begin > end )
			std::swap ( begin, end );

		if ( begin == end - 1 )
			mValueTemp.erase ( mValueTemp.begin() + begin );
		else
			mValueTemp.erase ( mValueTemp.begin() + begin,
			                   mValueTemp.begin() + end );

		mCursorPos = begin;
		mSelectionPos = -1;
		return true;
	}

	return false;
}

void Console::updateCursor ( NVGcontext *, float lastx,
                             const NVGglyphPosition *glyphs, int size ) {
	// handle mouse cursor events
	if ( mMouseDownPos.x() != -1 ) {
		if ( mMouseDownModifier == GLFW_MOD_SHIFT ) {
			if ( mSelectionPos == -1 )
				mSelectionPos = mCursorPos;
		} else
			mSelectionPos = -1;

		mCursorPos =
		    position2CursorIndex ( mMouseDownPos.x(), lastx, glyphs, size );

		mMouseDownPos = Vector2i ( -1, -1 );
	} else if ( mMouseDragPos.x() != -1 ) {
		if ( mSelectionPos == -1 )
			mSelectionPos = mCursorPos;

		mCursorPos =
		    position2CursorIndex ( mMouseDragPos.x(), lastx, glyphs, size );
	} else {
		// set cursor to last character
		if ( mCursorPos == -2 )
			mCursorPos = size;
	}

	if ( mCursorPos == mSelectionPos )
		mSelectionPos = -1;
}

float Console::cursorIndex2Position ( int index, float lastx,
                                      const NVGglyphPosition *glyphs, int size ) {
	float pos = 0;

	if ( index == size )
		pos = lastx; // last character
	else
		pos = glyphs[index].x;

	return pos;
}

int Console::position2CursorIndex ( float posx, float lastx,
                                    const NVGglyphPosition *glyphs, int size ) {
	int mCursorId = 0;
	float caretx = glyphs[mCursorId].x;

	for ( int j = 1; j < size; j++ ) {
		if ( std::abs ( caretx - posx ) > std::abs ( glyphs[j].x - posx ) ) {
			mCursorId = j;
			caretx = glyphs[mCursorId].x;
		}
	}

	if ( std::abs ( caretx - posx ) > std::abs ( lastx - posx ) )
		mCursorId = size;

	return mCursorId;
}

Console::SpinArea Console::spinArea ( const Vector2i &pos ) {
	if ( 0 <= pos.x() - mPos.x() && pos.x() - mPos.x() < 14.f ) { /* on scrolling arrows */
		if ( mSize.y() >= pos.y() - mPos.y() && pos.y() - mPos.y() <= mSize.y() / 2.f ) /* top part */
			return SpinArea::Top;
		else if ( 0.f <= pos.y() - mPos.y() && pos.y() - mPos.y() > mSize.y() / 2.f ) /* bottom part */
			return SpinArea::Bottom;
	}

	return SpinArea::None;
}

void Console::save ( Serializer &s ) const {
	Widget::save ( s );
	s.set ( "editable", mEditable );
	s.set ( "spinnable", mSpinnable );
	s.set ( "committed", mCommitted );
	s.set ( "value", mValue );
	s.set ( "defaultValue", mDefaultValue );
	s.set ( "alignment", ( int ) mAlignment );
	s.set ( "units", mUnits );
	s.set ( "format", mFormat );
	s.set ( "unitsImage", mUnitsImage );
	s.set ( "validFormat", mValidFormat );
	s.set ( "valueTemp", mValueTemp );
	s.set ( "cursorPos", mCursorPos );
	s.set ( "selectionPos", mSelectionPos );
}

bool Console::load ( Serializer &s ) {
	if ( !Widget::load ( s ) ) return false;

	if ( !s.get ( "editable", mEditable ) ) return false;

	if ( !s.get ( "spinnable", mSpinnable ) ) return false;

	if ( !s.get ( "committed", mCommitted ) ) return false;

	if ( !s.get ( "value", mValue ) ) return false;

	if ( !s.get ( "defaultValue", mDefaultValue ) ) return false;

	if ( !s.get ( "alignment", mAlignment ) ) return false;

	if ( !s.get ( "units", mUnits ) ) return false;

	if ( !s.get ( "format", mFormat ) ) return false;

	if ( !s.get ( "unitsImage", mUnitsImage ) ) return false;

	if ( !s.get ( "validFormat", mValidFormat ) ) return false;

	if ( !s.get ( "valueTemp", mValueTemp ) ) return false;

	if ( !s.get ( "cursorPos", mCursorPos ) ) return false;

	if ( !s.get ( "selectionPos", mSelectionPos ) ) return false;

	mMousePos = mMouseDownPos = mMouseDragPos = Vector2i::Constant ( -1 );
	mMouseDownModifier = mTextOffset = 0;
	return true;
}

NAMESPACE_END ( nanogui )
