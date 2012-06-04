#!/bin/bash

function show_syntax
{
    echo "Syntax: $0 <namespace> <class name>" 1>&2
}

if [ -z "$1" ]; then
    show_syntax $*
    exit 1
fi

if [ -z "$2" ]; then
    show_syntax $*
    exit 1
fi

outer_namespace=Lvk
outer_namespace_upper=`echo $outer_namespace | tr [a-z] [A-Z]`

namespace=$1
namespace_upper=`echo $namespace | tr [a-z] [A-Z]`

classname=$2
classname_upper=`echo $classname | tr [a-z] [A-Z]`
classname_lower=`echo $classname | tr [A-Z] [a-z]`

header_macro=${outer_namespace_upper}_${namespace_upper}_${classname_upper}_H

header_file=$classname_lower.h
source_file=$classname_lower.cpp


######################################
# make header file
######################################

echo "Creating header file $header_file..."

cat > $header_file <<EOF
/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef $header_macro
#define $header_macro

namespace $outer_namespace
{

namespace $namespace
{

/**
 * \brief $classname class 
 */

class $classname
{
public:
    $classname();

};

} // namespace $namespace

} // namespace $outer_namespace


#endif // $header_macro

EOF


######################################
# make source file
######################################

echo "Creating source file $source_file..."

cat > $source_file <<EOF
/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "$classname_lower.h"

//--------------------------------------------------------------------------------------------------
// $classname
//--------------------------------------------------------------------------------------------------

$outer_namespace::$namespace::$classname::$classname()
{
}

EOF

echo "Finished!"



