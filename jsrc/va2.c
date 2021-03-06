/* Copyright 1990-2011, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Atomic (Scalar) Dyadic                                           */

#include "j.h"
#include "ve.h"
#include "vcomp.h"

// shorthands for inplacing
#define VIP (VIPOKA+VIPOKW)   // inplace everything
#define VIPI0 ((VIPOKA*(sizeof(I)==sizeof(D))))  // inplace left arg if D is same length as I
#define VIP0I ((VIPOKW*(sizeof(I)==sizeof(D))))  // inplace right arg if D is same length as I
#define VIPID (VIPI0+VIPOKW)  // inplace D, and I if I is same length as D: I is left arg
#define VIPDI (VIP0I+VIPOKA)  // inplace D, and I if I is same length as D: I is right arg

static VA va[]={
/* non-atomic functions      */ {
 {{0,0}, {0,0}, {0,0},                                /* BB BI BD              */
  {0,0}, {0,0}, {0,0},                                /* IB II ID              */
  {0,0}, {0,0}, {0,0},                                /* DB DI DD              */
  {0,0}, {0,0}, {0,0}, {0,0}},                        /* ZZ XX QQ Symb         */
 {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},   /* ins: B I D Z X Q Symb */
 {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},   /* pfx: B I D Z X Q Symb */
 {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}} }, /* sfx: B I D Z X Q Symb */

/* 10    */ {
 {{(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP}, 
  {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VIP},     {(VF)bw0000II,  VI+VII+VIP},
  {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP},
  {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP}, {(VF)bw0000II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0000insI,VI+VII}, {(VF)bw0000insI,VI}},
 {{(VF)bw0000pfxI,VI+VII}, {(VF)bw0000pfxI,VI}},
 {{(VF)bw0000sfxI,VI+VII}, {(VF)bw0000sfxI,VI}} },

/* 11    */ {
 {{(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP}, 
  {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VIP},     {(VF)bw0001II,  VI+VII+VIP},
  {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP},
  {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP}, {(VF)bw0001II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0001insI,VI+VII}, {(VF)bw0001insI,VI}},
 {{(VF)bw0001pfxI,VI+VII}, {(VF)bw0001pfxI,VI}},
 {{(VF)bw0001sfxI,VI+VII}, {(VF)bw0001sfxI,VI}} },

/* 12    */ {
 {{(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP}, 
  {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VIP},     {(VF)bw0010II,  VI+VII+VIP},
  {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP},
  {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP}, {(VF)bw0010II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0010insI,VI+VII}, {(VF)bw0010insI,VI}},
 {{0,0}},
 {{(VF)bw0010sfxI,VI+VII}, {(VF)bw0010sfxI,VI}} },

/* 13    */ {
 {{(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP}, 
  {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VIP},     {(VF)bw0011II,  VI+VII+VIP},
  {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP},
  {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP}, {(VF)bw0011II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0011insI,VI+VII}, {(VF)bw0011insI,VI}},
 {{(VF)bw0011pfxI,VI+VII}, {(VF)bw0011pfxI,VI}},
 {{(VF)bw0011sfxI,VI+VII}, {(VF)bw0011sfxI,VI}} },

/* 14    */ {
 {{(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP}, 
  {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VIP},     {(VF)bw0100II,  VI+VII+VIP},
  {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP},
  {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP}, {(VF)bw0100II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0100insI,VI+VII}, {(VF)bw0100insI,VI}},
 {{0,0}},
 {{(VF)bw0100sfxI,VI+VII}, {(VF)bw0100sfxI,VI}} },

/* 15    */ {
 {{(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP}, 
  {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VIP},     {(VF)bw0101II,  VI+VII+VIP},
  {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP},
  {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP}, {(VF)bw0101II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0101insI,VI+VII}, {(VF)bw0101insI,VI}},
 {{(VF)bw0101pfxI,VI+VII}, {(VF)bw0101pfxI,VI}},
 {{(VF)bw0101sfxI,VI+VII}, {(VF)bw0101sfxI,VI}} },

/* 16    */ {
 {{(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP}, 
  {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VIP},     {(VF)bw0110II,  VI+VII+VIP},
  {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP},
  {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP}, {(VF)bw0110II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0110insI,VI+VII}, {(VF)bw0110insI,VI}},
 {{(VF)bw0110pfxI,VI+VII}, {(VF)bw0110pfxI,VI}},
 {{(VF)bw0110sfxI,VI+VII}, {(VF)bw0110sfxI,VI}} },

/* 17    */ {
 {{(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP}, 
  {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VIP},     {(VF)bw0111II,  VI+VII+VIP},
  {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP},
  {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP}, {(VF)bw0111II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw0111insI,VI+VII}, {(VF)bw0111insI,VI}},
 {{(VF)bw0111pfxI,VI+VII}, {(VF)bw0111pfxI,VI}},
 {{(VF)bw0111sfxI,VI+VII}, {(VF)bw0111sfxI,VI}} },

/* 18    */ {
 {{(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP}, 
  {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VIP},     {(VF)bw1000II,  VI+VII+VIP},
  {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP},
  {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP}, {(VF)bw1000II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1000insI,VI+VII}, {(VF)bw1000insI,VI}},
 {{0,0}},
 {{(VF)bw1000sfxI,VI+VII}, {(VF)bw1000sfxI,VI}} },

/* 19    */ {
 {{(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP}, 
  {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VIP},     {(VF)bw1001II,  VI+VII+VIP},
  {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP},
  {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP}, {(VF)bw1001II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1001insI,VI+VII}, {(VF)bw1001insI,VI}},
 {{(VF)bw1001pfxI,VI+VII}, {(VF)bw1001pfxI,VI}},
 {{(VF)bw1001sfxI,VI+VII}, {(VF)bw1001sfxI,VI}} },

/* 1a    */ {
 {{(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP}, 
  {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VIP},     {(VF)bw1010II,  VI+VII+VIP},
  {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP},
  {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP}, {(VF)bw1010II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1010insI,VI+VII}, {(VF)bw1010insI,VI}},
 {{0,0}},
 {{(VF)bw1010sfxI,VI+VII}, {(VF)bw1010sfxI,VI}} },

/* 1b    */ {
 {{(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP}, 
  {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VIP},     {(VF)bw1011II,  VI+VII+VIP},
  {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP},
  {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP}, {(VF)bw1011II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1011insI,VI+VII}, {(VF)bw1011insI,VI}},
 {{0,0}},
 {{(VF)bw1011sfxI,VI+VII}, {(VF)bw1011sfxI,VI}} },

/* 1c    */ {
 {{(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP}, 
  {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VIP},     {(VF)bw1100II,  VI+VII+VIP},
  {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP},
  {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP}, {(VF)bw1100II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1100insI,VI+VII}, {(VF)bw1100insI,VI}},
 {{0,0}},
 {{(VF)bw1100sfxI,VI+VII}, {(VF)bw1100sfxI,VI}} },

/* 1d    */ {
 {{(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP}, 
  {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VIP},     {(VF)bw1101II,  VI+VII+VIP},
  {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP},
  {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP}, {(VF)bw1101II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1101insI,VI+VII}, {(VF)bw1101insI,VI}},
 {{0,0}},
 {{(VF)bw1101sfxI,VI+VII}, {(VF)bw1101sfxI,VI}} },

/* 1e    */ {
 {{(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP}, 
  {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VIP},     {(VF)bw1110II,  VI+VII+VIP},
  {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP},
  {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP}, {(VF)bw1110II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1110insI,VI+VII}, {(VF)bw1110insI,VI}},
 {{0,0}},
 {{(VF)bw1110sfxI,VI+VII}, {(VF)bw1110sfxI,VI}} },

/* 1f    */ {
 {{(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP}, 
  {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VIP},     {(VF)bw1111II,  VI+VII+VIP},
  {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP},
  {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP}, {(VF)bw1111II,  VI+VII+VIP}, {0,0}}, 
 {{(VF)bw1111insI,VI+VII}, {(VF)bw1111insI,VI}},
 {{(VF)bw1111pfxI,VI+VII}, {(VF)bw1111pfxI,VI}},
 {{(VF)bw1111sfxI,VI+VII}, {(VF)bw1111sfxI,VI}} },

/* 21 !  */ {
 {{(VF)leBB, VB+VIP            }, {(VF)binDD,VD+VDD+VRI+VIP}, {(VF)binDD,VD+VDD+VIP}, 
  {(VF)binDD,VD+VDD+VRI+VIP}, {(VF)binDD,VD+VDD+VRI+VIP}, {(VF)binDD,VD+VDD+VIP}, 
  {(VF)binDD,VD+VDD+VIP    }, {(VF)binDD,VD+VDD+VIP    }, {(VF)binDD,VD+VIP    }, 
  {(VF)binZZ,VZ+VZZ}, {(VF)binXX,VX+VXX}, {(VF)binQQ,VX+VQQ}, {0,0}}, 
 {{(VF)leinsB,VB}},
 {{(VF)lepfxB,VB}},
 {{(VF)lesfxB,VB}} },

/* 25 %  */ {
 {{(VF)divBB,VD}, {(VF)divBI,VD+VIP0I}, {(VF)divBD,VD+VIPOKW},
  {(VF)divIB,VD+VIPI0}, {(VF)divII,VD+VIPI0+VIP0I}, {(VF)divID,VD+VIPID},
  {(VF)divDB,VD+VIPOKA}, {(VF)divDI,VD+VIPDI}, {(VF)divDD,VD+VIP+VCANHALT}, 
  {(VF)divZZ,VZ+VZZ}, {(VF)divXX,VX+VXX}, {(VF)divQQ,VQ+VQQ}, {0,0}},
 {{(VF)divinsD,VD+VDD}, {(VF)divinsD,VD+VDD}, {(VF)divinsD,VD}, {(VF)divinsZ,VZ}},
 {{(VF)divpfxD,VD+VDD}, {(VF)divpfxD,VD+VDD}, {(VF)divpfxD,VD}, {(VF)divpfxZ,VZ}},
 {{(VF)divsfxD,VD+VDD}, {(VF)divsfxD,VD+VDD}, {(VF)divsfxD,VD}, {(VF)divsfxZ,VZ}} },

/* 2a *  */ {
 {{(VF)andBB,  VB+VIP}, {(VF)tymesBI,VI+VIPOKW}, {(VF)tymesBD,VD+VIPOKW},
  {(VF)tymesIB,VI+VIPOKA}, {(VF)tymesII,VI+VIP}, {(VF)tymesID,VD+VIPID},
  {(VF)tymesDB,VD+VIPOKA}, {(VF)tymesDI,VD+VIPDI}, {(VF)tymesDD,VD+VIP}, 
  {(VF)tymesZZ,VZ+VZZ}, {(VF)tymesXX,VX+VXX}, {(VF)tymesQQ,VQ+VQQ}, {0,0}},
 {{(VF)andinsB,VB}, {(VF)tymesinsI,VI}, {(VF)tymesinsD,VD}, {(VF)tymesinsZ,VZ}, {0,0},          {0,0},          {0,0}},
 {{(VF)andpfxB,VB}, {(VF)tymespfxI,VI}, {(VF)tymespfxD,VD}, {(VF)tymespfxZ,VZ}, {(VF)tymespfxX,VX}, {(VF)tymespfxQ,VQ}, {0,0}},
 {{(VF)andsfxB,VB}, {(VF)tymessfxI,VI}, {(VF)tymessfxD,VD}, {(VF)tymessfxZ,VZ}, {(VF)tymessfxX,VX}, {(VF)tymessfxQ,VQ}, {0,0}} },

/* 2b +  */ {
 {{(VF)plusBB,VI    }, {(VF)plusBI,VI+VIPOKW}, {(VF)plusBD,VD+VIPOKW}, 
  {(VF)plusIB,VI+VIPOKA}, {(VF)plusII,VI+VIP}, {(VF)plusID,VD+VIPID}, 
  {(VF)plusDB,VD+VIPOKA    }, {(VF)plusDI,VD+VIPDI    }, {(VF)plusDD,VD+VIP+VCANHALT}, 
  {(VF)plusZZ,VZ+VZZ+VIP}, {(VF)plusXX,VX+VXX}, {(VF)plusQQ,VQ+VQQ}, {0,0}},
 {{(VF)plusinsB,VI}, {(VF)plusinsI,VI}, {(VF)plusinsD,VD}, {(VF)plusinsZ,VZ}, {0,0},         {0,0},         {0,0}},
 {{(VF)pluspfxB,VI}, {(VF)pluspfxI,VI}, {(VF)pluspfxD,VD}, {(VF)pluspfxZ,VZ}, {(VF)pluspfxX,VX}, {(VF)pluspfxQ,VQ}, {0,0}},
 {{(VF)plussfxB,VI}, {(VF)plussfxI,VI}, {(VF)plussfxD,VD}, {(VF)plussfxZ,VZ}, {(VF)plussfxX,VX}, {(VF)plussfxQ,VQ}, {0,0}} },

/* 2d -  */ {
 {{(VF)minusBB,VI    }, {(VF)minusBI,VI+VIPOKW}, {(VF)minusBD,VD+VIPOKW}, 
  {(VF)minusIB,VI+VIPOKA}, {(VF)minusII,VI+VIP}, {(VF)minusID,VD+VIPID},
  {(VF)minusDB,VD+VIPOKA    }, {(VF)minusDI,VD+VIPDI    }, {(VF)minusDD,VD+VIP+VCANHALT}, 
  {(VF)minusZZ,VZ+VZZ+VIP}, {(VF)minusXX,VX+VXX}, {(VF)minusQQ,VQ+VQQ}, {0,0}},
 {{(VF)minusinsB,VI}, {(VF)minusinsI,VI}, {(VF)minusinsD,VD}, {(VF)minusinsZ,VZ}, {0,0},          {0,0},          {0,0}},
 {{(VF)minuspfxB,VI}, {(VF)minuspfxI,VI}, {(VF)minuspfxD,VD}, {(VF)minuspfxZ,VZ}, {(VF)minuspfxX,VX}, {(VF)minuspfxQ,VQ}, {0,0}},
 {{(VF)minussfxB,VI}, {(VF)minussfxI,VI}, {(VF)minussfxD,VD}, {(VF)minussfxZ,VZ}, {0,0},          {0,0},          {0,0}} },

   // For Booleans, VIP means 'inplace if rank not specified and there is no frame'
/* 3c <  */ {
 {{(VF)ltBB,VB+VIP}, {(VF)ltBI,VB+VIPOKA}, {(VF)ltBD,VB+VIPOKA},
  {(VF)ltIB,VB+VIPOKW}, {(VF)ltII,VB}, {(VF)ltID,VB},
  {(VF)ltDB,VB+VIPOKW}, {(VF)ltDI,VB}, {(VF)ltDD,VB}, 
  {(VF)ltDD,VB+VDD+VIP}, {(VF)ltXX,VB+VXFC}, {(VF)ltQQ,VB+VQQ}, {(VF)ltSS,VB}},
 {{(VF)ltinsB,VB}},
 {{(VF)ltpfxB,VB}},
 {{(VF)ltsfxB,VB}} },

/* 3d =  */ {
 {{(VF)eqBB,VB+VIP}, {(VF)eqBI,VB+VIPOKA}, {(VF)eqBD,VB+VIPOKA},
  {(VF)eqIB,VB+VIPOKW}, {(VF)eqII,VB}, {(VF)eqID,VB},
  {(VF)eqDB,VB+VIPOKW}, {(VF)eqDI,VB}, {(VF)eqDD,VB}, 
  {(VF)eqZZ,VB+VZZ}, {(VF)eqXX,VB+VXEQ}, {(VF)eqQQ,VB+VQQ}, {(VF)eqII,VB}},
 {{(VF)eqinsB,VB}},
 {{(VF)eqpfxB,VB}},
 {{(VF)eqsfxB,VB}} },

/* 3e >  */ {
 {{(VF)gtBB,VB+VIP}, {(VF)gtBI,VB+VIPOKA}, {(VF)gtBD,VB+VIPOKA},
  {(VF)gtIB,VB+VIPOKW}, {(VF)gtII,VB}, {(VF)gtID,VB},
  {(VF)gtDB,VB+VIPOKW}, {(VF)gtDI,VB}, {(VF)gtDD,VB}, 
  {(VF)gtDD,VB+VDD+VIP}, {(VF)gtXX,VB+VXCF}, {(VF)gtQQ,VB+VQQ}, {(VF)gtSS,VB}},
 {{(VF)gtinsB,VB}},
 {{(VF)gtpfxB,VB}},
 {{(VF)gtsfxB,VB}} },

/* 5e ^  */ {   // may produce complex numbers
 {{(VF)geBB, VB+VIP}, {(VF)powBI,VD}, {(VF)powBD,VD},
  {(VF)powIB,VI}, {(VF)powII,VD}, {(VF)powID,VD+VCANHALT},
  {(VF)powDB,VD}, {(VF)powDI,VD}, {(VF)powDD,VD+VCANHALT}, 
  {(VF)powZZ,VZ+VZZ}, {(VF)powXX,VX+VXX}, {(VF)powQQ,VQ+VQQ}, {0,0}},
 {{(VF)geinsB,VB}},
 {{(VF)gepfxB,VB}},
 {{(VF)gesfxB,VB}} },

/* 7c |  */ {
 {{(VF)ltBB, VB+VIP    }, {(VF)remII,VI+VII+VIP}, {(VF)remDD,VD+VDD+VIP},
  {(VF)remII,VI+VII+VIP}, {(VF)remII,VI+VIP    }, {(VF)remID,VI+VCANHALT    },   // remID can 'overflow' if result is nonintegral
  {(VF)remDD,VD+VDD+VIP}, {(VF)remDD,VD+VDD+VIP}, {(VF)remDD,VD+VIP+VCANHALT}, 
  {(VF)remZZ,VZ+VZZ}, {(VF)remXX,VX+VXX}, {(VF)remQQ,VQ+VQQ}, {0,0}},
 {{(VF)ltinsB,VB}},
 {{(VF)ltpfxB,VB}},
 {{(VF)ltsfxB,VB}} },

/* 82 <. */ {
 {{(VF)andBB,VB+VIP}, {(VF)minBI,VI+VIPOKW}, {(VF)minBD,VD+VIPOKW},
  {(VF)minIB,VI+VIPOKA}, {(VF)minII,VI+VIP}, {(VF)minID,VD+VIPID},
  {(VF)minDB,VD+VIPOKA}, {(VF)minDI,VD+VIPDI}, {(VF)minDD,VD+VIP}, 
  {(VF)minDD,VD+VDD+VIP}, {(VF)minXX,VX+VXX}, {(VF)minQQ,VQ+VQQ}, {(VF)minSS,VSB}},  // always VIP a forced conversion
 {{(VF)andinsB,VB}, {(VF)mininsI,VI}, {(VF)mininsD,VD}, {(VF)mininsD,VD+VDD}, {(VF)mininsX,VX}, {0,0},        {(VF)mininsS,VSB}},
 {{(VF)andpfxB,VB}, {(VF)minpfxI,VI}, {(VF)minpfxD,VD}, {(VF)minpfxD,VD+VDD}, {(VF)minpfxX,VX}, {(VF)minpfxQ,VQ}, {(VF)minpfxS,VSB}},
 {{(VF)andsfxB,VB}, {(VF)minsfxI,VI}, {(VF)minsfxD,VD}, {(VF)minsfxD,VD+VDD}, {(VF)minsfxX,VX}, {(VF)minsfxQ,VQ}, {(VF)minsfxS,VSB}}},

/* 83 <: */ {
 {{(VF)leBB,VB+VIP}, {(VF)leBI,VB+VIPOKA}, {(VF)leBD,VB+VIPOKA},
  {(VF)leIB,VB+VIPOKW}, {(VF)leII,VB}, {(VF)leID,VB},
  {(VF)leDB,VB+VIPOKW}, {(VF)leDI,VB}, {(VF)leDD,VB}, 
  {(VF)leDD,VB+VDD+VIP}, {(VF)leXX,VB+VXCF}, {(VF)leQQ,VB+VQQ}, {(VF)leSS,VB}},
 {{(VF)leinsB,VB}},
 {{(VF)lepfxB,VB}},
 {{(VF)lesfxB,VB}} },

/* 84 >. */ {
 {{(VF)orBB,VB+VIP}, {(VF)maxBI,VI+VIPOKW}, {(VF)maxBD,VD+VIPOKW},
  {(VF)maxIB,VI+VIPOKA}, {(VF)maxII,VI+VIP}, {(VF)maxID,VD+VIPID},
  {(VF)maxDB,VD+VIPOKA}, {(VF)maxDI,VD+VIPDI}, {(VF)maxDD,VD+VIP}, 
  {(VF)maxDD,VD+VDD+VIP}, {(VF)maxXX,VX+VXX}, {(VF)maxQQ,VQ+VQQ}, {(VF)maxSS,VSB}},
 {{(VF)orinsB,VB}, {(VF)maxinsI,VI}, {(VF)maxinsD,VD}, {(VF)maxinsD,VD+VDD}, {(VF)maxinsX,VX}, {0,0},        {(VF)maxinsS,VSB}},
 {{(VF)orpfxB,VB}, {(VF)maxpfxI,VI}, {(VF)maxpfxD,VD}, {(VF)maxpfxD,VD+VDD}, {(VF)maxpfxX,VX}, {(VF)maxpfxQ,VQ}, {(VF)maxpfxS,VSB}},
 {{(VF)orsfxB,VB}, {(VF)maxsfxI,VI}, {(VF)maxsfxD,VD}, {(VF)maxsfxD,VD+VDD}, {(VF)maxsfxX,VX}, {(VF)maxsfxQ,VQ}, {(VF)maxsfxS,VSB}}},

/* 85 >: */ {
 {{(VF)geBB,VB+VIP}, {(VF)geBI,VB+VIPOKA}, {(VF)geBD,VB+VIPOKA},
  {(VF)geIB,VB+VIPOKW}, {(VF)geII,VB}, {(VF)geID,VB},
  {(VF)geDB,VB+VIPOKW}, {(VF)geDI,VB}, {(VF)geDD,VB}, 
  {(VF)geDD,VB+VDD+VIP}, {(VF)geXX,VB+VXFC}, {(VF)geQQ,VB+VQQ}, {(VF)geSS,VB}},
 {{(VF)geinsB,VB}},
 {{(VF)gepfxB,VB}},
 {{(VF)gesfxB,VB}} },

/* 88 +. */ {
 {{(VF)orBB,VB+VIP     }, {(VF)gcdII,VI+VII}, {(VF)gcdDD,VD+VDD+VIP},
  {(VF)gcdII,VI+VII}, {(VF)gcdII,VI    }, {(VF)gcdDD,VD+VDD+VIP},
  {(VF)gcdDD,VD+VDD+VIP}, {(VF)gcdDD,VD+VDD+VIP}, {(VF)gcdDD,VD+VIP+VCANHALT}, 
  {(VF)gcdZZ,VZ+VZZ}, {(VF)gcdXX,VX+VXX}, {(VF)gcdQQ,VQ+VQQ}, {0,0}},
 {{(VF)orinsB,VB}},
 {{(VF)orpfxB,VB}},
 {{(VF)orsfxB,VB}} },

/* 89 +: */ {
 {{(VF)norBB,VB+VIP    }, {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP},
  {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP},
  {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP}, 
  {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP}, {(VF)norBB,VB+VBB+VIP}, {0,0}},
 {{(VF)norinsB,VB}},
 {{(VF)norpfxB,VB}},
 {{(VF)norsfxB,VB}} },

/* 8a *. */ {
 {{(VF)andBB,VB+VIP    }, {(VF)lcmII,VI+VII}, {(VF)lcmDD,VD+VDD+VIP},
  {(VF)lcmII,VI+VII}, {(VF)lcmII,VI    }, {(VF)lcmDD,VD+VDD+VIP},
  {(VF)lcmDD,VD+VDD+VIP}, {(VF)lcmDD,VD+VDD+VIP}, {(VF)lcmDD,VD+VIP+VCANHALT}, 
  {(VF)lcmZZ,VZ+VZZ}, {(VF)lcmXX,VX+VXX}, {(VF)lcmQQ,VQ+VQQ}, {0,0}},
 {{(VF)andinsB,VB}},
 {{(VF)andpfxB,VB}},
 {{(VF)andsfxB,VB}} },

/* 8b *: */ {
 {{(VF)nandBB,VB+VIP},     {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP},
  {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP},
  {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP}, 
  {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP}, {(VF)nandBB,VB+VBB+VIP}, {0,0}},
 {{(VF)nandinsB,VB}},
 {{(VF)nandpfxB,VB}},
 {{(VF)nandsfxB,VB}} },

/* 95 ~: */ {
 {{(VF)neBB,VB+VIP}, {(VF)neBI,VB+VIPOKA}, {(VF)neBD,VB+VIPOKA},
  {(VF)neIB,VB+VIPOKW}, {(VF)neII,VB}, {(VF)neID,VB},
  {(VF)neDB,VB+VIPOKW}, {(VF)neDI,VB}, {(VF)neDD,VB}, 
  {(VF)neZZ,VB+VZZ}, {(VF)neXX,VB+VXEQ}, {(VF)neQQ,VB+VQQ}, {0,0}},
 {{(VF)neinsB,VB}},
 {{(VF)nepfxB,VB}},
 {{(VF)nesfxB,VB}} },

/* d1 o. */ {
 {{(VF)cirDD,VD+VDD}, {(VF)cirDD,VD+VDD}, {(VF)cirBD,VD},
  {(VF)cirDD,VD+VDD}, {(VF)cirDD,VD+VDD}, {(VF)cirID,VD},
  {(VF)cirDD,VD+VDD}, {(VF)cirDD,VD+VDD}, {(VF)cirDD,VD}, 
  {(VF)cirZZ,VZ+VZZ+VRD}, {(VF)cirDD,VD+VDD}, {(VF)cirDD,VD+VDD}, {0,0}},
 {{0,0}},
 {{0,0}},
 {{0,0}} }
};


static UC vaptr[256]={
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* 0 */
// C0  C1          ^D                    TAB LF          CR         
   1,  2,  3,  4,  5,  6,  7,  8,    9, 10, 11, 12, 13, 14, 15, 16, /* 1 */
// <-----------------------bitwise functions -------------------->
   0, 17,  0,  0,  0, 18,  0,  0,    0,  0, 19, 20,  0, 21,  0,  0, /* 2 */
//     !   "   #   $   %   &   '     (   )   *   +   ,   -   .   /  
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0, 22, 23, 24,  0, /* 3 */
// NOUN    HOOK FK ADVF                      :   ;   <   =   >   ?  
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* 4 */
// @                                                                
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0, 25,  0, /* 5 */
//                                               [   \   ]   ^   _  
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* 6 */
// `                                                                
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0, 26,  0,  0,  0, /* 7 */
//                                               {   |   }   ~      
   0,  0, 27, 28, 29, 30,  0,  0,   31, 32, 33, 34,  0,  0,  0,  0, /* 8 */
// =.  =:  <.  <:  >.  >:  _.        +.  +:  *.  *:  -.  -:  %.  %: 
   0,  0,  0,  0,  0, 35,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* 9 */
// ^.  ^:  $.  $:  ~.  ~:  |.  |:    ..  .:  :.  ::  ,.  ,:  ;.  ;: 
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* a */
// #.  #:  !.  !:  /.  /:  \.  \:    [.  [:  ].  ]:  {.  {:  }.  }: 
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* b */
// ".  ":  `.  `:  @.  @:  &.  &:    ?.  ?:  a.  A.  a:  b.  c.     
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* c */
// C.  d.  D.  D:  e.  E.  f.  F:    H.  i.  i:  I.  I:  j.  L.  L: 
   0,  0,  0, 36,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* d */
// m.  M.  n.  o.  p.  p:  Q:  q:    r.  s:  S:  t.  t:  T.  u.  u: 
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* e */
// v.  x.  x:  y.                    0:  }ip }cs {:: {:: }:: &.: p..
   0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0, /* f */
// ,ip                                                           FF  
  };   /* index in va[] for each ID */
/*   1   2   3   4   5   6   7     8   9   a   b   c   d   e   f   */

// return atomic2 ID for the verb w.  If w is b., look for its u operand and return the appropriate 
C jtvaid(J jt,A w){A x;C c;I k;V*v;
 v=FAV(w); c=v->id;
 if(c==CBDOT){x=v->fgh[0]; if(INT&AT(x)&&!AR(x)&&(k=*AV(x),16<=k&&k<=31))c=(C)k;}
 R vaptr[(UC)c]?c:0;
}

// prepare a primitive atomic verb for lookups using var()
// if the verb is atomic, we fill in the h field with a pointer to the va row for the verb
void va2primsetup(A w){
 UC xlatedid = vaptr[(UC)FAV(w)->id];  // see which line it is
 FAV(w)->localuse=(xlatedid?&va[xlatedid]:0);  // point to the line, or 0 if invalid
 if(xlatedid)FAV(w)->flag |= VISATOMIC2;  // indicate that localuse contains AV pointer
}

A jtcvz(J jt,I cv,A w){I t;
 t=AT(w);
 if(cv&VRD&&!(t&FL) )R pcvt(FL,w);
 if(cv&VRI&&!(t&INT))R icvt(w);
 R w;
}    /* convert result */

// Routine to lookup function/flags
// ptr is the type of lookup (insert/prefix/suffix) to generate the function for
// In the function, id is the pseudochar for the function to look up
//  t is the argument type
//  action routine is stored in *ado, flags in *cv.  If no action routine, *ado=0
#define VAF(fname,ptr,fp,fm,ft)   \
 void fname(J jt,C id,I t,VF*ado,I*cv){VA2*p;  \
  if(jt->jerr>=EWOV){                          \
   jt->jerr=0;                                 \
   *ado=(id==CPLUS?fp:id==CMINUS?fm:ft);       \
   *cv=VD;                                     \
  }else if(t&=NUMERIC+SBT){                     \
   p=(va+vaptr[(UC)id])->ptr+(t<=FL?(t>>INTX):t<=RAT?(3+(t>>XNUMX)):6);  \
   *ado=p->fgh[0]; *cv=p->cv;                       \
  }else *ado=0;                                \
 }
// Routine to lookup function/flags
// ptr is the type of lookup (insert/prefix/suffix) to generate the function for
// In the function, id is the pseudochar for the function to look up
//  t is the argument type
//  action routine is stored in *ado, flags in *cv.  If no action routine, *ado=0
#define VA2F(fname,ptr,fp,fm,ft)   \
 static VA2 fname##EWOV[] = { {ft,VD} , {fp,VD}, {fm,VD}, {0,0} }; \
 VA2 jt##fname(J jt,A self,I t){  \
  if(!(jt->jerr>=EWOV)){                          \
   if((t&=(NUMERIC+SBT)&(~SPARSE))&&FAV(self)->flag&VISATOMIC2){  /* numeric input, verb with dataline */        \
    R ((VA*)(FAV(self)->localuse))->ptr[t<=FL?(t>>INTX):t<=RAT?(3+(t>>XNUMX)):6];  \
   }else R fname##EWOV[3];                                \
  }else{ \
   jt->jerr=0;                                 \
   R fname##EWOV[2*(FAV(self)->id==CMINUS) + (FAV(self)->id==CPLUS)]; \
  }  \
 }

// Lookup the action routine & flags for insert/prefix/suffix
// first name is name of the generated function
// second name (e. g. pins) is the name of the structure element containing the action-routine pointers
// other names are the names of overflow action routines for plus,minus,times respectively

VA2F(vains,pins, plusinsO,minusinsO,tymesinsO)
VA2F(vapfx,ppfx, pluspfxO,minuspfxO,tymespfxO)
VA2F(vasfx,psfx, plussfxO,minussfxO,tymessfxO)

#define VARCASE(e,c) (70*(e)+(c))

// Table converting operand types to slot numbers in a va[] entry
// Employed when one arg is known to be CMPX/XNUM/RAT.  Indexed by
// bitmask of RAT,CMPX,FL.  Entry 9=CMPX; otherwise entry 8=some FL, otherwise
// entry 10 for XNUM, 11 for some RAT 
static UC xnumpri[] = {10 ,8 ,9 ,9 ,11 ,8 ,9 ,9};

extern A jtva2recur(J jt, AD * RESTRICT a, AD * RESTRICT w, AD * RESTRICT self);

// All dyadic arithmetic verbs f enter here, and also f"n.  a and w are the arguments, id
// is the pseudocharacter indicating what operation is to be performed
A jtva2(J jt,AD * RESTRICT a,AD * RESTRICT w,AD * RESTRICT self){A z;I acn,wcn,bcip;I ak,f,m,
     mf,n,nf,r,* RESTRICT s,*sf,wk,zk,zn,zt;VA2 adocv;
 RZ(a&&w);F2PREFIP;
#define an AN(a)
#define wn AN(w)
 {I at=AT(a);
  I wt=AT(w);
  // If an operand is empty, turn it to Boolean, and if the OTHER operand is non-numeric, turn that to Boolean too (leaving
  //  rank and shape untouched).  This change to the other operand is notional only - we won't actually convert
  // when there is an empty - but it guarantees that execution on n empty never fails.
  // If we switch a sparse nonnumeric matrix to boolean, that may be a space problem; but we don't
  // support nonnumeric sparse now
  if(((-((at|wt)&SPARSE))|(an-1)|(wn-1))<0) { // test for all unusual cases: sparse or empty arg.  This sets FLAGSPARSE if called for & clears other flags
   // if an operand is sparse, replace its type with the corresponding non-sparse type, for purposes of testing operand precisions
   if((at|wt)&SPARSE){
    at=(SPARSE&at)?DTYPE(at):at;
    wt=(SPARSE&wt)?DTYPE(wt):wt;
    jtinplace=0;  // We use jtinplace==0 as a flag meaning 'sparse'
   }
   if(an==0){at=B01;if(!(wt&NUMERIC))wt=B01;}  // switch empty arg to Boolean & ensure compatibility with other arg
   if(wn==0){wt=B01;if(!(at&NUMERIC))at=B01;}
  }

  // Figure out the result type.  Don't signal the error from it yet, because domain has lower priority than agreement
  // Extract zt, the type of the result, and cv, the flags indicating the types selected for the arguments and the result
  adocv=var(self,at,wt); zt=rtype(adocv.cv);
 }

 I ar = AR(a);
 I wr = AR(w);

 // Analyze the rank and calculate cell shapes and counts.  Not byte sizes yet, since there may be conversions
 // We detect agreement error before domain error
 RANK2T savedranks=jt->ranks;   // save original rank before we change it, in case we have to restart the operation
 {I *as = AS(a); I *ws = AS(w);
  if(savedranks==(RANK2T)~0){I b;I ipa;
   // No rank specified.  Since all these verbs have rank 0, that simplifies quite a bit
   r=an; m=wn; b=wr>=ar; zn=b?m:r; m=b?r:m; r=b?wr:ar; I shortr=b?ar:wr; s=b?ws:as; PROD(n,r-shortr,s+shortr);   // treat the entire operands as one big cell; get the rest of the values needed
   ipa=(adocv.cv>>VIPOKWX) & ((a==w)-1) & ((ar==r)*2 + (wr==r));  // inplaceability (see below)
   bcip=ipa+8*b;  // save the combined bcip for loop control
   DO(shortr, ASSERT(as[i]==ws[i],EVLENGTH);)  // agreement error if not prefix match
   f = 0;  // no frame since we didn't have rank
   if(jtinplace){
    // Non-sparse setup for copy loop, no rank
    mf=nf=1;  // suppress the outer loop, leaving only the loop over m and n
   }else{
    // Sparse setup: move the block-local variables to longer-lived ones.  We are trying to reduce register pressure
    // repurpose ak/wk/mf/nf to hold acr/wcr/af/wf, which we will pass into vasp.  This allows acr/wcr/af/wf to be block-local
    ak=ar; wk=wr; mf=0; nf=0;
   }
  }else{I af,wf,acr,wcr,q,b,c,zcn;I ipa;
   // Here, a rank was specified.  That means there must be a frame, according the to IRS rules
   acr=savedranks>>RANKTX; acr=ar<acr?ar:acr; af=ar-acr; PROD(acn,acr,as+af);  // r=left rank of verb, acr=effective rank, af=left frame, acn=left #atoms/cell
   wcr=(RANKT)savedranks; wcr=wr<wcr?wr:wcr; wf=wr-wcr; PROD(wcn,wcr,ws+wf); // r=right rank of verb, wcr=effective rank, wf=right frame, wcn=left #atoms/cell
       // note: the prod above can never fail, because it gives the actual # cells of an existing noun
   // Now that we have used the rank info, clear jt->rank.  All verbs start with jt->rank=0 unless they have "n applied
   // we do this before we generate failures
   RESETRANK;  // This is required for xnum/rat/sparse
   // if the frames don't agree, that's always an agreement error
   c=af<=wf; f=c?wf:af; q=c?af:wf; sf=c?ws:as;   // c='right frame is longer'; f=#longer frame; q=#shorter frame; sf->shape of arg with longer frame
   b=acr<=wcr; zcn=b?wcn:acn; m=b?acn:wcn; r=b?wcr:acr; I shortr=b?acr:wcr; I longf=b?wf:af; s=b?ws:as; s+=longf; PROD(n,r-shortr,s+shortr);   // b='right cell has larger rank'; zcn=#atoms in cell with larger rank;
     // m=#atoms in cell with shorter rank; n=#times shorter-rank cells must be repeated; r=larger of cell-ranks; s->shape of larger-rank cell
   ipa=(adocv.cv>>VIPOKWX) & (((a==w)|(zt&B01))-1) & ((ar==(f+r))*2 + (wr==(f+r)));  //  calc inplaceability - see below
   bcip=ipa+c*4+8*b;  // save combined loop control
   DO(q, ASSERT(as[i]==ws[i],EVLENGTH);)  // frames must match to the shorter length; agreement error if not
   if(jtinplace){  // If not sparse... This block isn't needed for sparse arguments, and may fail on them.  We move it here to reduce register pressure
    PROD(mf,q,sf); PROD(nf,f-q,q+sf);    // mf=#cells in common frame, nf=#times shorter-frame cell must be repeated.  Not needed if no cells
#ifdef DPMULD
    { DPMULDDECLS DPMULD(nf,zcn,zn,{jsignal(EVLIMIT);R 0;}) DPMULD(zn,mf,zn,{jsignal(EVLIMIT);R 0;}) }
#else
    RE(zn=mult(mf,mult(nf,zcn)));  // zn=total # result atoms  (only if non-sparse)
#endif
    // if the cell-shapes don't match, that's an agreement error UNLESS the frame contains 0; in that case it counts as
    // 'error executing on the cell of fills' and produces a scalar 0 as the result for that cell, which we handle by changing the result-cell rank to 0
    // Nonce: continue giving the error even when frame contains 0 - remove 1|| in the next line to conform to fill-cell rules
    DO(MIN(acr,wcr), ASSERT(as[af+i]==ws[wf+i],EVLENGTH);)
// this shows the fix   if(ICMP(as+af,ws+wf,MIN(acr,wcr))){if(1||zn)ASSERT(0,EVLENGTH)else r = 0;}
    // if looping required, calculate the strides for input & output.  Needed only if mf or nf>1, but not worth testing, since presumably one will, else why use rank?
    {zk=zcn*bp(zt); ak=acn*bp(AT(a)); wk=wcn*bp(AT(w));}//else {ak=wk=zk=0;}
    // zk=result-cell size in bytes; ak,wk=left,right arg-cell size in bytes.  Not needed if not looping
   }else{ak=acr; wk=wcr; mf=af; nf=wf;}  // For sparse, repurpose ak/wk/mf/nf to hold acr/wcr/af/wf, which we will pass into vasp.  This allows acr/wcr/af/wf to be block-local
  }
 }

 // Signal domain error if appropriate. Must do this after agreement tests
 ASSERT(adocv.f,EVDOMAIN);


 // finish up the computation of sizes.  We have to defer this till after var() because
 // if we are retrying the operation, we may be in error state until var clears it; and prod and mult can fail,
 // so we have to RE when we call them
 if(jtinplace){   // if not sparse...
  // Not sparse.

   // If op specifies forced input conversion AND if both arguments are non-sparse: convert them to the selected type.
   // Incompatible arguments were detected in var().  If there is an empty operand, skip conversions which
   // might fail because the type in t is incompatible with the actual type in a.  t is rare.
   //
   // Because of the priority of errors we mustn't check the type until we have verified agreement above
   if(adocv.cv&VARGMSK&&zn>0){I t=atype(adocv.cv);  // t not 0, and the result is not empty
    // Conversions to XNUM use a routine that pushes/sets/pops jt->mode, which controls the
    // type of conversion to XNUM in use.  Any result of the conversion is automatically inplaceable.  If type changes, change the cell-size too
    if(TYPESNE(t,AT(a))){RZ(a=!(t&XNUM)?cvt(t,a):xcvt((adocv.cv&VXCVTYPEMSK)>>VXCVTYPEX,a));jtinplace = (J)(intptr_t)((I)jtinplace | JTINPLACEA); ak=acn*bp(AT(a));}
    if(TYPESNE(t,AT(w))){RZ(w=!(t&XNUM)?cvt(t,w):xcvt((adocv.cv&VXCVTYPEMSK)>>VXCVTYPEX,w));jtinplace = (J)(intptr_t)((I)jtinplace | JTINPLACEW); wk=wcn*bp(AT(w));}
   }  // the function call here inhibits register assignment to temporaries.  It might be better to do the conversion earlier, and defer the error
      // until here.  We will have to look at the generated code when we can use all the registers
   // acn, wcn are not used beyond this point
   // From here on we have possibly changed the address of a and w, but we are still using shape pointers,
   // rank, type, etc. using the original input block.  That's OK.

// less regs, more comp    inplaceallow &= (I)jtinplace;  // allow only blocks originally marked inplaceable
  
   // Allocate a result area of the right type, and copy in its cell-shape after the frame
  // If an argument can be overwritten, use it rather than allocating a new one
  // Argument can be overwritten if: action routine allows it; flagged in jtinplace; usecount 1 or zombie; rank equals (length of longer frame)+(length of longer cell)
  // If the argument has rank that large, and the arguments agree, the argument MUST have the same number of atoms as the result, because all shape is accounted for.
  // rank = rank of result (the rank of the result is the sum of (the longer frame-length) plus (the larger cell-rank))
  // Also, if the operation is one that may abort, we suppress inplacing it if the user can't handle early assignment.
  // Finally, if a==w suppress inplacing, in case the operation must be retried (we could check which ones but they are
  // just not likely to be used reflexively)
  // 
  // If the # atoms is same as result, each atom must be read only once & can be written after it has been read
  // This holds true even if the atoms have different types, as long as they have the same size (this is encoded in the IPOK bits of cv)
  // use the inplacing from the action routine to qualify the caller's inplacing, EXCEPT
  // for Boolean inputs: since we do the operations a word at a time, they may overrun the output area and
  // are thus not inplaceable.  The exception is if there is only one loop through the inputs; that's always inplaceable
// less regs, more comp   {I inplaceallow = (adocv.cv>>VIPOKWX) & (((a==w)|((f!=0)&zt))-1) & ((((zn^an)|(ar^(f+r)))==0)*2 + (((zn^wn)|(wr^(f+r)))==0));
   
      // also turn off inplacing if a==w  or if Boolean with repeated cells   uses B01==1
      // (mf|nf)>1 is a better test than f!=0, because it handles frame of all 1s, but it's slower in the normal case
      // only the bottom 2 bits of inplaceallow are valid

   // Establish the result area z; if we're reusing an argument, make sure the type is updated to the result type
   // If the operation is one that can fail partway through, don't allow it to overwrite a zombie input unless so enabled by the user
// more regs, less comp    if((inplaceallow&(((zn^wn)|(wr^(f+r)))==0)) && (AC(w)<ACUC1 || AC(w)==ACUC1&&jt->assignsym&&jt->assignsym->val==w&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=w; AT(z)=zt;  //  Uses JTINPLACEW==1
// more regs, less comp    }else if(((inplaceallow>>=1)&(((zn^an)|(ar^(f+r)))==0)) && (AC(a)<ACUC1 || AC(a)==ACUC1&&jt->assignsym&&jt->assignsym->val==a&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=a; AT(z)=zt;  //  Uses JTINPLACEA==2
  if((bcip&(I)jtinplace&1) && (AC(w)<ACUC1 || AC(w)==ACUC1&&jt->assignsym&&jt->assignsym->val==w&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=w; if(TYPESNE(AT(w),zt))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEW==1
  }else if((bcip&(I)jtinplace&2) && (AC(a)<ACUC1 || AC(a)==ACUC1&&jt->assignsym&&jt->assignsym->val==a&&!(adocv.cv&VCANHALT && jt->asgzomblevel<2))){z=a; if(TYPESNE(AT(a),zt))MODBLOCKTYPE(z,zt)  //  Uses JTINPLACEA==2
  }else{GA(z,zt,zn,f+r,0); I *zs=AS(z); MCISds(zs,sf,f);    MCISds(zs,s,r);} 
  // s, r, f, and sf ARE NOT USED FROM HERE ON in this branch to reduce register pressure.  They have been destroyed in the loops above
  if(!zn)R z;  // If the result is empty, the allocated area says it all
  // zn  NOT USED FROM HERE ON

  // End of setup phase.  The execution phase:

  {C *av=CAV(a); C *wv=CAV(w); C *zv=CAV(z);   // point to the data
   // Call the action routines: 
#if 1  // register spills make this code faster.
   // note: the compiler unrolls these call loops.  Would be nice to suppress that
   if(1==nf){I i=mf; while(1){adocv.f(jt,bcip>>3,m,n,zv,av,wv); if(!--i)break; zv+=zk; av+=ak; wv+=wk;}}  // if the short cell is not repeated, loop over the frame
   else if(bcip&4){I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zv,av,wv); zv+=zk;         wv+=wk;}while(--in); av+=ak;}while(--im);} // if right frame is longer, repeat cells of a
   else         {I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zv,av,wv); zv+=zk;         av+=ak;}while(--in); wv+=wk;}while(--im);}  // if left frame is longer, repeat cells of w
#else  // would be faster if all loop variables moved to registers
   I wkm,wkn,akm,akn;
// compbug   if(f) {  // increments are immaterial if there is no looping, and the increments are on the stack and therefore slow to calculate... and the compiler inits the variables anyway in VS13
    wkm=wk, akn=ak; wkn=(bcip<<(BW-1-2))>>(BW-1);   // wkn=111.111 iff wk increments with n (and therefore ak with m)
    akm=akn&wkn; wkn&=wkm; wkm^=wkn; akn^=akm;  // if c, akm=ak/wkn=wk; else akn=ak/wkm=wk.  The other incr is 0
// compbug    }
   I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zv,av,wv); zv+=zk; av+=akn; wv +=wkn;}while(--in); if(!--im)break; av+=akm; wv +=wkm;}while(1);
#endif
  }
  // The work has been done.  If there was no error, check for optional conversion-if-possible or -if-necessary
  if(!jt->jerr){RETF(!(adocv.cv&VRI+VRD)?z:cvz(adocv.cv,z));  // normal return is here.  The rest is error recovery
  }else if(jt->jerr-EWOVIP>=0){A zz;C *zzv;I zzk;
   // Here for overflow that can be corrected in place
   // If the original result block cannot hold the D result, allocate a separate result area
   if(sizeof(D)==sizeof(I)){zz=z; AT(zz)=FL; zzk=zk;   // shape etc are already OK
   }else{GATV(zz,FL,AN(z),AR(z),AS(z)); zzk=zk*(sizeof(D)/sizeof(I));}
   // restore pointers to beginning of arguments
   zzv=CAV(zz);  // point to new-result data
   // Set up pointers etc for the overflow handling.  Set b=1 if w is taken for the x argument to repair
   if(jt->jerr==EWOVIP+EWOVIPMULII){D *zzvd=(D*)zzv; I *zvi=IAV(z);
    // Multiply repair.  We have to convert all the pre-overflow results to float, and then finish the multiplies
    DO(jt->mulofloloc, *zzvd++=(D)*zvi++;);  // convert the multiply results to float
    // Now repeat the processing.  Unlike with add/subtract overflow, we have to match up all the argument atoms
    adocv.f=(VF)tymesIIO;  // multiply-repair routine
    {C *av=CAV(a); C *wv=CAV(w);
#if 0  // for this rare case, use the shorter version
     if(1==nf){I i=mf; while(1){adocv.f(jt,bcip>>3,m,n,zzv,av,wv); if(--i==0)break; zzv+=zzk; av+=ak; wv+=wk;}}  // if the short cell is not repeated, loop over the frame
     else if(bcip&4){I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zzv,av,wv); zzv+=zzk;         wv+=wk;}while(--in); av+=ak;}while(--im);} // if right frame is longer, repeat cells of a
     else         {I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zzv,av,wv); zzv+=zzk;         av+=ak;}while(--in); wv+=wk;}while(--im);}  // if left frame is longer, repeat cells of w
#else
     I wkm,wkn,akm,akn;
// compbug   if(f) {  // increments are immaterial if there is no looping, and the increments are on the stack and therefore slow to calculate... and the compiler inits the variables anyway in VS13
      wkm=wk, akn=ak; wkn=(bcip<<(BW-1-2))>>(BW-1);   // wkn=111.111 iff wk increments with n (and therefore ak with m)
      akm=akn&wkn; wkn&=wkm; wkm^=wkn; akn^=akm;  // if c, akm=ak/wkn=wk; else akn=ak/wkm=wk.  The other incr is 0
// compbug    }
     I im=mf; do{I in=nf; do{adocv.f(jt,bcip>>3,m,n,zzv,av,wv); zzv+=zzk; av+=akn; wv +=wkn;}while(--in); if(!--im)break; av+=akm; wv +=wkm;}while(1);
#endif
    }
   } else {I nipw;   // not multiply repair
    switch(jt->jerr-EWOVIP){
    case EWOVIPPLUSII:
     // choose the non-in-place argument
     adocv.f=(VF)plusIIO; nipw = z!=w; break; // if w not repeated, select it for not-in-place
    case EWOVIPPLUSBI:
     adocv.f=(VF)plusBIO; nipw = 0; break;   // Leave the Boolean argument as a
    case EWOVIPPLUSIB:
     adocv.f=(VF)plusBIO; nipw = 1; break;  // Use w as not-in-place
    case EWOVIPMINUSII:
     adocv.f=(VF)minusIIO; nipw = z!=w; break; // if w not repeated, select it for not-in-place
    case EWOVIPMINUSBI:
     adocv.f=(VF)minusBIO; nipw = 0; break;   // Leave the Boolean argument as a
    case EWOVIPMINUSIB:
     adocv.f=(VF)minusBIO; nipw = 1; break;  // Use w as not-in-place
    }
    // nipw means 'use w as not-in-place'; c means 'repeat cells of a'; so if nipw!=c we repeat cells of not-in-place, if nipw==c we set nf to 1
    // if we are repeating cells of the not-in-place, we leave the repetition count in nf, otherwise subsume it in mf
    // b means 'repeat atoms inside a'; so if nipw!=b we repeat atoms of not-in-place, if nipw==b we set n to 1
    {C *av, *zv=CAV(z);
     if(nipw){av=CAV(w), ak=wk;}else{av=CAV(a);}  if(nipw==((bcip>>2)&1)){mf *= nf; nf = 1;} if(nipw==((bcip>>3)&1)){m *= n; n = 1;}
     // We have set up ado,nf,mf,nipw,m,n for the conversion.  Now call the repair routine.  n is # times to repeat a for each z, m*n is # atoms of z/zz
     DO(mf, DO(nf, adocv.f(jt,nipw,m,n,zzv,av,zv); zzv+=zzk; zv+=zk;); av+=ak;)  // use each cell of a (nf) times
    }
   }
   RESETERR
   R zz;  // Return the result after overflow has been corrected
  }
 }else{z=vasp(a,w,FAV(self)->id,adocv.f,adocv.cv,atype(adocv.cv),zt,mf,ak,nf,wk,f,r); if(!jt->jerr)R z;}  // handle sparse arrays separately.  at this point ak/wk/mf/nf hold acr/wcr/af/wf
 // If we got an internal-only error during execution of the verb, restart to see if it's
 // a recoverable error such an overflow during integer addition.  We have to restore
 // jt->rank, which might have been modified.  All sparse errors come through here, so they can't
 // do overflow recovery in-place
 R NEVM<jt->jerr?irs2(a,w,self,savedranks>>RANKTX,(RANKT)savedranks,jtva2):0;
#undef an
#undef wn
}    /* scalar fn primitive and f"r main control */

/*
 acn wcn zcn  # atoms in a cell
 acr wcr r    cell rank
 af  wf  f    frame
 ak  wk  zk   # bytes in a cell
 an  wn  zn   overall # atoms
 ar  wr  f+r  overall rank

 b    1 iff cell rank of a <= cell rank of w
 c    1 iff frame     of a <= frame of w
 m    # of atoms in the cell with the smaller rank
 mf   agreed # of frames
 n    excess # of cell atoms
 nf   excess # of frames
 f    max of frame of a, frame of w
 q    min of frame of a, frame of w
 r    max of cell rank  of a, cell rank  of w
 s    max of cell shape of a, cell shape of w
 sf   max of frame shape of a, frame shape of w
*/


// 4-nested loop for dot-products.  Handles repeats for inner and outer frame.  oneprod is the code for calculating a single vertor inner product
#define SUMATLOOP(ti,to,oneprod) \
  {ti *av=ti##AV(a),*wv=ti##AV(w); to *zv=to##AV(z); DQ(nfro, I jj=nfri; ti *ov0=repeata?av:wv; while(1){DQ(ndpo, I j=ndpi; ti *av0=av; while(1){oneprod if(!--j)break; av=av0;}) if(!--jj)break; if(repeata)av=ov0;else wv=ov0; })}

// routine to do the dot-product calculations.  Brought out to help the compiler allocate registers
static A jtsumattymesprods(J jt,I it,A a, A w,I dplen,I nfro,I nfri,I ndpo,I ndpi,I repeata,A z){
 switch(it){
 case B01:
  SUMATLOOP(B,I,
   I total=0; I k=dplen; I *avi=(I*)av; I *wvi=(I*)wv;
    while(k>>LGSZI){I kn=MIN(255,k>>LGSZI); k-=kn<<LGSZI; I total2=0; DQ(kn, total2+=*avi++&*wvi++;) ADDBYTESINI(total2); total+=total2;} av=(B*)avi; wv=(B*)wvi; DQ(k, total+=*av++&*wv++;)
   *zv++=total;
  )
  break;
 case INT:
  SUMATLOOP(I,D,D total=0.0; DQ(dplen, total+=(D)*av++*(D)*wv++;); *zv++=total;)
  break;
 case FL:
  NAN0;
  SUMATLOOP(D,D,D total=0.0; DQ(dplen, total+=(D)*av++*(D)*wv++;); *zv++=total;)
  if(NANTEST){  // if there was an error, it might be 0 * _ which we will turn to 0.  So rerun, checking for that.
   NAN0;
   SUMATLOOP(D,D,D total=0.0; DQ(dplen, D u=*av++; D v=*wv++; if(u&&v)total+=dmul2(u,v);); *zv++=total;)
   NAN1;
  }
  break;
 }
 RETF(z);
}



// +/@:*"1 with IRS
DF2(jtsumattymes1){
 RZ(a&&w);
 // Order so the rank of a is <= rank of w.  Since we do not handle outer frames here, this will guarantee that only the
 // a argument below can have repeated cells
 I ar=AR(a); I wr=AR(w); I acr=jt->ranks>>RANKTX; I wcr=jt->ranks&RMAX;
 // get the cell-ranks to use 
 acr=ar<acr?ar:acr;   // r=left rank of verb, acr=effective rank
 wcr=wr<wcr?wr:wcr;  // r=right rank of verb, wcr=effective rank
     // note: the prod above can never fail, because it gives the actual # cells of an existing noun
   // Now that we have used the rank info, clear jt->ranks.  All verbs start with jt->ranks=RMAXX unless they have "n applied
   // we do this before we generate failures
 RESETRANK;  // This is required if we go to slower code
 I it=MAX(AT(a),AT(w));  // if input types are dissimilar, convert to the larger
 // if an argument is empty, sparse, has cell-rank 0, or not a fast arithmetic type, revert to the code for f/@:g atomic
 if(((-((AT(a)|AT(w))&(NOUN&~(B01|INT|FL))))|(AN(a)-1)|(AN(w)-1)|(acr-1)|(wcr-1))<0) { // test for all unusual cases
  R rank2ex(a,w,FAV(self)->fgh[0],MIN(acr,1),MIN(wcr,1),acr,wcr,jtfslashatg);
 }
 // We can handle it here, and both ranks are at least 1.

 // If there is no additional rank (i. e. highest cell-rank is 1), ignore the given rank (which must be 1 1) and use the argument rank
 // This promotes the outer loops to inner loops
 {I rankgiven = (acr|wcr)-1; acr=rankgiven?acr:ar; wcr=rankgiven?wcr:wr;}

 // Exchange if needed to make the cell-rank of a no greater than that of w.  That way we know that w will never repeat in the inner loop
 if(acr>wcr){A t=w; I tr=wr; I tcr=wcr; w=a; wr=ar; wcr=acr; a=t; ar=tr; acr=tcr;}

 // Verify inner frames match
 DO(acr-1, ASSERT(AS(a)[ar-acr+i]==AS(w)[wr-wcr+i],EVLENGTH);); ASSERT(AS(a)[ar-1]==AS(w)[wr-1],EVLENGTH);  // agreement error if not prefix match

 // calculate inner repeat amounts and result shape
 I dplen = AS(a)[ar-1];  // number of atoms in 1 dot-product
 I ndpo; PROD(ndpo,acr-1,AS(w)+wr-wcr);  // number of cells of a = # outer loops
 I ndpi; PROD(ndpi,wcr-acr,AS(w)+wr-wcr+acr-1);  // number of times each cell of a must be repeated (= excess frame of w)
 I zn=ndpo*ndpi;  // number of results from 1 inner cell.  This can't overflow since frames agree and operands are not empty

 A z; 
 // if there is frame, create the outer loop values
 I nfro,nfri,repeata;  // outer loop counts, and which arg is repeated
 if(((ar-acr)|(wr-wcr))==0){  // normal case
  nfro=nfri=1;  // no outer loops, repeata immaterial
  GA(z,FL>>(it&B01),ndpo*ndpi,wcr-1,AS(w));  // type is INT if inputs booleans, otherwise FL
 }else{
  // There is frame, analyze and check it
  I af=ar-acr; I wf=wr-wcr; I commonf=wf; I *as=AS(a), *ws=AS(w); I *longs=as;
  repeata=wf>=af; commonf=wf>=af?af:commonf; longs=wf>=af?ws:longs;  // repeat flag, length of common frame, pointer to long shape
  af+=wf; af-=2*commonf;  // repurpose af to be length of surplus frame
  DO(commonf, ASSERT(as[i]==ws[i],EVLENGTH);)  // verify common frame
  PROD(nfri,af,longs+commonf); PROD(nfro,commonf,longs);   // number of outer loops, number of repeats
  I zn = ndpo*ndpi*nfro; RE(zn=mult(zn,nfri));  // no error possible till we extend the shape
  GA(z,FL>>(it&B01),zn,af+commonf+wcr-1,0); I *zs=AS(z);  // type is INT if inputs booleans, otherwise FL
  // install the shape
  MCISd(zs,longs,af+commonf);
  MCIS(zs,ws+wr-wcr,wcr-1);
 }

 // Convert arguments as required
 if(TYPESNE(it,AT(a))){RZ(a=cvt(it,a));}  // convert to common input type
 if(TYPESNE(it,AT(w))){RZ(w=cvt(it,w));}

 RETF(jtsumattymesprods(jt,it,a,w,dplen,nfro,nfri,ndpo,ndpi,repeata,z);
);
}


static A jtsumattymes(J jt, A a, A w, I b, I t, I m, I n, I nn, I r, I *s, I zn){A z;
 RZ(a&&w);
 switch(CTTZNOFLAG(t)){
 case B01X:  // the aligned cases are handled elsewhere, a word at a time
  {B*av=BAV(a),u,*wv=BAV(w);I*zu,*zv;
   GATV(z,INT,zn,r-1,1+s); zu=AV(z);
   if(1==n){
             zv=zu; DO(m,                     *zv++ =*av++**wv++;);
    DO(nn-1, zv=zu; DO(m,                     *zv+++=*av++**wv++;););
   }else{if(!b){B* tv=av; av=wv; wv=tv;}
             zv=zu; DO(m, u=*av++;      DO(n, *zv++ =u**wv++;););
    DO(nn-1, zv=zu; DO(m, u=*av++; if(u)DO(n, *zv+++=u**wv++;) else wv+=n;););
   }
  }
  break;
#if !SY_64
 case INTX:
  {D u,*zu,*zv;I*av=AV(a),*wv=AV(w);
   GATV(z,FL,zn,r-1,1+s); zu=DAV(z);
   if(1==n){
             zv=zu; DO(m,                        *zv++ =*av++*(D)*wv++;);
    DO(nn-1, zv=zu; DO(m,                        *zv+++=*av++*(D)*wv++;););
   }else{if(!b){I *tv=av; av=wv; wv=tv;}
             zv=zu; DO(m, u=(D)*av++;      DO(n, *zv++ =u**wv++;););
    DO(nn-1, zv=zu; DO(m, u=(D)*av++; if(u)DO(n, *zv+++=u**wv++;) else wv+=n;););
   }
   RZ(z=icvt(z));
  }
  break;
#endif
 case FLX:   
  {D*av=DAV(a),u,v,*wv=DAV(w),*zu,*zv;
   GATV(z,FL,zn,r-1,1+s); zu=DAV(z);
   NAN0;
   // First, try without testing for 0*_ .  If we hit it, it will raise NAN
   if(1==n){
             zv=zu; DO(m, u=*av++;            v=*wv++; *zv++ =u*v; );
    DO(nn-1, zv=zu; DO(m, u=*av++;            v=*wv++; *zv+++=u*v; ););
   }else{if(!b){zv=av; av=wv; wv=zv;}
             zv=zu; DO(m, u=*av++; DO(n, v=*wv++; *zv++ =u*v;););
    DO(nn-1, zv=zu; DO(m, u=*av++; DO(n, v=*wv++; *zv+++=u*v;)););
   }
   if(NANTEST){av-=m*nn;wv-=m*nn*n; // try again, testing for 0*_
    NAN0;
    if(1==n){
              zv=zu; DO(m, u=*av++;            v=*wv++; *zv++ =u&&v?dmul2(u,v):0;  );
     DO(nn-1, zv=zu; DO(m, u=*av++;            v=*wv++; *zv+++=u&&v?dmul2(u,v):0;  ););
    }else{   // don't swap again
              zv=zu; DO(m, u=*av++;      DO(n, v=*wv++; *zv++ =u&&v?dmul2(u,v):0;););
     DO(nn-1, zv=zu; DO(m, u=*av++; if(u)DO(n, v=*wv++; *zv+++=   v?dmul2(u,v):0;) else wv+=n;););
    }
    NAN1;
   }
  }
 }
 RETF(z);
}    /* a +/@:* w for non-scalar a and w */

static C sumbf[]={CSTARDOT,CMIN,CSTAR,CPLUSDOT,CMAX,CEQ,CNE,CSTARCO,CPLUSCO,CLT,CLE,CGT,CGE};

#define SUMBFLOOPW(BF)     \
 {DO(q, memset(tv,C0,p); DO(255, DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);); DO(zn,zv[i]+=tu[i];));  \
        memset(tv,C0,p); DO(r,   DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);); DO(zn,zv[i]+=tu[i];) ;  \
 }
#define SUMBFLOOPX(BF)     \
 {DO(q, memset(tv,C0,p); DO(255, DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);                           \
                               av+=zn; u=(UI*)av; wv+=zn; v=(UI*)wv;); DO(zn,zv[i]+=tu[i];));  \
        memset(tv,C0,p); DO(r,   DO(dw,tv[i]+=BF(*u,*v); ++u; ++v;);                           \
                               av+=zn; u=(UI*)av; wv+=zn; v=(UI*)wv;); DO(zn,zv[i]+=tu[i];) ;  \
 }
#if SY_ALIGN
#define SUMBFLOOP(BF)   SUMBFLOOPW(BF)
#else
#define SUMBFLOOP(BF)   if(zn%SZI)SUMBFLOOPX(BF) else SUMBFLOOPW(BF)
#endif

static A jtsumatgbool(J jt,A a,A w,C id){A t,z;B* RESTRICTI av,* RESTRICTI wv;I dw,n,p,q,r,*s,zn,* RESTRICT zv;UC* RESTRICT tu;UI* RESTRICTI tv,*u,*v;
 RZ(a&&w);
 s=AS(w); n=*s;
 zn=AN(w)/n; dw=(zn+SZI-1)>>LGSZI; p=dw*SZI;
 q=n/255; r=n%255;
 GATV(z,INT,zn,AR(w)-1,1+s); zv=AV(z); memset(zv,C0,zn*SZI);
 GATV(t,INT,dw,1,0); tu=UAV(t); tv=(UI*)tu;
 av=BAV(a); u=(UI*)av; 
 wv=BAV(w); v=(UI*)wv;
 switch(id){
  case CMIN: 
  case CSTAR: 
  case CSTARDOT: SUMBFLOOP(AND ); break;
  case CMAX:
  case CPLUSDOT: SUMBFLOOP(OR  ); break;
  case CEQ:      SUMBFLOOP(EQ  ); break;
  case CNE:      SUMBFLOOP(NE  ); break;
  case CSTARCO:  SUMBFLOOP(NAND); break;
  case CPLUSCO:  SUMBFLOOP(NOR ); break;
  case CLT:      SUMBFLOOP(LT  ); break;
  case CLE:      SUMBFLOOP(LE  ); break;
  case CGT:      SUMBFLOOP(GT  ); break;
  case CGE:      SUMBFLOOP(GE  ); break;
 }
 RETF(z);
}    /* a +/@:g w  for boolean a,w where a-:&(* /@$)w; see also plusinsB */

DF2(jtfslashatg){A fs,gs,y,z;B b,bb,sb=0;C*av,c,d,*wv;I ak,an,ar,*as,at,m,
     n,nn,r,rs,*s,t,wk,wn,wr,*ws,wt,yt,zn,zt;V*sv;VA2 adocv,adocvf;
 RZ(a&&w&&self);
 an=AN(a); ar=AR(a); as=AS(a); at=an?AT(a):B01; sv=FAV(self); 
 wn=AN(w); wr=AR(w); ws=AS(w); wt=wn?AT(w):B01;
 b=ar<=wr; r=b?wr:ar; rs=b?ar:wr; s=b?ws:as; nn=r?s[0]:1;  // b='w has higher rank'; r=higher rank rs=lower rank s->longer shape  nn=#items in longer-shape arg
 ASSERT(!ICMP(as,ws,MIN(ar,wr)),EVLENGTH);
 if(SPARSE&(at|wt)||!an||!wn||2>nn){b=CFORK==sv->id; R df1(df2(a,w,b?sv->fgh[2]:sv->fgh[1]),b?sv->fgh[1]:sv->fgh[0]);}  // if sparse or empty, or just 1 item, do it the old-fashioned way
 rs=MAX(1,rs); PROD(m,rs-1,s+1); PROD(n,r-rs,s+rs); zn=m*n;   // zn=#atoms in _1-cell of longer arg = #atoms in result; m=#atoms in _1-cell of shorter arg  n=#times to repeat shorter arg  (*/ surplus longer shape)
   // if the short-frame arg is an atom, move its rank to 1 so we get the lengths of the _1-cells of the replicated arguments
 if(CFORK==sv->id){fs=sv->fgh[1]; gs=sv->fgh[2];}else{fs=sv->fgh[0]; gs=sv->fgh[1];}
 y=FAV(fs)->fgh[0]; c=ID(y); d=ID(gs);
 if(c==CPLUS){
  if(at&B01&&wt&B01&&1==n&&(0==(zn&(SZI-1))||!SY_ALIGN)&&strchr(sumbf,d))R sumatgbool(a,w,d);
  if(d==CSTAR){
   if(ar&&wr&&TYPESEQ(at,wt)&&at&B01+FL+(INT*!SY_64))R jtsumattymes(jt,a,w,b,at,m,n,nn,r,s,zn);
   if(!ar||!wr){  // if either argument is atomic, apply the distributive property to save multiplies
    z=!ar?tymes(a,df1(w,fs)):tymes(w,df1(a,fs));
    if(jt->jerr==EVNAN)RESETERR else R z;
   }
  }
 }
 adocv=var(gs,at,wt); ASSERT(adocv.f,EVDOMAIN); yt=rtype(adocv.cv ); t=atype(adocv.cv);
 adocvf=var(y,yt,yt); ASSERT(adocvf.f,EVDOMAIN); zt=rtype(adocvf.cv);
 sb=(yt&B01)&&(c==CPLUS);  // +/@:g where g produces Boolean.  Could use &
 if(!(sb||TYPESEQ(yt,zt)))R df1(df2(a,w,gs),fs);
 if(t){
  bb=1&&t&XNUM;
  if(TYPESNE(t,at))RZ(a=bb?xcvt((adocv.cv&VXCVTYPEMSK)>>VXCVTYPEX,a):cvt(t,a));
  if(TYPESNE(t,wt))RZ(w=bb?xcvt((adocv.cv&VXCVTYPEMSK)>>VXCVTYPEX,w):cvt(t,w));
 }
 ak=b?m:zn; wk=b?zn:m; ak=an<nn?0:ak; wk=wn<nn?0:wk; ak*=bp(AT(a));wk*=bp(AT(w));
 GA(y,yt,zn,1,0); 
 GA(z,zt,zn,r-1,1+s);
 if(sb){A t;I j,tn,*zv;UC*tc;UI*ti,*yv;  /* +/@:g for boolean-valued g */
  av=CAV(a); wv=CAV(w); yv=(UI*)AV(y); zv=AV(z); memset(zv,C0,zn*SZI);
  tn=(zn+SZI-1)>>LGSZI; GATV(t,INT,tn,1,0); tc=UAV(t); ti=(UI*)tc;
  for(j=nn;0<j;j-=255){
   memset(ti,C0,tn*SZI); 
   DO(MIN(j,255), adocv.f(jt,b,m,n,yv,av,wv); av+=ak; wv+=wk; DO(tn,ti[i]+=yv[i];););
   DO(zn, zv[i]+=tc[i];);
  }
 }else{A z1;B p=0;C*yv,*zu,*zv;
  av=CAV(a)+ak*(nn-1); wv=CAV(w)+wk*(nn-1); yv=CAV(y); zv=CAV(z);
  GA(z1,zt,zn,r-1,1+s); zu=CAV(z1);
  adocv.f(jt,b,m,n,zv,av,wv);
  DO(nn-1, av-=ak; wv-=wk; adocv.f(jt,b,m,n,yv,av,wv); adocvf.f(jt,b,zn,1L,p?zv:zu,yv,p?zu:zv); p=!p;);
  if(NEVM<jt->jerr){jt->jerr=0; z=df1(df2(a,w,gs),fs);}else if(p)z=z1;
 }
 RE(0); RETF(z);
}    /* a f/@:g w where f and g are atomic*/


// If each argument has a single direct-numeric atom, go process through speedy-singleton code
#define CHECKSSING(a,w,f) RZ(a&&w); if(AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL))))R f(jt,a,w);
#define CHECKSSINGSB(a,w,f) RZ(a&&w); if(HOMO(AT(a),AT(w)) && AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL+SBT))))R f(jt,a,w);
#define CHECKSSINGOP(a,w,f,op) RZ(a&&w); if(AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL))))R f(jt,a,w,op);
#define CHECKSSINGOPSB(a,w,f,op) RZ(a&&w); if(HOMO(AT(a),AT(w)) && AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL+SBT+LIT+C2T+C4T))))R f(jt,a,w,op);
#define CHECKSSINGPROV(a,w,f) RZ(a&&w); if(AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL))))R f(jt,a,w)
#define CHECKSSINGNZ(a,w,f) RZ(a&&w); if(AN(a)==1 && AN(w)==1 && !((AT(a)|AT(w))&UNSAFE(~(B01+INT+FL)))){A z = f(jt,a,w); if(z)R z;}

// These are the entry points for the individual verbs.  They pick up the verb-name
// and transfer to jtva2 which does the work

F2(jtbitwise0000){CHECKSSINGOP(a,w,jtssbitwise,0) R va2(a,w,ds(16));}
F2(jtbitwise0001){CHECKSSINGOP(a,w,jtssbitwise,1) R va2(a,w,ds(17));}
F2(jtbitwise0010){CHECKSSINGOP(a,w,jtssbitwise,2) R va2(a,w,ds(18));}
F2(jtbitwise0011){CHECKSSINGOP(a,w,jtssbitwise,3) R va2(a,w,ds(19));}

F2(jtbitwise0100){CHECKSSINGOP(a,w,jtssbitwise,4) R va2(a,w,ds(20));}
F2(jtbitwise0101){CHECKSSINGOP(a,w,jtssbitwise,5) R va2(a,w,ds(21));}
F2(jtbitwise0110){CHECKSSINGOP(a,w,jtssbitwise,6) R va2(a,w,ds(22));}
F2(jtbitwise0111){CHECKSSINGOP(a,w,jtssbitwise,7) R va2(a,w,ds(23));}

F2(jtbitwise1000){CHECKSSINGOP(a,w,jtssbitwise,8) R va2(a,w,ds(24));}
F2(jtbitwise1001){CHECKSSINGOP(a,w,jtssbitwise,9) R va2(a,w,ds(25));}
F2(jtbitwise1010){CHECKSSINGOP(a,w,jtssbitwise,10) R va2(a,w,ds(26));}
F2(jtbitwise1011){CHECKSSINGOP(a,w,jtssbitwise,11) R va2(a,w,ds(27));}

F2(jtbitwise1100){CHECKSSINGOP(a,w,jtssbitwise,12) R va2(a,w,ds(28));}
F2(jtbitwise1101){CHECKSSINGOP(a,w,jtssbitwise,13) R va2(a,w,ds(29));}
F2(jtbitwise1110){CHECKSSINGOP(a,w,jtssbitwise,14) R va2(a,w,ds(30));}
F2(jtbitwise1111){CHECKSSINGOP(a,w,jtssbitwise,15) R va2(a,w,ds(31));}

F2(jtbitwiserotate){CHECKSSINGOP(a,w,jtssbitwise,16) R genbitwiserotate(a,w);}
F2(jtbitwiseshift){CHECKSSINGOP(a,w,jtssbitwise,17) R genbitwiseshift(a,w);}
F2(jtbitwiseshifta){CHECKSSINGOP(a,w,jtssbitwise,18) R genbitwiseshifta(a,w);}

F2(jteq     ){CHECKSSINGOPSB(a,w,jtsseqne,0) R va2(a,w,ds(CEQ     ));}
F2(jtlt     ){CHECKSSINGSB(a,w,jtsslt) R va2(a,w,ds(CLT     ));}
F2(jtminimum){CHECKSSINGSB(a,w,jtssmin) R va2(a,w,ds(CMIN    ));}
F2(jtle     ){CHECKSSINGSB(a,w,jtssle) R va2(a,w,ds(CLE     ));}
F2(jtgt     ){CHECKSSINGSB(a,w,jtssgt) R va2(a,w,ds(CGT     ));}
F2(jtmaximum){CHECKSSINGSB(a,w,jtssmax) R va2(a,w,ds(CMAX    ));}
F2(jtge     ){CHECKSSINGSB(a,w,jtssge) R va2(a,w,ds(CGE     ));}
F2(jtplus   ){CHECKSSING(a,w,jtssplus) R va2(a,w,ds(CPLUS   ));}
F2(jtgcd    ){CHECKSSING(a,w,jtssgcd) R va2(a,w,ds(CPLUSDOT));}
F2(jtnor    ){CHECKSSING(a,w,jtssnor) R va2(a,w,ds(CPLUSCO ));}
F2(jttymes  ){CHECKSSING(a,w,jtssmult) R va2(a,w,ds(CSTAR   ));}
F2(jtlcm    ){CHECKSSING(a,w,jtsslcm) R va2(a,w,ds(CSTARDOT));}
F2(jtnand   ){CHECKSSING(a,w,jtssnand) R va2(a,w,ds(CSTARCO ));}
F2(jtminus  ){CHECKSSING(a,w,jtssminus) R va2(a,w,ds(CMINUS  ));}
F2(jtdivide ){CHECKSSING(a,w,jtssdiv) R va2(a,w,ds(CDIV    ));}
F2(jtexpn2  ){RZ(w); if(((((I)AR(w)-1)&(AT(w)<<(BW-1-FLX)))<0)&&0.5==*DAV(w))R sqroot(a); CHECKSSINGNZ(a,w,jtsspow) R va2(a,w,ds(CEXP    ));}  // use sqrt hardware for sqrt.  Only for atomic w
F2(jtne     ){CHECKSSINGOPSB(a,w,jtsseqne,1) R va2(a,w,ds(CNE     ));}
F2(jtoutof  ){CHECKSSING(a,w,jtssoutof) R va2(a,w,ds(CBANG   ));}
F2(jtcircle ){R va2(a,w,ds(CCIRCLE ));}
F2(jtresidue){RZ(a&&w); R INT&AT(w)&&equ(a,num[2])?intmod2(w):va2(a,w,ds(CSTILE));}


// These are the unary ops that are implemented using a canned argument

// Shift the w-is-inplaceable flag to a.  Bit 1 is known to be 0 in any call to a monad
#define IPSHIFTWA (jt = (J)(intptr_t)(((I)jt+JTINPLACEW)&-JTINPLACEA))

F1(jtnot   ){R w&&AT(w)&B01+SB01?eq(num[0],w):minus(zeroionei[1],w);}
F1(jtnegate){R minus(zeroionei[0],  w);}
F1(jtdecrem){IPSHIFTWA; R minus(w,     zeroionei[1]);}
F1(jtincrem){R plus(zeroionei[1],   w);}
F1(jtduble ){R tymes(num[2],w);}
F1(jtsquare){R tymes(w,     w);}   // leave inplaceable in w only
F1(jtrecip ){R divide(zeroionei[1],   w);}
F1(jthalve ){if(w&&!(AT(w)&XNUM+RAT))R tymes(onehalf,w); IPSHIFTWA; R divide(w,num[2]);}

static void zeroF(J jt,B b,I m,I n,B*z,void*x,void*y){memset(z,C0,m*n);}
static void  oneF(J jt,B b,I m,I n,B*z,void*x,void*y){memset(z,C1,m*n);}

// Analyze the verb and arguments and come up with *ado, address of the routine to handle one
// list of arguments producing a list of results; and *cv, the conversion control which specifies
// the precision inputs must be converted to, and what the result type will be.
// The flags in cv have doubled letters (e.g. VDD) for input precision, single letters (e. g. VD) for result
// result is a VA2 struct  containing ado and cv.  If failure, ado is 0 and the caller should signal domain error
#if 0
// Returned value is 0 for failure, 1 for success
B jtvar(J jt,C id,A a,A w,I at,I wt,VF*ado,I*cv){B b;I t,x;VA2 *p;
 // If there is a pending error, it might be one that can be cured with a retry; for example, fixed-point
 // overflow, where we will convert to float.  If the error is one of those, get the routine and conversion
 // for it, and return.
 if(jt->jerr){
  switch(VARCASE(jt->jerr,id)){
   default: if(jt->jerr>NEVM){RESETERR ASSERT(0,EVSYSTEM);} R 0;  // Unhandled internal error should not occur.  Force error out
   case VARCASE(EWIMAG,CCIRCLE ): *ado=(VF)cirZZ;   *cv=VZ+VZZ+VRD; break;
   case VARCASE(EWIMAG,CEXP    ): *ado=(VF)powZZ;   *cv=VZ+VZZ+VRD; break;
   case VARCASE(EWIRR ,CBANG   ): *ado=(VF)binDD;   *cv=VD+VDD;     break;
   case VARCASE(EWIRR ,CEXP    ): *ado=(VF)powDD;   *cv=VD+VDD;     break;
   case VARCASE(EWRAT ,CDIV    ): *ado=(VF)divQQ;   *cv=VQ+VQQ;     break;
   case VARCASE(EWRAT ,CEXP    ): *ado=(VF)powQQ;   *cv=VQ+VQQ;     break;
   case VARCASE(EWDIV0,CDIV    ): *ado=(VF)divDD;   *cv=VD+VDD;     break;
   case VARCASE(EWOVIP+EWOVIPPLUSII  ,CPLUS): case VARCASE(EWOVIP+EWOVIPPLUSBI  ,CPLUS): case VARCASE(EWOVIP+EWOVIPPLUSIB  ,CPLUS):
    *ado=(VF)plusIO;  *cv=VD+VII;     break;   // used only for sparse arrays
   case VARCASE(EWOVIP+EWOVIPMINUSII  ,CMINUS): case VARCASE(EWOVIP+EWOVIPMINUSBI  ,CMINUS): case VARCASE(EWOVIP+EWOVIPMINUSIB  ,CMINUS):
    *ado=(VF)minusIO; *cv=VD+VII;     break;   // used only for sparse arrays
   case VARCASE(EWOVIP+EWOVIPMULII,CSTAR): *ado=(VF)tymesIO; *cv=VD+VII;     break;   // used only for sparse arrays
   case VARCASE(EWOV  ,CPLUSDOT): *ado=(VF)gcdIO;   *cv=VD+VII;     break;
   case VARCASE(EWOV  ,CSTARDOT): *ado=(VF)lcmIO;   *cv=VD+VII;     break;
   case VARCASE(EWOV  ,CSTILE  ): *ado=(VF)remDD;   *cv=VD+VDD+VIP;     break;
  }
  RESETERR;
 }else if(!((t=UNSAFE(at|wt))&(NOUN&~NUMERIC))){
  // Normal case where we are not retrying: here for numeric arguments
  // vaptr converts the character pseudocode into an entry in va;
  // that entry contains 34 (ado,cv) pairs, indexed according to verb/argument types.
  // the first 9 entries [0-8] are a 3x3 array of the combinations of the main numeric types
  // B,I,D; then [9] CMPX [10] XINT (but not RAT) [11] RAT [12] SBT (symbol)
  // then [13-19] are for verb/, with precisions B I D Z X Q Symb
  // [20-26] for verb\, and [27-33] for verb\.
  if(t<CMPX) {
   // Here for the fast and important case, where the arguments are both B01/INT/FL
   // The index into va is atype*3 + wtype, calculated sneakily
   p = &va[vaptr[(UC)id]].p2[((at>>1)+((at+wt)>>2))&(CMPX-1)];
   *cv = p->cv;
   jt->mulofloloc = 0;  // Reinit multiplier-overflow count, in case we hit overflow
  } else {
   // Here one of the arguments is CMPX/RAT/XNUM  (we don't support XD and XZ yet)
   // They are in priority order CMPX, FL, RAT, XNUM.  Extract those bits and look up
   // the type to use
   p = &va[vaptr[(UC)id]].p2[xnumpri[((t>>3)&3)+((t>>5)&4)]];  // bits: RAT CMPX FL
   x = p->cv;
   // Some entries specify no input conversion in the (DD,DD) slot.  I don't know why.  But if
   // an input is FL (and remember, the other input is known here to be CMPX, RAT, or XNUM),
   // we'd better specify an input conversion of VDD, unless one is explicitly given, as it will be for the CMPX slot
   if((t&FL)&&!(x&(VBB|VII|VDD|VZZ))){x = (x&(~VARGMSK))|VDD;}   // This is part of where XNUM/RAT is promoted to FL
   *cv = x;
  }
  *ado = p->fgh[0];   // finish getting the output values - cv was done above
 }else{
  // Normal case, but nonnumeric.  This will be a domain error except for = and ~:, and a few symbol operations
  b=!HOMO(at,wt); *cv=VB;  // b = 'inhomogeneous types (always compare not-equal)'; cv indicates no input conversion, boolean result
  { jt->rela=arel;}  // set flags indicating 'indirect datatype' for use during compare
  { jt->relw=wrel;}
  switch(id){
    // for =, it's just 0 for inhomogeneous types, or the routines to handle the other comparisons
   case CEQ: *ado=b?(VF)zeroF:at&SBT?(VF)eqII:at&BOX?(VF)eqAA:
                  at&LIT?(wt&LIT?(VF)eqCC:wt&C2T?(VF)eqCS:(VF)eqCU):
                  at&C2T?(wt&LIT?(VF)eqSC:wt&C2T?(VF)eqSS:(VF)eqSU):
                          wt&LIT?(VF)eqUC:wt&C2T?(VF)eqUS:(VF)eqUU; break;
    // similarly for ~:
   case CNE: *ado=b?(VF) oneF:at&SBT?(VF)neII:at&BOX?(VF)neAA:
                  at&LIT?(wt&LIT?(VF)neCC:wt&C2T?(VF)neCS:(VF)neCS):
                  at&C2T?(wt&LIT?(VF)neSC:wt&C2T?(VF)neSS:(VF)neSU):
                          wt&LIT?(VF)neUC:wt&C2T?(VF)neUS:(VF)neUU; break;
   default:
    // If not = ~:, it had better be a symbol operation.
    ASSERT(at&SBT&&wt&SBT,EVDOMAIN);
    p = &va[vaptr[(UC)id]].p2[12];  // fetch the 'symbol' entry
    ASSERT(p->fgh[0],EVDOMAIN);   // not all verbs support symbols - fail if this one doesn't
    *ado=p->fgh[0]; *cv=p->cv;  // return the values read
  }
 }
 R 1;
}    /* function and control for rank */
#else
VA2 jtvar(J jt,A self,I at,I wt){B b;I t;
 // If there is a pending error, it might be one that can be cured with a retry; for example, fixed-point
 // overflow, where we will convert to float.  If the error is one of those, get the routine and conversion
 // for it, and return.
 if(!jt->jerr){
  VA *vainfo=(VA*)FAV(self)->localuse;  // extract table line from the primitive
  if(!((t=UNSAFE(at|wt))&(NOUN&~NUMERIC))){
  // Normal case where we are not retrying: here for numeric arguments
  // vaptr converts the character pseudocode into an entry in va;
  // that entry contains 34 (ado,cv) pairs, indexed according to verb/argument types.
  // the first 9 entries [0-8] are a 3x3 array of the combinations of the main numeric types
  // B,I,D; then [9] CMPX [10] XINT (but not RAT) [11] RAT [12] SBT (symbol)
  // then [13-19] are for verb/, with precisions B I D Z X Q Symb
  // [20-26] for verb\, and [27-33] for verb\.
  if(t<CMPX) {
   // Here for the fast and important case, where the arguments are both B01/INT/FL
   // The index into va is atype*3 + wtype, calculated sneakily
   jt->mulofloloc = 0;  // Reinit multiplier-overflow count, in case we hit overflow
   R vainfo->p2[(UNSAFE(at)>>1)+((UNSAFE(at)+UNSAFE(wt))>>2)];
  } else {
   // Here one of the arguments is CMPX/RAT/XNUM  (we don't support XD and XZ yet)
   // They are in priority order CMPX, FL, RAT, XNUM.  Extract those bits and look up
   // the type to use
   VA2 selva2 = vainfo->p2[xnumpri[((t>>3)&3)+((t>>5)&4)]];  // bits: RAT CMPX FL
   // Some entries specify no input conversion in the (DD,DD) slot.  I don't know why.  But if
   // an input is FL (and remember, the other input is known here to be CMPX, RAT, or XNUM),
   // we'd better specify an input conversion of VDD, unless one is explicitly given, as it will be for the CMPX slot
   if((t&FL)&&!(selva2.cv&(VBB|VII|VDD|VZZ))){selva2.cv = (selva2.cv&(~VARGMSK))|VDD;}   // This is part of where XNUM/RAT is promoted to FL
   R selva2;
  }
 }else{
  // Normal case, but nonnumeric.  This will be a domain error except for = and ~:, and a few symbol operations
  VA2 retva2;  // where we build the return value
  UC id=(UC)FAV(self)->id;  // extract the 
  b=!HOMO(at,wt); retva2.cv=VB;  // b = 'inhomogeneous types (always compare not-equal)'; cv indicates no input conversion, boolean result
  switch(id){
    // for =, it's just 0 for inhomogeneous types, or the routines to handle the other comparisons
   case CEQ: retva2.f=b?(VF)zeroF:at&SBT?(VF)eqII:at&BOX?(VF)eqAA:
                  at&LIT?(wt&LIT?(VF)eqCC:wt&C2T?(VF)eqCS:(VF)eqCU):
                  at&C2T?(wt&LIT?(VF)eqSC:wt&C2T?(VF)eqSS:(VF)eqSU):
                          wt&LIT?(VF)eqUC:wt&C2T?(VF)eqUS:(VF)eqUU; break;
    // similarly for ~:
   case CNE: retva2.f=b?(VF) oneF:at&SBT?(VF)neII:at&BOX?(VF)neAA:
                  at&LIT?(wt&LIT?(VF)neCC:wt&C2T?(VF)neCS:(VF)neCS):
                  at&C2T?(wt&LIT?(VF)neSC:wt&C2T?(VF)neSS:(VF)neSU):
                          wt&LIT?(VF)neUC:wt&C2T?(VF)neUS:(VF)neUU; break;
   default:
    // If not = ~:, it had better be a symbol operation.
    if(!(at&wt&SBT)){retva2.f=0; R retva2;}  // if not symbol, return not found
    R vainfo->p2[12];  // fetch the 'symbol' entry and return it - it may be not found too
  }
  R retva2;  // mult be =/~:, return what we created
 }
 }else{VA2 retva2;
  retva2.f=0;  // error if not filled in
  switch((UC)FAV(self)->id){
  case CCIRCLE: if(jt->jerr==EWIMAG){retva2.f=(VF)cirZZ; retva2.cv=VZ+VZZ+VRD;} break;
  case CEXP: if(jt->jerr==EWIMAG){retva2.f=(VF)powZZ; retva2.cv=VZ+VZZ+VRD;}
             else if(jt->jerr==EWRAT){retva2.f=(VF)powQQ; retva2.cv=VQ+VQQ;}
             else if(jt->jerr==EWIRR){retva2.f=(VF)powDD; retva2.cv=VD+VDD;} break;
  case CBANG: if(jt->jerr==EWIRR){retva2.f=(VF)binDD; retva2.cv=VD+VDD;} break;
  case CDIV: if(jt->jerr==EWRAT){retva2.f=(VF)divQQ; retva2.cv=VQ+VQQ;}
             else if(jt->jerr==EWDIV0){retva2.f=(VF)divDD; retva2.cv=VD+VDD;} break;
  case CPLUS: if(jt->jerr==EWOVIP+EWOVIPPLUSII||jt->jerr==EWOVIP+EWOVIPPLUSBI||jt->jerr==EWOVIP+EWOVIPPLUSIB){retva2.f=(VF)plusIO; retva2.cv=VD+VII;} break;
  case CMINUS: if(jt->jerr==EWOVIP+EWOVIPMINUSII||jt->jerr==EWOVIP+EWOVIPMINUSBI||jt->jerr==EWOVIP+EWOVIPMINUSIB){retva2.f=(VF)minusIO; retva2.cv=VD+VII;} break;
  case CSTAR: if(jt->jerr==EWOVIP+EWOVIPMULII){retva2.f=(VF)tymesIO; retva2.cv=VD+VII;} break;
  case CPLUSDOT: if(jt->jerr==EWOV){retva2.f=(VF)gcdIO; retva2.cv=VD+VII;} break;
  case CSTARDOT: if(jt->jerr==EWOV){retva2.f=(VF)lcmIO; retva2.cv=VD+VII;} break;
  case CSTILE: if(jt->jerr==EWOV){retva2.f=(VF)remDD; retva2.cv=VD+VDD+VIP;} break;
  }
  if(retva2.f){RESETERR}else{if(jt->jerr>NEVM){RESETERR jsignal(EVSYSTEM);}}  // system error if unhandled exception.  Otherwise reset error only if we handled it
  R retva2;
 }
}    /* function and control for rank */
#endif
