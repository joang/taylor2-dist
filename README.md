# taylor version 2

taylor generates an Ordinary Differential Equation (ODE) solver that

1. writes source and header codes automatically,
2. has a high level of reusability, and
3. is thread-safe.

Moreover, taylor also

4. allows flexible arithmetic definition,
5. provides highly-accurate numerical solutions,
6. incorporates jet transport, and
7. computes directional, first-order, and high-order variational equations.

We recommend reviewing the manual for further details.


#### Authors

 Joan Gimeno, Angel Jorba, and Maorong Zou

---

## Changelog

### Nov 7, 2022 -- v2.0.1

* Minor updates
    1. Removed static variables number_of_jet_vars, number_of_symbols,
      degree_of_jet_vars, monomial_counts, monomial_offsets and
      size_of_jet_variable from output files.
    2. Rename symbolic constants _SIZE_OF_JET_VAR_ to
      _MAX_SIZE_OF_JET_VAR, _DEGREE_OF_JET_VARS to
      _MAX_DEGREE_OF_JET_VARS and _NUMBER_OF_SYMBOLS to
      _NUMBER_OF_MAX_SYMBOLS
    3. In taylor_step_xxx, allow user to pass NULL to ht, and order.
    4. Update Makefile in doc/example2 to use -jet_library jet_1.
    
### Sep 15, 2022 -- v2.0.0

* First commit
