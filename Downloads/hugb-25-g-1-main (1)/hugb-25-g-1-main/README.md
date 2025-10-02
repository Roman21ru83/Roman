# Hugb2025Template

This is a template project that contains the structure for the project work in T-303-HUGB Software Engineering, Fall 2025.

Please make sure to read the [Code of Conduct](https://gitlab.com/grischal/hugb2025template/-/blob/main/code-of-conduct.md).

## Folder Structure
The ```docs``` folder will contain all your documentation throughout the project. For sprint-specific documents, create folders for each sprint. Documents that you might maintain throughout the project can be placed directly in the docs folder.

The ```src``` folder will contain all source code that you write throughout the project, starting from Sprint 2 onwards.

## Running the server and the tests

To run the server, execute <code>flask --app src/server.py run</code> from the root directory. You need to install the requirements first (and potentially use a venv, depending on your Python setup).

To run individual test files, execute the modules using unittest. For example:
<code>python -m unittest src.tests.test_create_task</code>
You can also execute all files with a certain filename pattern using discover:
<code>python -m unittest discover -s src/tests -t src -p "test_*.py"</code>

## Running coverage analysis and reporting coverage results

<code>Coverage</code> is run in a similar way as the unit tests. Use the following commands to run tests for coverage analysis, report results on the command line or as HTML, and configure coverage to only include your source code. For further uses, check the official documentation of coverage.

<code>coverage run -m unittest src.tests.test_create_task</code>         # runs coverage over a single test module
<code>coverage run --append -m unittest src.tests.test_set_status</code> # append another module to the same coverage data
<code>coverage run --source src -m unittest discover -s src/tests -t src -p "test_*.py"</code>  # run all tests; collect coverage only for src/
<code>coverage report -m</code>                                          # textual coverage summary
<code>coverage html</code>                                              # HTML coverage report in htmlcov/