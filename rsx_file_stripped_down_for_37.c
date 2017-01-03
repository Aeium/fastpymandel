////////////////////////////////////////////////////////////////
// rsx_filt.c
////////////////////////////////////////////////////////////////
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "rsx_file.h"

int CALLBACK _filter(double zx, double zy, int _ncolor)
{
	n_color = _ncolor;
	__real__ z = zx;
	__imag__ z = zy;
	Do_Filter();
	return n_color;
}

//////////////////////////////////////////////////////
// Filters
//////////////////////////////////////////////////////

void Do_Filter()
{
	switch (filter)
	{
		

	  case 37:
			if (fabs(__real__ z) < dStrands_HI_cx && 
					fabs(__real__ z) > dStrands_LO_cx)
			{
				n_color_z++;				
				pXTemp[n_color] = 1 - (dStrands_HI_cx - fabs(__real__ z))/(dStrands_cxd);
			  xsav = __real__ z;
			}			
			else
			{
				pXTemp[n_color] = 1;
			}

			if (fabs(__imag__ z) < dStrands_HI_cy &&
					fabs(__imag__ z) > dStrands_LO_cy)
			{
				n_color_z++;				
				pYTemp[n_color] = 1 - (dStrands_HI_cy - fabs(__imag__ z))/(dStrands_cyd);
			  ysav = __imag__ z;		 
			}
			else
			{
				pYTemp[n_color] = 1;
			}

		 break;

	  
 }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

RGB_IDATA CALLBACK _filter_complete()
{
	__real__ z = xsav;
	__imag__ z = ysav;

	Filter_Complete();

	if (fabs(xsav) < minsize)
	{
		if (xsav > 0)
			xsav = minsize;
		else
			xsav = -minsize;
	}

	if (fabs(ysav) < minsize)
	{
		if (ysav > 0)
			ysav = minsize;
		else
			ysav = -minsize;
	}

	if (fabs(xsav) > maxsize)
	{
		if (xsav > 0)
			xsav = maxsize;
		else
			xsav = -maxsize;
	}

	if (fabs(ysav) > maxsize)
	{
		if (ysav > 0)
			ysav = maxsize;
		else
			ysav = -maxsize;
	}

	switch (filter)
	{		
		case 27:  // Check for a fractal Dimension filter
		case 28:  // standard deviation filter
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 48:
		case 49:

			FDimension();
			break;

		default:
			break;
	}

	if (size_x <= 640 && size_y <= 480 && nUsingBuffers)
	{
		pXSave[px+py*size_x] = xsav;
		pYSave[px+py*size_x] = ysav;
		
		/*
		if (px == 1 && py == 1)
		{
			sprintf(cstr, "step 1, xsav = %f, ysav=%f, i = %d, rj = %f, gj = %f",
				xsav, ysav, i, rj, gj);
			MessageBox(NULL, cstr, "bug", MB_OK);
		}
		*/
				
		rjData[px+py*size_x] = rj;
		gjData[px+py*size_x] = gj;
		bjData[px+py*size_x] = bj;
	}

	if (nColorMethod)	
		color_methods();
	
	//MessageBox(NULL, "step 5", "bug", MB_OK);

	rgbColor.i  = j;
	rgbColor.rj	= rj;
	rgbColor.gj	= gj;
	rgbColor.bj	= bj;

	return rgbColor;

}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void Filter_Complete()
{
	static int nCount = 0;
	
	j = rj = gj = bj = n_color;
  
	switch (filter)
  {
    
		case 37:
			if (n_color_z)
				j = n_color;
			else
				j = 0;
			break;

		default:
		  break;
  }
}

