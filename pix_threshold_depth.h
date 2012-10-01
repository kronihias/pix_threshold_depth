/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Clamp pixel values to a threshold

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_pix_threshold_depth_H_
#define _INCLUDE__GEM_PIXES_pix_threshold_depth_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_threshold_depth
    
    Clamp pixel values to a threshold

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a float - "ft1"
    
    "vec_thresh" - The threshold vector
    "ft1" - Set all thresholds to one value
   
-----------------------------------------------------------------*/
#ifdef _WIN32
class GEM_EXPORT pix_threshold_depth : public GemPixObj
#else
class GEM_EXTERN pix_threshold_depth : public GemPixObj
#endif
{
    CPPEXTERN_HEADER(pix_threshold_depth, GemPixObj);

    public:

        //////////
        // Constructor
    	pix_threshold_depth();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_threshold_depth();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	virtual void 	processYUVImage(imageStruct &image);
    	

    	//////////

    	//////////
    	// Set the new threshold value
    	void	    	floatHiThreshMess(float hi_thresh);
    	void	    	floatLoThreshMess(float lo_thresh);

    	void	    	floatWhiteningMess(float whitening);
			void	    	floatTrimMess(float trim);
			void	    	floatInvertMess(float arg);
			
    	//////////
    	// The new color
    	//unsigned char  	m_thresh[4];
      //  unsigned char	m_Y;
      
			// distance thresholds - min < passing < max
			float m_hi_thresh;
			float m_lo_thresh;
			
			// if true set passing color to 255 255 255 255
			bool m_whitening;
			
			// if true trim x & y
			bool m_trim;

			// if true invert colors/alpha value
			bool m_invert;
			
			// used for trimming picture
			float m_x_min;
			float m_x_max;			
			float m_y_min;
			float m_y_max;
			
    private:
    
    	//////////
    	// Static member functions
    	static void 	floatHiThreshMessCallback(void *data, t_floatarg hi_thresh);
    	static void 	floatLoThreshMessCallback(void *data, t_floatarg lo_thresh);

			static void 	floatXMinMessCallback(void *data, t_floatarg x_min);
			static void 	floatXMaxMessCallback(void *data, t_floatarg x_max);
			static void 	floatYMinMessCallback(void *data, t_floatarg y_min);
			static void 	floatYMaxMessCallback(void *data, t_floatarg y_max);
			
    	static void 	floatWhiteningMessCallback(void *data, t_floatarg whitening);
			static void 	floatTrimMessCallback(void *data, t_floatarg trim);
			static void 	floatInvertMessCallback(void *data, t_floatarg invert);
};

#endif	// for header file
