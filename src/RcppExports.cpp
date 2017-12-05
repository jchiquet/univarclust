// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// vCalculeClassif
IntegerVector vCalculeClassif(const NumericVector& mesLambdas, const NumericVector& lambdas, const IntegerVector& idown, const IntegerVector& iup, const IntegerVector& fils, const IntegerVector& class0, const int& longueurClassif);
RcppExport SEXP _fusedanova_vCalculeClassif(SEXP mesLambdasSEXP, SEXP lambdasSEXP, SEXP idownSEXP, SEXP iupSEXP, SEXP filsSEXP, SEXP class0SEXP, SEXP longueurClassifSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericVector& >::type mesLambdas(mesLambdasSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type lambdas(lambdasSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type idown(idownSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type iup(iupSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type fils(filsSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type class0(class0SEXP);
    Rcpp::traits::input_parameter< const int& >::type longueurClassif(longueurClassifSEXP);
    rcpp_result_gen = Rcpp::wrap(vCalculeClassif(mesLambdas, lambdas, idown, iup, fils, class0, longueurClassif));
    return rcpp_result_gen;
END_RCPP
}
// CalculeFils
IntegerVector CalculeFils(const IntegerVector& idown, int longueurClassif);
RcppExport SEXP _fusedanova_CalculeFils(SEXP idownSEXP, SEXP longueurClassifSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const IntegerVector& >::type idown(idownSEXP);
    Rcpp::traits::input_parameter< int >::type longueurClassif(longueurClassifSEXP);
    rcpp_result_gen = Rcpp::wrap(CalculeFils(idown, longueurClassif));
    return rcpp_result_gen;
END_RCPP
}

RcppExport SEXP noSplit(SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP noSplitcv(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP Splitcv(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP withSplit(SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"_fusedanova_vCalculeClassif", (DL_FUNC) &_fusedanova_vCalculeClassif, 7},
    {"_fusedanova_CalculeFils", (DL_FUNC) &_fusedanova_CalculeFils, 2},
    {"noSplit",                     (DL_FUNC) &noSplit,                     4},
    {"noSplitcv",                   (DL_FUNC) &noSplitcv,                   6},
    {"Splitcv",                     (DL_FUNC) &Splitcv,                     6},
    {"withSplit",                   (DL_FUNC) &withSplit,                   4},
    {NULL, NULL, 0}
};

RcppExport void R_init_fusedanova(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
