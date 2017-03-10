/*
* @Author: kmrocki@us.ibm.com
* @Date:   2017-03-09 21:00:38
* @Last Modified by:   kmrocki@us.ibm.com
* @Last Modified time: 2017-03-09 22:16:24
*/

#pragma once

#include <nanogui/common.h>
#include <functional>

NAMESPACE_BEGIN ( nanogui )

const int BARCOLOR_LUT_SIZE = 10;
const Color barcolormap_lut[] = {

	Color(255, 0, 0, 127),
	Color(255, 140, 0, 127),
	Color(229, 255, 0, 127),
	Color(51, 255, 92, 127),
	Color(51, 255, 204, 127),
	Color(153, 224, 255, 127),
	Color(153, 168, 255, 127),
	Color(51, 82, 255, 127),
	Color(242, 0, 255, 127),
	Color(255, 102, 179, 127)

};

const int PARULA_LUT_SIZE = 10;
const Color parula_lut[] = {

	Color(53, 42, 135, 127),
	Color(15, 92, 221, 127),
	Color(18, 125, 216, 127),
	Color(7, 156, 207, 127),
	Color(21, 177, 180, 127),
	Color(89, 189, 140, 127),
	Color(165, 190, 107, 127),
	Color(225, 185, 82, 127),
	Color(252, 206, 46, 127),
	Color(249, 251, 14, 127)

};

// jet colormap functions
float interpolate_jet( float val, float y0, float x0, float y1, float x1 );
float jet_base (float value );
float jet_red( float value );
float jet_green( float value );
float jet_blue( float value );

// colormaps
Color jet_colormap(float value);
Color parula_colormap(float value);
Color bar_colormap(float value);

NAMESPACE_END ( nanogui )