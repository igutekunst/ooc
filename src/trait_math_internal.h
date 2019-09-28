#ifndef OOC_TRAIT_MATH_INTERNAL_H
#define OOC_TRAIT_MATH_INTERNAL_H
/**
 * This file defines the functions and data members required
 * by the math trait.
 */


struct OOCTraitMath {
    bool enabled;
    const void* (*add)(const void* lhs, const void* rhs);
    const void* (*sub)(const void* lhs, const void* rhs);
    const void* (*mul)(const void* lhs, const void* rhs);
    const void* (*div)(const void* lhs, const void* rhs);
    int (*to_int)(const void* _self);
};




#endif
