////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     nodes.i (interfaces)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#define SWIG_FILE_WITH_INIT
// #include "../../libraries/model/include/Node.h"
#include "AccumulatorNode.h"
%}

%rename (ModelGraph) model::Model;

%include "AccumulatorNode.h"
