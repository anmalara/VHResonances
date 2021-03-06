/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Landau core + exponential tail
// Andrea Malara
// 01/08/2019

#include "Riostream.h" 

#include "HiggsAnalysis/CombinedLimit/interface/PolinomialExponent_3p.hpp"
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(PolinomialExponent_3p)

PolinomialExponent_3p::PolinomialExponent_3p(const char *name, const char *title, RooAbsReal& _x, RooAbsReal& _e1, RooAbsReal& _e2,RooAbsReal& _e3) :
RooAbsPdf(name,title), x("x","x",this,_x),  e1("e1","e1",this,_e1), e2("e2","e2",this,_e2), e3("e3","e3",this,_e3) {
}


PolinomialExponent_3p::PolinomialExponent_3p(const PolinomialExponent_3p& other, const char* name) :
RooAbsPdf(other,name), x("x",this,other.x), e1("e1",this,other.e1), e2("e2",this,other.e2), e3("e3",this,other.e3) {
}

Double_t PolinomialExponent_3p::evaluate() const {
  Double_t xp = x/1000.;
  Double_t result = exp(e1*xp+e2*xp*xp+e3*xp*xp*xp);
  return result;
}
