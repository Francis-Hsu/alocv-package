#include "alocv/alo_config.h"
#include <vector>

/*! For a given coefficient set beta, finds the active set by a magnitude-based rule.
 *
 *  @param[in] p The number of coefficients.
 *  @param[in] beta A pointer to the coefficients.
 *  @param[in] tolerance The tolerance to determine which values are 0.
 */
std::vector<blas_size> find_active_set(blas_size p, const double* beta, double tolerance);

/*! Finds the largest active set in the given set of solutions.
 *
 * @param num_tuning The number of rows of B
 * @param p The number of columns of B
 * @param[in] B A pointer to the coefficients for each value of the regularizer.
 * @param ldb the leading dimension of B.
 * @param tolerance The tolerance to determine which values are 0.
 */
blas_size max_active_set_size(blas_size num_tuning, blas_size p, const double* B, blas_size ldb, double tolerance);


/*! Compute the ALO value
 *
 * @param n: The number of observations
 * @param p: The number of parameters
 * @param[in] A: A n x p matrix representing the observed data.
 * @param lda: The leading dimension of A
 * @param[in] y: A vector of length n representing the observed values.
 * @param[in] beta: A vector of length p representing fitted coefficients.
 * @param[in] leverage: A vector of length n representing the leverage value for each observation.
 * 
 * @returns The ALO mean-squared error.
 */
double compute_alo(blas_size n, blas_size p, const double* A, blas_size lda, const double* y,
                   const double* beta, const double* leverage);