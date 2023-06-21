//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include "solver.h"
#include "clause.h"
#include "encoding_util.h"


void Solver::addClause(const Clause& clause) {
    if (clause.literals.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (clause.literals.size() == 1) {
        enqueue(clause.literals.back());
    } else {
        clauses.push_back(clause);
        // Add clause to the watchlist of the negation of the two first elements
        watch_lists[negate_literal(clause.literals[0])].push_back(std::ref(clauses.back()));
        watch_lists[negate_literal(clause.literals[1])].push_back(std::ref(clauses.back()));
    }
}

bool Solver::enqueue(Literal_t literal, std::optional<std::reference_wrapper<Clause>> reason) {
    if (value(literal) != UNASSIGNED) {
        if (value(literal) == FALSE) {
            // Conflicting assignment found
            while (!propagation_queue.empty()) {
                propagation_queue.pop();
            }
            return false;
        } else {
            // This variable was already assigned
            return true;
        }
    } else {
        assignments[var_index(literal)] = lsign(literal);
        trail.push_back(literal);
        // level[var_index(literal)] = currentDecisionLevel();
        antecedent_clauses[var_index(literal)] = reason;
        propagation_queue.push(literal);
        return true;
    }
}

lbool Solver::value(Literal_t l) {
    if (sign(l)) {
        return assignments[var_index(l)];
    } else {
        return !assignments[var_index(l)];
    }
}

void Solver::setNumberOfVariables(int number) {
    assignments.clear();
    decision_levels.clear();
    antecedent_clauses.clear();
    watch_lists.clear();
    for (; number > 0; --number) {
        assignments.push_back(UNASSIGNED);
        decision_levels.push_back(DECISION_LEVEL_UNASSIGNED);
        antecedent_clauses.emplace_back();
        watch_lists.emplace_back();
        watch_lists.emplace_back();
    }

}