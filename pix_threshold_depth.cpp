////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


#include <math.h>

#include "pix_threshold_depth.h"


CPPEXTERN_NEW(pix_threshold_depth);

#define XtoZ 1.111466646194458
#define YtoZ 0.833599984645844

/////////////////////////////////////////////////////////
//
// pix_threshold_depth
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_threshold_depth :: pix_threshold_depth()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("whitening"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("lo_thresh"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("hi_thresh"));
    m_hi_thresh = 3000.0;
    m_lo_thresh = 10.1;
    m_whitening = false;
	m_trim = false;
    m_invert = false;
    m_usercoloring = false;
	m_x_min = -2000.0;
	m_x_max = 2000.0;
	m_y_min = -2000.0;
	m_y_max = 2000.0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_threshold_depth :: ~pix_threshold_depth()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: processRGBAImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;
    
    int value = 0;
    int userid = 0;
    while(datasize--)
    {
			value = ((int)base[chRed] << 8) + (int)base[chGreen];
        
            if (m_usercoloring)
                userid = base[chBlue];
        
            if (((value < (int)m_lo_thresh) || (value > (int)m_hi_thresh) && userid == 0))
			{
				if (!m_invert)
				{
					base[chRed] = 0;
					base[chGreen] = 0;
					base[chBlue] = 0;
					base[chAlpha] = 0;
				} else {
					base[chRed] = 255;
					base[chGreen] = 255;
					base[chBlue] = 255;
					base[chAlpha] = 255;
				}
			} else {
				
				if (m_trim)
				{
					// calculate real x/y coordinates for trimming
					// x component
					//float FovH = 1.0144686707507438;
					//float XtoZ = tan(FovH / 2.0) * 2.0;
					float pos_x = (-1) * ((datasize % image.xsize) - image.xsize);
					float real_x = ((pos_x / 640.0) - 0.5) * value * XtoZ;
				
					// y component
					//float FovV=0.78980943449644714;
					//float YtoZ = tan(FovV / 2.0) * 2.0;
					float pos_y = (-1) * (ceil((float)((datasize / image.xsize)) - image.ysize));
					float real_y = (0.5 - (pos_y / 480.0)) * value * YtoZ;
					
					if ((real_y < m_y_max) && (real_y > m_y_min) && (real_x < m_x_max) && (real_x > m_x_min))
					{
						if (m_whitening)
						{
							if (!m_invert)
							{
								base[chRed] = 255;
								base[chGreen] = 255;
								base[chBlue] = 255;
								base[chAlpha] = 255;
							} else {
								base[chRed] = 0;
								base[chGreen] = 0;
								base[chBlue] = 0;
								base[chAlpha] = 0;
							}
						}
						// just leave pixel data if pass w.o. whitening
					} else {
						base[chRed] = 0;
						base[chGreen] = 0;
						base[chBlue] = 0;
						base[chAlpha] = 0;
					}
				} else {
					if (m_whitening)
					{
						if (!m_invert)
						{
							base[chRed] = 255;
							base[chGreen] = 255;
							base[chBlue] = 255;
							base[chAlpha] = 255;
						} else {
							base[chRed] = 0;
							base[chGreen] = 0;
							base[chBlue] = 0;
							base[chAlpha] = 0;
						}
					}
				}
			}
		base += 4;
    }    
}

/////////////////////////////////////////////////////////
// YUV Depth Image --> Has to be corrected
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: processYUVImage(imageStruct &image)
{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;
		int value = 0;
    while(datasize--)
    {
			value = ((int)base[chRed] << 8) + (int)base[chGreen];
			if ((value < (int)m_lo_thresh) || (value > (int)m_hi_thresh))
			{
				base[chRed] = 0;
				base[chGreen] = 0;
			} else {
				
				if (m_trim)
				{
					// calculate real x/y coordinates for trimming
					// x component
					//float FovH = 1.0144686707507438;
					//float XtoZ = tan(FovH / 2.0) * 2.0;
					float pos_x = (-1) * ((datasize % image.xsize) - image.xsize);
					float real_x = ((pos_x / 640.0) - 0.5) * value * XtoZ;
				
					// y component
					//float FovV=0.78980943449644714;
					//float YtoZ = tan(FovV / 2.0) * 2.0;
					float pos_y = (-1) * (ceil((float)((datasize / image.xsize)) - image.ysize));
					float real_y = (0.5 - (pos_y / 480.0)) * value * YtoZ;
					
					if ((real_y < m_y_max) && (real_y > m_y_min) && (real_x < m_x_max) && (real_x > m_x_min))
					{
						if (m_whitening)
						{
							base[chRed] = 255;
							base[chGreen] = 255;
						}
						// just leave pixel data if pass w.o. whitening
					} else {
						base[chRed] = 0;
						base[chGreen] = 0;
					}
				} else {
					if (m_whitening)
					{
						base[chRed] = 255;
						base[chGreen] = 255;
					}
				}
			}
		base += 2;
    }    
}
/////////////////////////////////////////////////////////
// floatHiThreshMess
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: floatHiThreshMess(float hi_thresh)
{
    if (hi_thresh >= 0)
    {
			m_hi_thresh = hi_thresh;
			verbose (1, "high threshold set to %f", m_hi_thresh);
		}
}

/////////////////////////////////////////////////////////
// floatLoThreshMess
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: floatLoThreshMess(float lo_thresh)
{
    if (lo_thresh >= 0)
    {
			m_lo_thresh = lo_thresh;
			verbose (1, "low threshold set to %f", m_lo_thresh);
		}
}

/////////////////////////////////////////////////////////
// floatWhiteningMess
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: floatWhiteningMess(float whitening)
{
	if (whitening < 0.5)
	{
		m_whitening = false;
	} else {
		m_whitening = true;
	}
}

void pix_threshold_depth :: floatTrimMess(float trim)
{
	if (trim < 0.5)
	{
		m_trim = false;
	} else {
		m_trim = true;
	}
}

void pix_threshold_depth :: floatInvertMess(float arg)
{
	if (arg < 0.5)
	{
		m_invert = false;
	} else {
		m_invert = true;
	}
}

void pix_threshold_depth :: floatUsercoloringMess(float arg)
{
	if (arg < 0.5)
	{
		m_usercoloring = false;
	} else {
		m_usercoloring = true;
	}
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_threshold_depth :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatLoThreshMessCallback),
    	    gensym("lo_thresh"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatHiThreshMessCallback),
    	    gensym("hi_thresh"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatYMinMessCallback),
    	    gensym("y_min"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatYMaxMessCallback),
    	    gensym("y_max"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatXMinMessCallback),
    	    gensym("x_min"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatXMaxMessCallback),
    	    gensym("x_max"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatWhiteningMessCallback),
    	    gensym("whitening"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatTrimMessCallback),
		    	gensym("trim"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatInvertMessCallback),
		    	gensym("invert"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_threshold_depth::floatUsercoloringMessCallback),
                    gensym("usercoloring"), A_FLOAT, A_NULL);
}
void pix_threshold_depth :: floatLoThreshMessCallback(void *data, t_floatarg lo_thresh)
{
    GetMyClass(data)->floatLoThreshMess((float)lo_thresh);
}
void pix_threshold_depth :: floatHiThreshMessCallback(void *data, t_floatarg hi_thresh)
{
    GetMyClass(data)->floatHiThreshMess((float)hi_thresh);
}

void pix_threshold_depth :: floatYMinMessCallback(void *data, t_floatarg y_min)
{
		GetMyClass(data)->m_y_min=((float)y_min);
}
void pix_threshold_depth :: floatYMaxMessCallback(void *data, t_floatarg y_max)
{
    GetMyClass(data)->m_y_max=((float)y_max);
}
void pix_threshold_depth :: floatXMinMessCallback(void *data, t_floatarg x_min)
{
    GetMyClass(data)->m_x_min=((float)x_min);
}
void pix_threshold_depth :: floatXMaxMessCallback(void *data, t_floatarg x_max)
{
    GetMyClass(data)->m_x_max=((float)x_max);
}
void pix_threshold_depth :: floatWhiteningMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatWhiteningMess((float)arg);
}
void pix_threshold_depth :: floatTrimMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatTrimMess((float)arg);
}
void pix_threshold_depth :: floatInvertMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatInvertMess((float)arg);
}
void pix_threshold_depth :: floatUsercoloringMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatUsercoloringMess((float)arg);
}