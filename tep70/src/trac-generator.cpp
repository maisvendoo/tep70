#include    "trac-generator.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TracGenerator::TracGenerator(QObject *parent) : Device(parent)
  , Uf(0.0)
  , In(0.0)
  , omega(0.0)
  , Ra(0.0011)
  , Rf(0.535)
  , M(0.0)
  , U(0.0)
  , Tf(0.1)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TracGenerator::~TracGenerator()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TracGenerator::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    U = cPhi(Y[0]) * omega - Ra * In;

    M = cPhi(Y[0]) * In;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TracGenerator::ode_system(const state_vector_t &Y,
                               state_vector_t &dYdt,
                               double t)
{
    Q_UNUSED(t)

    dYdt[0] = (Uf - Rf * Y[0]) / Rf / Tf;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TracGenerator::load_config(CfgReader &cfg)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
double TracGenerator::cPhi(double If)
{
    return magnetic_char.getValue(If);
}
