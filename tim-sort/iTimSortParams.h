#ifndef _ITimSortParams_h
#define _ITimSortParams_h

#include "getMinrun.h"

enum EWhatMerge {WM_NoMerge, WM_MergeXY, WM_MergeYZ};

class  ITimSortParams {
    
public:
    // Вычисление minRun по n.
    virtual unsigned int minRun(unsigned int n) const = 0;
    // Нужно ли выполнять слияние, если в стеке ровно два рана размеров lenX и lenY.
    virtual bool needMerge(unsigned int lenX, unsigned int lenY) const = 0;
    // Как сливать, если в стеке три последних рана имеют размеры lenX, lenY и lenZ.
    virtual EWhatMerge whatMerge(unsigned int lenX, unsigned int lenY, unsigned int lenZ) const = 0;
    // Когда в merge искать бинпоиском, сколько элементов меньше заданного.
    virtual unsigned int GetGallop() const = 0;
};



class defaultParams : public ITimSortParams {

public:
    
    unsigned int minRun(unsigned int n) const {
        return getMinRun(n);
    }
    
    bool needMerge(unsigned int lenX, unsigned int lenY) const {
        return true;
    }
    
    EWhatMerge whatMerge(unsigned int lenX, unsigned int lenY, unsigned int lenZ) const {
        if (lenX < lenY + lenZ) {
            return WM_MergeXY;
        } else {
            return lenY < lenZ ? WM_MergeYZ : WM_NoMerge;
        }
    }
    
    unsigned int GetGallop() const {
        return 7;
    }
};

#endif /* _ITimSortParams_h */
