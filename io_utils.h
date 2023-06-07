//
// Created by benjamin on 26.04.23.
//

#ifndef SAT_SOLVER_IO_UTILS_H
#define SAT_SOLVER_IO_UTILS_H

#include <string>
#include <vector>
#include <bitset>

#include "lorina/dimacs.hpp"

#include "solver_structs.h"
#include "encoding_util.h"

class Reader : public lorina::dimacs_reader
{
private:
    Cnf &cnf;

public:
    explicit Reader(Cnf &cnf) : cnf(cnf) {}

    void on_format(const std::string &format) const override
    {
        std::cout << format << std::endl;
    };

    void on_number_of_clauses(uint64_t number_of_clauses) const override
    {

    }

    void on_number_of_variables(uint64_t number_of_variables) const override
    {
        cnf.number_of_variables = number_of_variables;
    }
    void on_clause(const std::vector<int> &clause_input) const override
    {
        Clause clause_store;

        for (int literal: clause_input) {
            clause_store.push_back(internal_representation(literal));
        }
        cnf.clauses.push_back(clause_store);


    }
    void on_end() const override
    {

    }


};

Cnf import_from_file(std::string);

std::string output_dimacs(std::vector<bool>);

std::vector<bool> bitset_to_vector(std::bitset<64> bitset);

#endif //SAT_SOLVER_IO_UTILS_H
