% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/alocv.R
\docType{package}
\name{alocv}
\alias{alocv}
\title{alocv: A package for computing unbiased risk estimates for high-dimensional models.}
\usage{
alocv(fit, x, y, ...)
}
\arguments{
\item{fit}{The fitted object.}

\item{x}{The predictor or feature matrix.}

\item{y}{The response vector.}

\item{...}{Arguments to be passed to methods.}
}
\description{
alocv: A package for computing unbiased risk estimates for high-dimensional models.

Computes the approximate cross-validation value for a fitted
classifier or regressor.
}
\seealso{
\code{\link{alocv.glmnet}}

\code{\link{alocv.svm}}
}
