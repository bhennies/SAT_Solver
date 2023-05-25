//
// Created by benjamin on 26.04.23.
//


#include "../io_utils.h"
#include "../dpll.h"
#include "../encoding_util.h"
#include "../solver_structs.h"

#include <catch2/catch_test_macros.hpp>

INITIALIZE_EASYLOGGINGPP

TEST_CASE("Input/Output in dimacs format") {
    std::vector<bool> assignment = {0,1,0,1};
    REQUIRE(output_dimacs({0,1,0,1}) == "v -1 2 -3 4");
    REQUIRE(internal_representation(-1) == 1);
    REQUIRE(internal_representation(1) == 0);
    REQUIRE(internal_representation(-5) == 9);
    REQUIRE(internal_representation(13) == 24);
    REQUIRE(dimacs_format(1) == -1);
    REQUIRE(dimacs_format(12) == 7);
    REQUIRE(dimacs_format(0) == 1);
}


TEST_CASE("Apply Unit Propagation") {
    Cnf cnf_1 = import_from_file("../../example_dimacs/unit_propagation_test_1.cnf");
    apply_unit_propagation(cnf_1);
    CHECK(cnf_1.clauses.empty());
    Assignments expected_assignments = from_dimacs_list( {-1, -2, -3});
    CHECK(cnf_1.assignments == expected_assignments);
    Cnf cnf_2_before = import_from_file("../../example_dimacs/unit_propagation_test_2_before.cnf");
    Cnf cnf_2_after = import_from_file("../../example_dimacs/unit_propagation_test_2_after.cnf");
    apply_unit_propagation(cnf_2_before);
    CHECK(cnf_2_after == cnf_2_before);
    Assignments expected_assignments_2 = from_dimacs_list( {1});
    CHECK(cnf_2_before.assignments == expected_assignments_2);
    // Unittest for detecting conflicts
    Cnf conflict = import_from_file("../../example_dimacs/unit_propagation_test_simple_conflict.cnf");
    CHECK(!apply_unit_propagation(conflict));
    // function applied to empty clause list
    Cnf cnf_empty;
    CHECK(apply_unit_propagation(cnf_empty));
}

TEST_CASE("Negate Literal") {
    REQUIRE(negate_literal(0) == 1);
    REQUIRE(negate_literal(1) == 0);
    REQUIRE(negate_literal(10) == 11);
    REQUIRE(negate_literal(13) == 12);
}