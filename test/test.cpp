//
// Created by benjamin on 26.04.23.
//


#include "../io_utils.h"
#include "test_util.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Input") {
    Solver s;
    import_from_file("../../example_dimacs/unit_test_import.cnf", s);
    std::vector<std::vector<int>> expected_clauses = {{-2, 4, 5}, {-1, 6, -7},
                                                      {-1, -3, -5}, {4, 6, 8},
                                                      {1, 5, -7}};
    std::vector<Clause> expected_clauses_internal = internal_representation(expected_clauses);
    //s.antecedent_clauses.emplace_back(s.clauses[0]);
    CHECK(s.clauses == expected_clauses_internal);
    CHECK(s.watch_lists.size() == 20);
    CHECK(s.assignments.size() == 10);
    CHECK(s.watch_lists[internal_representation(-7)].empty());
    CHECK(s.watch_lists[internal_representation(7)].empty());
    CHECK(s.watch_lists[internal_representation(6)].empty());
    CHECK(s.watch_lists[internal_representation(-6)].size() == 2);
    CHECK(s.watch_lists[internal_representation(2)].front() == s.clauses[0]);
}

TEST_CASE("Solver propagation routine") {
    Solver s;
    s.setNumberOfVariables(3);
    s.addClauses({{1,-2,3}, {-1,-3,-2}, {1}});
    s.propagate();
    check_assignments(s, {TRUE, UNASSIGNED, UNASSIGNED});
    check_clauses(s, {{1,-2,3}, {-3,-2,-1}});
    check_watchlists(s, {{}, {0}, {0,1}, {}, {1}, {}});
    Solver s1;
    s1.setNumberOfVariables(3);
    s1.addClauses({{1,-2,3}, {-1,-3,-2}, {1}, {-1,2}});
    s1.propagate();
    check_assignments(s1, {TRUE, TRUE, FALSE});
    check_clauses(s1, {{1,-2,3}, {-3,-2,-1}, {2,-1}});
    check_watchlists(s1, {{2}, {0}, {0,1}, {2}, {1}, {}});
}

TEST_CASE("Propagation in one clause") {
    Solver s;
    s.setNumberOfVariables(3);
    s.addClause(internal_representation({1,-2,3}));
    check_watchlists(s, {{},{0},{0},{},{},{}});
    s.clauses[0].propagate(s, internal_representation(2));
    s.assignments[1] = TRUE;
    // this has to be checked for the general propagation function
    // CHECK(s.watch_lists[internal_representation(2)].empty());
    CHECK(s.watch_lists[internal_representation(-3)].front() == s.clauses[0]);
    CHECK(s.watch_lists[internal_representation(-1)].front() == s.clauses[0]);
    s.clauses[0].propagate(s, internal_representation(-1));
    CHECK(s.watch_lists[internal_representation(-3)].front() == s.clauses[0]);
    CHECK(s.watch_lists[internal_representation(-1)].front() == s.clauses[0]);
    CHECK(s.propagation_queue.front() == internal_representation(3));

}

TEST_CASE("Solver enqueue") {

}

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


/*TEST_CASE("Apply Unit Propagation") {
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
}*/

TEST_CASE("Negate Literal") {
    REQUIRE(negate_literal(0) == 1);
    REQUIRE(negate_literal(1) == 0);
    REQUIRE(negate_literal(10) == 11);
    REQUIRE(negate_literal(13) == 12);
}