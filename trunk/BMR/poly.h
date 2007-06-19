#ifndef POLYCHOTOMOUS_MAIN_HEADER_
#define POLYCHOTOMOUS_MAIN_HEADER_

#include "Matrix.h"
#include "data.h"
#include "BayesParamManager.h"
#include "ModelTypeParam.h"
#include "Design.h"

class IParamMatrix {
public:
    virtual unsigned D() const =0;
    virtual unsigned C() const =0;
    virtual double& operator()(unsigned j, unsigned k) =0;
    virtual double operator() (unsigned j, unsigned k) const =0;
    virtual const vector<double>& classparam( unsigned k ) const =0;
    virtual void reset( unsigned d_, unsigned c_, double v=0.0) =0;
};

class ParamMatrixSquare : public IParamMatrix {
    vector< vector<double> > m; //replace by sparse row storage when needed
    unsigned d; //features
    unsigned c; //classes
public:
    unsigned D() const { return d; }
    unsigned C() const { return c; }
    double& operator()(unsigned j, unsigned k) {
        if( j>=d || k>=c )  throw DimensionConflict(__FILE__,__LINE__);
        return m[k][j];
    }
    double operator()(unsigned j, unsigned k) const {
        if( j>=d || k>=c )  throw DimensionConflict(__FILE__,__LINE__);
        return m[k][j];
    }
    const vector<double>& classparam( unsigned k ) const { return m[k]; }
    bool active(unsigned j, unsigned k) const { return true; } //temp
    void reset( unsigned d_, unsigned c_, double v=0.0)
    {
        d = d_; c = c_;
        m = vector< vector<double> >( c, vector<double>(d,v) );
    }
    //ctor
    ParamMatrixSquare() {};
    ParamMatrixSquare( unsigned d_, unsigned c_, double v=0.0)
        : d(d_), c(c_)//, m( c, vector<double>(d,v) )  
    {
        m = vector< vector<double> >( c, vector<double>(d,v) );
    }
};
inline std::ostream& operator<<( std::ostream& s, const IParamMatrix& m ) {
    s<<endl<<m.D()<<" "<<m.C();
    for( size_t j=0; j<m.D(); j++ ) {
        s<<endl<<j;
        for( size_t k=0; k<m.C(); k++ ) 
            s<<" "<<m(j,k);
    }
    return s;
}

class FixedParams {
    const vector< vector<bool> > m_allzeroes;  //jk
    bool m_referenceClass;
    int m_referenceClassId;
    mutable int nfixed;
public:
    bool z_active() const { return m_allzeroes.size()>0; }
    bool operator() (unsigned j, unsigned k) const {
       if( !z_active() && !m_referenceClass ) return false;
        if( m_referenceClass && k==m_referenceClassId ) return true; //bug k==m_allzeroes[0].size()-1 ver2.02
        if( j>=m_allzeroes.size() ) return false; // btw takes care if intercept is unaccounted for
        if( k>=m_allzeroes[j].size() ) return false;
        return m_allzeroes[j][k];
    }
    int refClassId() const { return m_referenceClassId; }
    FixedParams( const vector< vector<bool> >& allzeroes, bool referenceClass, int referenceClassId=0 ) 
        : m_allzeroes(allzeroes), m_referenceClass(referenceClass),  m_referenceClassId(referenceClassId), nfixed(-1)
    {}
    /*vector<unsigned> varStat() const {
        vector<unsigned> stat;
        for( unsigned j=0; j<m_allzeroes.size(); j++ ) {
            unsigned n=0;
            for( unsigned k=0; k<m_allzeroes[j].size(); k++ )
                if( (*this)(j,k) ) n++;
            stat.push_back( n );
        }
        return stat;
    }*/
    unsigned count() const {
        if( nfixed>=0 ) return nfixed; //already set
        nfixed=0;
        if( !z_active() && !m_referenceClass ) ;
        else
            for( unsigned j=0; j<m_allzeroes.size(); j++ )
                for( unsigned k=0; k<m_allzeroes[j].size(); k++ )
                    if( (*this)(j,k) ) nfixed++;
        return nfixed;
    }
};

enum ResultFormat { resProb, resScore };

template<class paramItr> 
double dotSparse( 
    SparseVector::const_iterator vecFrom, SparseVector::const_iterator vecTo, 
//    SparseVecConstItr paramFrom, SparseVecConstItr paramTo )
    paramItr paramFrom, paramItr paramTo )
/* assumption: both vectors are indexed by offset in 'featSelect', i.e. indices start from 0
 * This is guaranteed by DesignRowSet
 */
{
    double score = 0.0;
    while( vecFrom!=vecTo && paramFrom!=paramTo )
        if( vecFrom->first < paramFrom.var() )  ++vecFrom;
        else if( paramFrom.var() < vecFrom->first )  ++paramFrom;
        else { //equal
            score += vecFrom->second * paramFrom.val();
            ++vecFrom;
            ++paramFrom;
        }
    return score;
}

/*double dotSparse( 
    SparseVector::const_iterator vecFrom, SparseVector::const_iterator vecTo, 
    SparseVecConstItr paramFrom, SparseVecConstItr paramTo );*/
//double dotSparseByDense( const SparseVector& x, const vector<double>& b );

Vector score( const Vector& beta, const Matrix& tstx );
double dotSparseByDense( const SparseVector& x, const vector<double>& b );
double tuneThreshold( const valarray<double>& score, const BoolVector& y, const class ModelType& modelType );
void makeConfusionTable( ostream& o, INameResolver& names, 
                           const vector<unsigned>& y, const vector<unsigned>& prediction );
void makeCT2by2( ostream& o, INameResolver& names, 
                           const vector<unsigned>& y, 
                           const vector< vector<double> >& allScores,
                           const vector<unsigned>& prediction );
/*void  displayEvaluation( ostream& o, const BoolVector& y, const BoolVector& prediction, 
                        const string& comment =string() );
void  displayEvaluationCT( ostream& o, int TP, int FP, int FN, int TN );*/
unsigned argmax( const vector<double>& score );
vector<double> estprob( const vector<double>& score );
void testModel( const char* topic,
                const class ModelType& modelType,
                const IParamMatrix& beta,
                double threshold,
                IRowSet & testDesignData,
                std::ostream& result, ResultFormat resultFormat=resProb );
double PointLogLikelihood( const vector<double>& linscores, unsigned y, const class ModelType& modelType );

class LRModel {
    bool m_bTrained;
    std::string m_topic;
    class IDesign* m_pDesign;
    vector<int> m_featSelect;
    ParamMatrixSquare m_beta;
    double m_threshold;
    class ModelType m_modelType;
    BayesParameter m_bayesParam;
public:
    void Train( const char* topic,
        RowSetMem & trainData,
        const class HyperParamPlan& hyperParamPlan,
        const class PriorTermsByTopic& priorTermsByTopic,
        const class DesignParameter& designParameter,
        const class ModelType& modelType,
        class WriteModel& modelFile,
        std::ostream& result, ResultFormat resultFormat );
    void Restore( class ReadModel& modelFile, const INameResolver& names );
    void Test( IRowSet & TestRowSet, std::ostream& result, ResultFormat resultFormat );
    //ctor-dtor
    LRModel()   : m_pDesign(0), m_bTrained(false) {}
    ~LRModel() { delete m_pDesign; }
private:
    void tuneModel(   IRowSet & drs,
                    const HyperParamPlan& hyperParamPlan,
                    const IParamMatrix& priorMean, const IParamMatrix& priorScale,
                    const class FixedParams& fixedParams,
                    class Stats& stats );
    void squeezerModel(   IRowSet & drs,
                    const BayesParameter& bayesParameter,
                    unsigned squeezeTo,
                    double hpStart,
                    const Vector& priorMean, const Vector& priorScale );
};

#endif //POLYCHOTOMOUS_MAIN_HEADER_

/*
    Copyright 2005, Rutgers University, New Brunswick, NJ.

    All Rights Reserved

    Permission to use, copy, and modify this software and its documentation for any purpose 
    other than its incorporation into a commercial product is hereby granted without fee, 
    provided that the above copyright notice appears in all copies and that both that 
    copyright notice and this permission notice appear in supporting documentation, and that 
    the names of Rutgers University, DIMACS, and the authors not be used in advertising or 
    publicity pertaining to distribution of the software without specific, written prior 
    permission.

    RUTGERS UNIVERSITY, DIMACS, AND THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO 
    THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
    ANY PARTICULAR PURPOSE. IN NO EVENT SHALL RUTGERS UNIVERSITY, DIMACS, OR THE AUTHORS 
    BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER 
    RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
    NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
    PERFORMANCE OF THIS SOFTWARE.
*/
