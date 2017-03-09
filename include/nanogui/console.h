/*
* @Author: Kamil Rocki
* @Date:   2017-03-01 09:56:14
* @Last Modified by:   Kamil Rocki
* @Last Modified time: 2017-03-01 10:07:11
*/

/** \file */

#pragma once

#include <nanogui/compat.h>
#include <nanogui/widget.h>
#include <sstream>

NAMESPACE_BEGIN ( nanogui )

/**
 * \class Console console.h nanogui/console.h
 *
 * \brief Fancy text box with builtin regular expression-based validation.
 */
class NANOGUI_EXPORT Console : public Widget {
  public:
	/// How to align the text in the text box.
	enum class Alignment {
		Left,
		Center,
		Right
	};

	Console ( Widget *parent, const std::string &value = "Untitled" );

	bool editable() const { return mEditable; }
	void setEditable ( bool editable );

	bool spinnable() const { return mSpinnable; }
	void setSpinnable ( bool spinnable ) { mSpinnable = spinnable; }

	const std::string &value() const { return mValue; }
	void setValue ( const std::string &value ) { mValue = value; }

	const std::string &defaultValue() const { return mDefaultValue; }
	void setDefaultValue ( const std::string &defaultValue ) { mDefaultValue = defaultValue; }

	Alignment alignment() const { return mAlignment; }
	void setAlignment ( Alignment align ) { mAlignment = align; }

	const std::string &units() const { return mUnits; }
	void setUnits ( const std::string &units ) { mUnits = units; }

	int unitsImage() const { return mUnitsImage; }
	void setUnitsImage ( int image ) { mUnitsImage = image; }

	/// Return the underlying regular expression specifying valid formats
	const std::string &format() const { return mFormat; }
	/// Specify a regular expression specifying valid formats
	void setFormat ( const std::string &format ) { mFormat = format; }

	/// Set the \ref Theme used to draw this widget
	virtual void setTheme ( Theme *theme ) override;

	/// Set the change callback
	std::function<bool ( const std::string &str )> callback() const { return mCallback; }
	void setCallback ( const std::function<bool ( const std::string &str )> &callback ) { mCallback = callback; }

	virtual bool mouseButtonEvent ( const Vector2i &p, int button, bool down, int modifiers ) override;
	virtual bool mouseMotionEvent ( const Vector2i &p, const Vector2i &rel, int button, int modifiers ) override;
	virtual bool mouseDragEvent ( const Vector2i &p, const Vector2i &rel, int button, int modifiers ) override;
	virtual bool focusEvent ( bool focused ) override;
	virtual bool keyboardEvent ( int key, int scancode, int action, int modifiers ) override;
	virtual bool keyboardCharacterEvent ( unsigned int codepoint ) override;

	virtual Vector2i preferredSize ( NVGcontext *ctx ) const override;
	virtual void draw ( NVGcontext *ctx ) override;
	virtual void save ( Serializer &s ) const override;
	virtual bool load ( Serializer &s ) override;
  protected:
	bool checkFormat ( const std::string &input, const std::string &format );
	bool copySelection();
	void pasteFromClipboard();
	bool deleteSelection();

	void updateCursor ( NVGcontext *ctx, float lastx,
	                    const NVGglyphPosition *glyphs, int size );
	float cursorIndex2Position ( int index, float lastx,
	                             const NVGglyphPosition *glyphs, int size );
	int position2CursorIndex ( float posx, float lastx,
	                           const NVGglyphPosition *glyphs, int size );

	/// The location (if any) for the spin area.
	enum class SpinArea { None, Top, Bottom };
	SpinArea spinArea ( const Vector2i &pos );

  protected:
	bool mEditable;
	bool mSpinnable;
	bool mCommitted;
	std::string mValue;
	std::string mDefaultValue;
	Alignment mAlignment;
	std::string mUnits;
	std::string mFormat;
	int mUnitsImage;
	std::function<bool ( const std::string &str )> mCallback;
	bool mValidFormat;
	std::string mValueTemp;
	int mCursorPos;
	int mSelectionPos;
	Vector2i mMousePos;
	Vector2i mMouseDownPos;
	Vector2i mMouseDragPos;
	int mMouseDownModifier;
	float mTextOffset;
	double mLastClick;
  public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

NAMESPACE_END ( nanogui )
