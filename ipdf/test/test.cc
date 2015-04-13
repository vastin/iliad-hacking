#include <stdio.h>
#include "goo/GooList.h"
#include "poppler/GlobalParams.h"
#include "poppler/Object.h"
#include "poppler/Link.h"
#include "goo/GooString.h"
#include "poppler/PDFDoc.h"
#include "poppler/SplashOutputDev.h"
#include "splash/SplashTypes.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define AppName "xpdf"

int main(int argc, char * argv[])
{

    printf("open file %s\n", argv[1]);
    GooString * name = new GooString(argv[1]);

    
    
    globalParams = new GlobalParams("");
    
      SplashColor     white;
    white[0] = 255;
    white[1] = 255;
    white[2] = 255;
    SplashColorMode mode = splashModeRGB8;
    SplashOutputDev * outputDev = new SplashOutputDev(mode/*Packed*/, 4, gFalse, white);
    printf("outputdev %d\n",(int)outputDev);
    
     PDFDoc * pdfdoc = new PDFDoc(name);    
    
    
    if (!pdfdoc->isOk())
    {
        printf("could not open doc %s\n", argv[1]);
        return -1;
    }   
        
//    const int SCREEN_WIDTH = 768;
//    const int SCREEN_HEIGHT = 1024;
    double zoom = 200;
    double hDPI = zoom * 0.01 * 72;
    double vDPI = zoom * 0.01 * 72;
    int rotate  = 0;
    printf("start docuemnt pages %d\n", pdfdoc->getNumPages());
    outputDev->startDoc(pdfdoc->getXRef());
    printf("start document done!\n");
    struct timeval t1, t2;
    for(int i = 1; i <= pdfdoc->getNumPages(); ++i)
    {
        printf("render page %d\n", i);
        gettimeofday(&t1, NULL);
        pdfdoc->displayPage(outputDev,  i, hDPI, vDPI, rotate, gFalse, gTrue, gTrue);
        gettimeofday(&t2, NULL);
        
        float cost = (float)(((t2.tv_sec - t1.tv_sec) * 1000 * 1000) + (t2.tv_usec - t1.tv_usec));
        printf("page %d time %lf\n", i, cost/(1000*1000));
    }
       
    return 0;    

}
