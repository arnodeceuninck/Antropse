# Zorg dat bovenaan cmakelist tussen de CXX en EXE flags dit staat: 
# -fprofile-arcs -ftest-coverage
# Run de test code eens, om de coverage reports te genereren
# Voer vervolgens de code hieronder uit 
lcov -c -d . -o name.info
genhtml name.info
