#!/bin/sh

exec "$@" | sed -e 's@\\@/@g' -e 's@\([a-zA-Z]\):[/]@/\1/@'
