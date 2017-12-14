#include <Rcpp.h>
#include <bits/stdc++.h>
#include "nosplit.h"
#include <stdio.h>

using namespace Rcpp ;

class Rule {
  
private:
  int group1 ;
  int group2 ;
  double lambda ;
  
public:
  Rule(int group1_, int group2_, double lambda_) : group1(group1_),  group2(group2_), lambda(lambda_) {} ;

  int get_group1() const {return group1 ;}  
  int get_group2() const {return group2 ;}  
  double get_lambda() const {return lambda ;}
  bool is_active(const LogicalVector& active) const {
    return(active[group1] & active[group2]);
  }
};

class RuleComparator {
public:
  int operator() (const Rule& r1, const Rule& r2) {
    return r1.get_lambda() > r2.get_lambda();
  }
};

// double get_lambda(int group1, int group2, const DataFrame& table) {
double compute_lambda(int group1, int group2, const NumericVector& beta, const NumericVector& lambda, const NumericVector& slope) {

  return((beta[group1] - beta[group2] - slope[group1] * lambda[group1] + slope[group2] * lambda[group2]) / (slope[group2] - slope[group1]));
}

//' @export
// [[Rcpp::export]]
DataFrame fuse(NumericVector beta0, NumericVector slope0, IntegerVector grp_size0) {

  // VARIABLES DECLARATION
  int n = grp_size0.size() ;
  NumericVector lambda       (2 * n - 1) ; 
  NumericVector beta         (2 * n - 1) ;
  NumericVector slope        (2 * n - 1) ;
  IntegerVector i_low        (2 * n - 1) ;
  IntegerVector i_split      (2 * n - 1) ;
  IntegerVector i_high       (2 * n - 1) ;
  IntegerVector grp_low      (2 * n - 1) ;
  IntegerVector grp_high     (2 * n - 1) ;
  IntegerVector grp_size     (2 * n - 1) ;
  LogicalVector active       (2 * n - 1) ;
  LogicalVector has_grp_low  (2 * n - 1) ;
  LogicalVector has_grp_high (2 * n - 1) ;
  priority_queue <Rule, vector<Rule>, RuleComparator> myMinHeap ;

  // INITIALIZATION OF THE N-1 GROUPS 
  for (int k=0; k < n; k++) {
    lambda[k]   = 0.0;
    beta[k]     = beta0[k];
    slope[k]    = slope0[k];
    i_low[k]    = k;
    i_split[k]  = k;
    i_high[k]   = k;
    grp_size[k] = grp_size0[k] ;
    active[k]   = true;
    if (k == 0) {
      has_grp_low[k] = false ;
      grp_low[k] = -1;
    } else {
      has_grp_low[k] = true ;
      grp_low[k] = k - 1;
    }
    if (k == (n-1)) {
      has_grp_high[k] = false ;
      grp_high[k] = -1 ;
    } else {
      has_grp_high[k] = true ;
      grp_high[k] = k + 1;
    }
  }
  
  // FIRST SET OF RULE BETWEEN THE N-2 PAIRS OF GROUPS AT THE BOTTOM OF THE TREE
  for (int k=0; k < (n-1); k++) {
    myMinHeap.push(Rule(k, k + 1, compute_lambda(k, k + 1, beta, lambda, slope)));
  }

  // n-1 kS WILL OCCUR  
  for (int k = n; k < (2*n-1);  k++) {
    
    // FIND THE NEXT ACTIVE RULE
    while (!myMinHeap.top().is_active(active)) {
      myMinHeap.pop() ;
    }
    // std::cout << "k = " << k - (n-1) + 1 
    //           << ", heapSize = " << myMinHeap.size() 
    //           << ", active groups = " << sum(active) << std::endl;
    Rule rule_ = myMinHeap.top();
    myMinHeap.pop() ;
    
    // MERGE THE TWO FUSING GROUP
    // Updating the fields of the DataFrame of the fusing group
    int group1 = rule_.get_group1(), group2 = rule_.get_group2();
    double lambda_k = rule_.get_lambda();
    // merge(k, rule_.get_lambda(), rule_.get_group1(), rule_.get_group2(), table) ;
    
    lambda  [k] = lambda_k ;
    grp_size[k] = grp_size[group1] + grp_size[group2] ;
    slope   [k] = (grp_size[group1] * slope[group1] + grp_size[group2] * slope[group2]) / grp_size[k] ;
    beta    [k] = beta[group1] + (lambda_k - lambda[group1]) * slope[group1] ;
    if(i_low[group1] < i_low[group2]) {
      i_split     [k] = i_high      [group1];
      i_low       [k] = i_low       [group1];
      i_high      [k] = i_high      [group2];
      grp_low     [k] = grp_low     [group1];
      grp_high    [k] = grp_high    [group2];
      has_grp_low [k] = has_grp_low [group1];
      has_grp_high[k] = has_grp_high[group2];
    } else {
      i_split     [k] = i_high      [group2]; 
      i_low       [k] = i_low       [group2];
      i_high      [k] = i_high      [group1];
      grp_low     [k] = grp_low     [group2];
      grp_high    [k] = grp_high    [group1];
      has_grp_low [k] = has_grp_low [group2];
      has_grp_high[k] = has_grp_high[group1];
    }
    
    // Updating neighbors
    if(has_grp_low [k])  grp_high[  grp_low [k] ] = k;
    if(has_grp_high [k])  grp_low[ grp_high [k] ] = k;
    
    active[group1] = false ;
    active[group2] = false ;
    active[k] = true ;
    
    // get new rules and add them to the queue
    if (has_grp_low[k] & active[grp_low[k]]) {
      myMinHeap.push(Rule(grp_low[k], k, compute_lambda(grp_low[k], k, beta, lambda, slope)));
    }
    
    if (has_grp_high[k] & active[grp_high[k]]) {
      myMinHeap.push(Rule(k, grp_high[k], compute_lambda(k, grp_high[k], beta, lambda, slope)));
    }
  }
  
  return(DataFrame::create(Named("beta"  ) = tail(beta       , n-1),
                           Named("lambda") = tail(lambda     , n-1),
                           Named("slope" ) = tail(slope      , n-1),
                           Named("down"  ) = tail(i_low   + 1, n-1),
                           Named("high"  ) = tail(i_high  + 1, n-1),
                           Named("split" ) = tail(i_split + 1, n-1)));
  
}

//' @export
// [[Rcpp::export]]
DataFrame fuse_old(NumericVector x, NumericVector slopes, NumericVector ngroup){

	Group *G = maketree(&x[0], x.length(), &slopes[0], &ngroup[0]);

  int nrow = 2*G->len -1; // nb infos = K init gpes + K-1 ks 
  
  NumericVector beta(nrow), lambda(nrow), slope(nrow);
  IntegerVector idown(nrow), iup(nrow);
  
  int row=0;
  
  add_results(G, &beta[0], &lambda[0], &slope[0], &idown[0], &iup[0], &row);
  
  DataFrame L = DataFrame::create(Named("beta"  , beta),
                                  Named("lambda", lambda),
                                  Named("slope" , slope),
                                  Named("idown" , idown),
                                  Named("iup"   , iup));
	
	delete_tree(G); // delete the old tree for memory

	return L;
}

// we process each dimension individually using this function
// RcppExport SEXP noSplitcv(SEXP R_x,SEXP R_xv,SEXP R_ngroup, SEXP R_xtest,SEXP R_ngrouptest ,SEXP R_args) {
// 
// 	NumericVector x(R_x);
// 	NumericVector xv(R_xv);
// 	NumericVector xtest(R_xtest);
// 	NumericVector ngroup(R_ngroup);
// 	NumericVector ngrouptest(R_ngrouptest);
// 	List args(R_args);
//   
// 	std::string weights = Rcpp::as<std::string>(args["weights"]); 
// 	double gamma = Rcpp::as<double>(args["gamma"]); 
// 	NumericMatrix W = args["W"];
// 	NumericVector lambdalist = args["lambdalist"];
// 
// 	NumericVector error(lambdalist.length());
// 
// 	vector<double> sl = calculateSlope(x,ngroup,xv,weights,gamma,W,x.length());
//  
// 	Group *G = maketree(&x[0], x.length(), &sl[0],&ngroup[0]);
// 
// 	error_cv(G,&lambdalist[0],lambdalist.length(),&xtest[0], &ngrouptest[0],&error[0]);
// 
// 	delete_tree(G); 
// 
// 	return(error);
// }
// 
