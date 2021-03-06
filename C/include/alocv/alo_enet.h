#ifndef WENDA_ALO_ENET_H_INCLUDED
#define WENDA_ALO_ENET_H_INCLUDED

#include "alo_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GlmFamilyGaussian = 0,
    GlmFamilyPoisson = 1,
    GlmFamilyLogit = 2,
} GlmFamily;

/*! Computes the ALOCV value for the elastic-net problem.
 *
 * @param n: The number of observations.
 * @param p: The number of parameters.
 * @param m: The number of fitted values.
 * @param[in] A: The predictor matrix, a n x p matrix in column-major order.
 * @param[in] lda: The leading dimension of A.
 * @param[in] B: The fitted values, a p x m matrix in column-major order.
 * @param ldb: The leading dimension of B.
 * @param[in] y: The vector of observed responses, a vector of length n.
 * @param[in, optional] a0: A vector of fitted intercept values, or NULL if no intercept has been fitted.
 * @param[in] lambda: The vector of regularization values, a vector of length m.
 * @param alpha: The elastic net parameter.
 * @param has_intercept: Whether the model was fitted with an intercept term.
 * @param family: The family of the GLM that was fitted.
 * @param use_rfp: Whether to use the rectangular packed format or computations.
 *      This uses significantly less memory.
 * @param[out] alo: A vector of length m, will contain the ALOCV value for each tuning.
 * @param[out, optional] leverage: If provided, a n x m matrix which will contain the
 *      leverage values for each observation and tuning.
 * @param[out, optional] alo_mse: A vector of length m, will contain the mean-squared error ALOCV value.
 * @param[out, optional] alo_mae: A vector of length m, will contain the mean absolute error ALOCV value.
 * 
 */
void enet_compute_alo_d(blas_size n, blas_size p, blas_size m, const double* A, blas_size lda,
                        const double* B, blas_size ldb, const double* y, const double* a0,
                        const double* lambda, double alpha,
                        int has_intercept, GlmFamily family, int use_rfp, double tolerance,
                        double* alo, double* leverage, double* alo_mse, double* alo_mae);

#ifdef __cplusplus
} // extern "C"
#endif


#endif // WENDA_ALO_ENET_H_INCLUDED