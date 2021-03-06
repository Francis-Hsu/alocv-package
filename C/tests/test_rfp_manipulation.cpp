#include "catch.hpp"
#include <gram_utils.hpp>
#include <blas_configuration.h>

#include <algorithm>
#include <cmath>
#include <random>

#include "rfp_utils.h"


namespace {

bool increment_diag_correct(int n, bool skip_first) {
	auto init_values = make_random_matrices(n);

	auto X = init_values.first.get();
	auto X_rf = init_values.second.get();

	std::vector<double> initial_diagonal(n);

	for (int i = 0; i < n; ++i) {
		initial_diagonal[i] = X[i + n * i];
	}

    offset_diagonal(n, X_rf, 3.5, skip_first, SymmetricFormat::RFP);
	int info;
    dtfttr("N", "L", &n, X_rf, X, &n, &info);

    for(int i = skip_first ? 1 : 0; i < n; ++i) {
        if(X[i + n * i] != initial_diagonal[i] + 3.5) {
            return false;
        }
    }

    if(skip_first && X[0] != 0) {
        return false;
    }

    return true;
}

}

TEST_CASE("Offset Diagonal Correct for RFP (even)", "[RFP]") {
    REQUIRE(increment_diag_correct(6, false));
}

TEST_CASE("Offset Diagonal Correct for RFP (odd)", "[RFP]") {
    REQUIRE(increment_diag_correct(5, false));
}

TEST_CASE("Offset Diagonal with skip Correct for RFP (even)", "[RFP]") {
    REQUIRE(increment_diag_correct(6, true));
}

TEST_CASE("Offset Diagonal with skip correct for RFP (odd)", "[RFP]") {
    REQUIRE(increment_diag_correct(5, true));
}


namespace {

int test_diagonal_extract(int n) {
	auto init_values = make_random_matrices(n);

	auto X = init_values.first.get();
	auto X_rf = init_values.second.get();

	for (int i = 0; i < n; ++i) {
		auto diag = diagonal_element(n, X_rf, i, SymmetricFormat::RFP);

		if (diag != X[i + i * n]) {
			return i;
		}
	}

	return -1;
}

}

TEST_CASE("Extract Diagonal Element Correct for RFP (even)", "[RFP]") {
	REQUIRE(test_diagonal_extract(4) == -1);
}

TEST_CASE("Extract Diagonal Element Correct for RFP (odd)", "[RFP]") {
	REQUIRE(test_diagonal_extract(5) == -1);
}

namespace {

std::pair<std::vector<double>, std::vector<double>> test_copy(int n, int k) {
	auto init_triangular = make_random_matrices(n, true);

	auto lhs_full = init_triangular.first.get();
	auto lhs_rfp = init_triangular.second.get();

	std::vector<double> my_copy(n);
	std::fill(my_copy.begin(), my_copy.end(), 0.0);

	copy_column(n, lhs_rfp, k, my_copy.data(), MatrixTranspose::Identity, SymmetricFormat::RFP);

	return std::make_pair(std::move(my_copy), std::vector<double>(lhs_full + k * n, lhs_full + k * n + n));
}

std::pair<std::vector<double>, std::vector<double>> test_copy_transpose(int n, int k) {
	auto init_triangular = make_random_matrices(n, true);

	auto lhs_full = init_triangular.first.get();
	auto lhs_rfp = init_triangular.second.get();

	std::vector<double> my_copy(n, 0.0);
	std::vector<double> reference(n, 0.0);

	copy_column(n, lhs_rfp, k, my_copy.data(), MatrixTranspose::Transpose, SymmetricFormat::RFP);
	strided_copy(lhs_full + k, lhs_full + k + n * n, reference.begin(), n, 1);

	return std::make_pair(std::move(my_copy), std::move(reference));
}

std::pair< std::vector<double>, std::vector<double>> test_copy_symmetric(int n, int k) {
	auto init_symmetric = make_random_matrices(n);

	auto lhs_full = init_symmetric.first.get();
	auto lhs_rfp = init_symmetric.second.get();

	std::vector<double> my_copy(n, 0.0);
	std::vector<double> reference(n, 0.0);

	copy_column(n, lhs_rfp, k, my_copy.data(), MatrixTranspose::Identity, SymmetricFormat::RFP, true);
	std::copy(lhs_full + k * n, lhs_full + k * n + n, reference.begin());

	return std::make_pair(std::move(my_copy), std::move(reference));
}

}

TEST_CASE("Copy Column Correct for RFP (even / first)", "[RFP]") {
	auto result = test_copy(4, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (even / second)", "[RFP]") {
	auto result = test_copy(4, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / first)", "[RFP]") {
	auto result = test_copy(5, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / second)", "[RFP]") {
	auto result = test_copy(5, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (even / first / transpose)", "[RFP]") {
	auto result = test_copy_transpose(4, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (even / second / transpose)", "[RFP]") {
	auto result = test_copy_transpose(4, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / first / transpose)", "[RFP]") {
	auto result = test_copy_transpose(5, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / second / transpose)", "[RFP]") {
	auto result = test_copy_transpose(5, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / first / symm", "[RFP]") {
	auto result = test_copy_symmetric(5, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (odd / second / symm", "[RFP]") {
	auto result = test_copy_symmetric(5, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (even / first / symm", "[RFP]") {
	auto result = test_copy_symmetric(4, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for RFP (even / second / symm", "[RFP]") {
	auto result = test_copy_symmetric(4, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Column Correct for Full Symmetric", "[RFP]") {
	blas_size n = 5;
	blas_size k = 2;

	auto init_symmetric = make_random_matrices(n);

	auto lhs_full = init_symmetric.first.get();
	auto lhs_rfp = init_symmetric.second.get();

    std::vector<double> expected(lhs_full + k * n, lhs_full + k * n + n);

	std::fill(lhs_full, lhs_full + n * n, 0.0);
	blas_size info;
	dtfttr("N", "L", &n, lhs_rfp, lhs_full, &n, &info);

	std::vector<double> my_copy(n, 0.0);

	copy_column(n, lhs_full, k, my_copy.data(), MatrixTranspose::Identity, SymmetricFormat::RFP, true);

	REQUIRE_THAT(my_copy, Catch::Matchers::Equals(expected));
}

namespace {

std::pair<std::vector<double>, std::vector<double>> test_copy_add(int n, int k) {
	auto init_symmetric = make_random_matrices(n);

	auto lhs_full = init_symmetric.first.get();
	auto lhs_rfp = init_symmetric.second.get();

	std::vector<double> my_copy(n);
	std::generate(my_copy.begin(), my_copy.end(), [counter = 0]() mutable { return counter++; });
	std::vector<double> expected(my_copy.begin(), my_copy.end());

	double a = 3.0;
	copy_add_column(n, lhs_rfp, k, a, my_copy.data(), SymmetricFormat::RFP);

	std::transform(
		lhs_full + k * n, lhs_full + k * n + n, expected.begin(), expected.begin(), [=](double x, double y) { return a * x + y; }
	);

	return std::make_pair(std::move(my_copy), std::move(expected));
}

}

TEST_CASE("Copy Add Column Correct for RFP (odd / first)", "[RFP]") {
	auto result = test_copy_add(5, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Add Column Correct for RFP (odd / second)", "[RFP]") {
	auto result = test_copy_add(5, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Add Column Correct for RFP (even / first)", "[RFP]") {
	auto result = test_copy_add(4, 1);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Add Column Correct for RFP (even / second)", "[RFP]") {
	auto result = test_copy_add(4, 3);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Copy Add Column Correct for Full Triangular", "[RFP]") {
    blas_size n = 5;
    auto init_symmetric = make_random_matrices(n);

	auto lhs_full = init_symmetric.first.get();
	auto lhs_rfp = init_symmetric.second.get();
    auto lhs_tri = std::vector<double>(n * n, 0.0);
    dlacpy("L", &n, &n, lhs_full, &n, lhs_tri.data(), &n);

	std::vector<double> my_copy(n);
	std::generate(my_copy.begin(), my_copy.end(), [counter = 0]() mutable { return counter++; });
	std::vector<double> expected(my_copy.begin(), my_copy.end());

    double alpha = 0.5;
    blas_size one_i = 1;

    SECTION("First Column") {
        blas_size col_index = 0;
        copy_add_column(n, lhs_tri.data(), col_index, alpha, my_copy.data(), SymmetricFormat::Full);
        daxpy(&n, &alpha, lhs_full + col_index * n, &one_i, expected.data(), &one_i);

        REQUIRE_THAT(my_copy, Catch::Matchers::Equals(expected));
    }

    SECTION("Middle Column") {
        blas_size col_index = 2;
        copy_add_column(n, lhs_tri.data(), col_index, alpha, my_copy.data(), SymmetricFormat::Full);
        daxpy(&n, &alpha, lhs_full + col_index * n, &one_i, expected.data(), &one_i);

        REQUIRE_THAT(my_copy, Catch::Matchers::Equals(expected));
    }

    SECTION("Last Column") {
        blas_size col_index = n - 1;
        copy_add_column(n, lhs_tri.data(), col_index, alpha, my_copy.data(), SymmetricFormat::Full);
        daxpy(&n, &alpha, lhs_full + col_index * n, &one_i, expected.data(), &one_i);

        REQUIRE_THAT(my_copy, Catch::Matchers::Equals(expected));
    }

}


namespace {

std::pair<std::vector<double>, std::vector<double>> test_index(int n) {
	auto init_symmetric = make_random_matrices(n, true);

	auto lhs_full = init_symmetric.first.get();
	auto lhs_rfp = init_symmetric.second.get();

	std::vector<double> my_copy(n * (n + 1) / 2);
	std::vector<double> reference(n * (n + 1) / 2);

	int counter = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j <= i; ++j) {
            double* rfp_ptr = index_rfp(n, lhs_rfp, i, j);
            REQUIRE(rfp_ptr - lhs_rfp < n * (n + 1) / 2);
            REQUIRE(rfp_ptr - lhs_rfp >= 0);
			my_copy[counter] = *rfp_ptr;
			reference[counter] = lhs_full[i + j * n];
            counter += 1;
		}
	}

	return std::make_pair(std::move(my_copy), std::move(reference));
}}


TEST_CASE("Index Correct for RFP (even)", "[RFP]") {
	auto result = test_index(4);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

TEST_CASE("Index Correct for RFP (odd)", "[RFP]") {
	auto result = test_index(5);

	REQUIRE_THAT(result.first, Catch::Matchers::Equals(result.second));
}

