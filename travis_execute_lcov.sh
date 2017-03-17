#!/bin/bash
#
# Execute lcov.
#

# Creating report
cd ${TRAVIS_BUILD_DIR}
lcov --directory obj/ --base-directory ./ --capture --output-file coverage.info
lcov --list coverage.info

# Uploading report to CodeCov
bash <(curl -s https://codecov.io/bash) -t 87eab1ad-9a4e-4385-a498-70fe89c01020 || echo "Codecov did not collect coverage reports"

exit 0

