#!/usr/bin/env bash
set -e

# add other scripts to run here
. Scripts/test_project.sh

exec "$@"
