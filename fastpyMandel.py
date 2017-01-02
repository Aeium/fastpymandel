
# great blog post https://www.ibm.com/developerworks/community/blogs/jfp/entry/How_To_Compute_Mandelbrodt_Set_Quickly?lang=en

import numpy as np
from numba import jit, vectorize, guvectorize, float64, complex64, int32, float32
from matplotlib import pyplot as plt
from matplotlib import colors
from matplotlib import image
from pylab import imshow, show


@jit(int32(complex64, int32, float32))
def mandelbrot(c, maxiter, extra=0):
    z = c
    distance = 2**40
    for n in range(maxiter):
        #if abs(z) > 2:
        #    return n
        z = z**(2) + z**(z+extra) + c
        if z.imag < distance and z.imag > 0:
            distance = z.imag
        if z.imag*-1 < distance and z.imag < 0:
            distance = z.imag*-1
        if z.real < distance and z.real > 0:
            distance = z.real
        if z.real*-1 < distance and z.real < 0:
            distance = z.real*-1
    
    return distance * -1000

@guvectorize([(complex64[:], int32[:], float32[:], int32[:])], '(n),(),()->(n)',target='parallel')
def mandelbrot_numpy(c, maxit, extra, output):
    maxiter = maxit[0]
    extraVal = extra[0]
    for i in range(c.shape[0]):
        output[i] = mandelbrot(c[i],maxiter, extraVal)
        
def mandelbrot_set2(xmin,xmax,ymin,ymax,width,height,maxiter, extra):
    r1 = np.linspace(xmin, xmax, width, dtype=np.float32)
    r2 = np.linspace(ymin, ymax, height, dtype=np.float32)
    c = r1 + r2[:,None]*1j
    n3 = mandelbrot_numpy(c,maxiter, extra)
    return (r1,r2,n3.T) 

def mandelbrot_image(xmin,xmax,ymin,ymax,width=10,height=10,maxiter=256,cmap='jet', filename='mandel', extra=0):
    dpi = 72
    img_width = dpi * width
    img_height = dpi * height
    x,y,z = mandelbrot_set2(xmin,xmax,ymin,ymax,img_width,img_height,maxiter, extra)
    
    fig, ax = plt.subplots(figsize=(width, height),dpi=72)
    ticks = np.arange(0,img_width,3*dpi)
    x_ticks = xmin + (xmax-xmin)*ticks/img_width
    plt.xticks(ticks, x_ticks)
    y_ticks = ymin + (ymax-ymin)*ticks/img_width
    plt.yticks(ticks, y_ticks)
    
    img = imshow(z.T, origin='lower')
    img.set_cmap(cmap)
    img.write_png('%s.png' % filename, noscale=True)
    #show()
    
    
    #image.save_image(fig)



for i in range(1396,2000):
    print("saving image %s" %i)
    mandelbrot_image(-.3,.2,.5,1, maxiter=1000, filename= 'animation2/mandel_%s' % i, extra = i/900.0 + .0001 ,cmap='hot')
