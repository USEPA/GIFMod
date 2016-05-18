#include "Reaction.h"


CReaction::CReaction(void)
{


}

CReaction::CReaction(int nterms)
{
	terms.resize(nterms);

}

CReaction::CReaction(const CReaction &CR)
{
	nproducts = CR.nproducts;
	prodrates = CR.prodrates; 
	nterms = CR.nterms;
	products = CR.products;
	product_phase = CR.product_phase;
	product_p_type = CR.product_p_type;
	RxnID = CR.RxnID;
	temp_exponent = CR.temp_exponent;
	rate = CR.rate;
	terms = CR.terms;
	name = CR.name;

}

CReaction CReaction::operator=(const CReaction &CR)
{
	nproducts = CR.nproducts;
	nterms = CR.nterms;
	products = CR.products;
	product_phase = CR.product_phase;
	product_p_type = CR.product_p_type;
	RxnID = CR.RxnID;
	temp_exponent = CR.temp_exponent;
	terms = CR.terms;
	rate = CR.rate;
	name = CR.name;
	return *this;
}

CReaction::~CReaction(void)
{
	terms.clear();
}


void CReaction::set_n_terms(int n_terms)
{
	nterms = n_terms;
	terms.resize(nterms); 
}

void CReaction::set_n_prdcts(int n_prods)
{
	nproducts = n_prods;
	products.resize(nproducts);
	prodrates.resize(nproducts);
	product_phase.resize(nproducts);
}

void CReaction::getRxn(FILE *file)
{


}